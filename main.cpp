#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;

//Estructura Proceso que guarda toda la informacion necesaria de un proceso
struct Proceso{
    vector<int> pagM; //Indice del vector M en el que estan las paginas del proceso
    vector<int> pagS; //Indice del vector S (de swapping) en el que estan las paginas del proceso en el area de swapping
    clock_t t_start; //Tiempo en el que inicia el proceso
    int page_faults; //Numero de swap-outs
    int turnaround; //Tiempo de salida - Tiempo de inicio
    Proceso(int t_s) : pagM(), pagS(), t_start(t_s), page_faults(0), turnaround(-1) { }
};

  //Variables globales
//Memoria real
vector<int> M(128,0);
//Area de swapping
vector<int> S(128,0);
//Contadores swap-in y swap-out
int sIn = 0, sOut = 0;
//Turnaround promedio
double turnaroundPromedio=0;
//Mapa de procesos
unordered_map<int,int> ind_procesos;
vector<Proceso> procesos;
//Queues para Memoria M
queue<int> queueM;
//Opcion para escoger algoritmo
char opcion;


  //Funciones
//Funcion Swap-Out para algoritmos FIFO y LRU
void swapOutA(int direccionV, int idProceso) {
    bool iterar = true;
    //Conocer el numero de marcos por usar
    int cantPag = direccionV/16;
    //Para cada pagina a "swappear"...
    for (int i = 0; i < cantPag; ++i) {
        //El primero que entro
        int indice = queueM.front();
        int j = 0;
        while (iterar && j<S.size()) {
            //Si se encuentra una casilla vacía en S
            if (S[j] == 0){
                //Swap Out
                S[j] = M[indice];
                //Actualizar pagM y pagS
                cout << "- Página " << indice << " del proceso " << M[indice] << " swappeada al marco " << j << " del area de swapping";
                procesos[ind_procesos[M[indice]]].pagM.erase(procesos[ind_procesos[M[indice]]].pagM.begin() + indice);
                procesos[ind_procesos[M[indice]]].pagS.push_back(j);
                iterar = false;
            }
            ++j;
        }
        //Se cambia la memoria M a 0
        M[indice] = 0;
        queueM.pop();
        //Se agrega errores
        ++procesos[ind_procesos[idProceso]].page_faults;
    }

    //Sumar al numero de swap outs realizados
    ++sOut;
}

//Funcion que hace swap out de M
//Se llama cuando se intenta cargar un proceso a la memoria y no hay suficiente espacio
//NOTA: se utiliza la misma funcion debido a que la unica diferencia entre ambos algoritmos es el orden de la queueM a la que tienen acceso. Esto quiere decir que, lo que se modifica es el orden de la queueM, no la funcionalidad de hacer swap out (sacar un paginas de la memoria principal y pasarlas a la memoria secundaria S)
void swapOut(int idProceso, int cantPagASwap) {
    //Para cada pagina a "swappear"...
    for (int i = 0; i < cantPagASwap; ++i) {
        bool iterar = true;
        //El primero que entro
        int indice = queueM.front();
        int j = 0;
        while (iterar && j<S.size()) {
            //Si se encuentra una casilla vacía en S
            if (S[j] == 0){
                //Swap Out
                S[j] = M[indice];

                //Actualizar pagM y pagS
                cout << "- Página " << indice << " del proceso " << M[indice] << " swappeada al marco " << j << " del area de swapping\n";
                procesos[ind_procesos[M[indice]]].pagM.erase(procesos[ind_procesos[M[indice]]].pagM.begin());
                procesos[ind_procesos[M[indice]]].pagS.push_back(j);
                iterar = false;
                //Sumar al numero de swap outs realizados
                ++sOut;
            }
            ++j;
        }
        //Se cambia la memoria M a 0
        M[indice] = 0;
        queueM.pop();
        //Se agrega al numero de page faults del proceso
        ++procesos[ind_procesos[M[indice]]].page_faults;
    }
}

//Funcion Swap-In
//Su objetivo es desplegar en pantalla la informacion relacionada a una pagina que se esta tratando de accesar pero se encuentra en la memoria secundaria, por lo que se hace swap in para ponerla en la memoria principal
void swapIn(int numPag, int idProceso) {
    //Si no hay espacio disponible en M, se hace swap-out
    if (!(find(M.begin(), M.end(), 0) != M.end())) {
        //La cantidad de paginas a swappear es 1 debido a que, cuando se utiliza la funcion de accesar, solo accede a 1 pagina
        swapOut(idProceso, 1);
    }
    bool iterar = true;
    //Obtener el indice de la página en memoria S
    int pagMover = procesos[ind_procesos[idProceso]].pagS[numPag];
    int j = 0;
    //Mientras que no encuentre casilla vacía en M...
    while (iterar && j<M.size()) {
        //Si se encuentra una casilla vacía en M
        if (M[j] == 0){
            //Swap In
            M[j] = S[pagMover];
            queueM.push(j);
            //Actualizar pagM y pagS
            cout << "- Se localizo la pagina  " << numPag << " del proceso " << S[pagMover] << " que estaba en la posicion " << pagMover << " de swapping y se cargo al marco " << j;

            //Update de pagM y de pagS en el proceso
            procesos[ind_procesos[idProceso]].pagS.erase(procesos[ind_procesos[idProceso]].pagS.begin() + pagMover);
            procesos[ind_procesos[idProceso]].pagM.push_back(j);
            iterar = false;
        }
        ++j;
    }
    //Se cambia la memoria S a 0
    S[pagMover] = 0;
    //Sumar al numero de swap ins realizados
    ++sIn;
}

//Funcion que libera las paginas del proceso de queue
void liberarQueue(vector<int> memoria, int idProceso, queue<int> &queueMemoria) {
    vector<int> indicesBorrar;
    for (int i = 0; i < memoria.size(); ++i) {
        if (memoria[i] == idProceso) {
            indicesBorrar.push_back(i);
        }
    }
    queue<int> queueSubstitution;
    queueSubstitution.swap(queueMemoria);
    int tamQueueSub = queueSubstitution.size();
    for (int i = 0; i < tamQueueSub; ++i) {
        if (queueSubstitution.front() == indicesBorrar[i]) {
            queueSubstitution.pop();
        }
        queueMemoria.push(queueSubstitution.front());
        queueSubstitution.pop();
    }
}

//Funcion que se encarga de agregar una pagina a la queueM cuando es accesada
//NOTA: esta es la funcion que diferencia LRU de FIFO debido a que, la unica diferencia entre estos algoritmos es que LRU busca en la queueM a la pagina menos utilizada. Esto quiere decir que, si leo una pagina (con la funcion A), estoy "utilizando" esa pagina, por lo que seria la mas utilizada, mientras que la queueM se queda con las paginas en el orden en el que fueron cargadas a la memoria. Por lo que esta funcion se asegura de que, si se accesa una pagina, esta pasa al ultimo lugar de la queueM, indicando asi que es la pagina mas utilizada recientemente.
void agregarPagLRU(int d, int p, int m){
  //Recibir pag que quiero quitar de la queue
  //Vaciar todo de la queue a otra queue
    //Si encuentro esa pag que estoy buscando, no ponerla
  //Regresar todo a queueM
  //Agregar al final la pag (que recibi)
}

/*
Funcion de acceso:
d: Direccion virtual
p: Proceso
m: Booleano que determina si se modifica o es solo lectura
   (si es 0, solo se lee; si es 1, se modifica)
*/
void A(int d, int p, int m){
    cout << "\nA " << d << " " << p << " " << m << "\n";
    cout << "-------------\n";

    //Inicializacion del numero de pagina
    //Necesario redondear hacia arriba ya que se le debe asignar espacio a todo el proceso (y no quedar con una porcion del proceso sin el)
    int numPag = ceil(double(d)/16);
    //Inicializacion de indice en M de la pagina que se busca desplegar (i) y la direccion real (r)
    int i, r;

    //Desplazamiento
    int desplazamiento = d%16;

    //Buscar id del proceso en M
    if(find(M.begin(), M.end(), p) != M.end() &&
       find(procesos[ind_procesos[p]].pagM.begin(), procesos[ind_procesos[p]].pagM.end(), numPag) != procesos[ind_procesos[p]].pagM.end()) {
      //Si esta, buscar la r = indice de pagM de la estructura proceso, por lo que, buscar el indice (de M) en el que la pag esta en el vector pagM
      i = procesos[ind_procesos[p]].pagM[numPag];
    } else {
      //Si pagina no esta, buscarla en el area de swapping (S)
      swapIn(numPag,p);
    }
    //Si no esta en S, desplegar error de que la pagina no existe

    //Calculo de direccion real: r * 16 + desplazamiento
    r = i*16+desplazamiento;
    //Si el algoritmo escogido por el usuario es LRU, llamar funcion adicional para declarar la pagina accesada como la mas usada recientemente
    if(opcion == 'L' || opcion == 'l') {
    //Agregar pagina de proceso accesada a la queueM
    agregarPagLRU(r,p,m);
  }

    cout << "- Dir. Virtual: " << d << "\n- Dir. Real: " << r << "\n\n";
}

//Revisar si M tiene espacio disponible de tamano n
int hay_espacio_en_M(int n){
    int count=0;
    for(int &i:M) if(i==0) ++count;
    if(count < n) {
      return n-count;
    }
    return 0;
}

//Funcion para cargar un proceso a memoria
//Si no hay suficiente espacio, utiliza alguno de los dos algoritmos para hacer swpping
void cargar_a_memoria(int id, int tamano){

    tamano = ceil(double(tamano)/16);
    if(double(tamano)/16 > 128) {
      cout << "De acuerdo a lo declarado en las instrucciones, la longitud maxima de un proceso debe ser de 2048 bytes.\n";
    } else {
      //Se revisa que hay espacio disponible en la memoria M
      if(hay_espacio_en_M(tamano) > 0){
          //Swap-out de M
          swapOut(id, tamano);
      }
      //Llenar M en el espacio encontrado
      //Se llena pagM en el proceso
      cout << "Indices asignados:\n";
      int cantPags = tamano;
      int i = 0;
      while(cantPags != 0 && i <= M.size()) {
          if(M[i]==0){
              M[i]=id;
              procesos[ind_procesos[id]].pagM.push_back(i);
              queueM.push(i);
              cout << i << " ";
              cantPags--;
          }
          ++i;
      }
      cout << "\n\n";
  }
}

/*
Funcion de cargar proceso:
n: Cantidad de bytes de solicitud que se cargan a memoria
p: Numero de proceso

*/
void P(int n, int p){
    cout << "\nP " << n << " " << p << "\n";
    cout << "--------------\n";
    //Revisa si existe en mapa de procesos, si no, se crea
    if(!ind_procesos.count(p)){
        //Se crea proceso
        Proceso procesoP(clock());
        cout << "Se crea el proceso " << p << endl;
        ind_procesos[p] = procesos.size();
        procesos.push_back(procesoP);
    }
    //Se carga a la memoria
    cargar_a_memoria(p,n);

}

/*
Funcion liberar paginas de proceso:
p: Numero de proceso a liberar
*/
void L(int p){
    cout << "\nL " << p << "\n";
    cout << "--------------\n";
   //Se libera la memoria
    // liberarQueue(M, p, queueM);
    for(int i:procesos[ind_procesos[p]].pagM){
        M[i] = 0;
    }
    for(int i:procesos[ind_procesos[p]].pagS){
        S[i] = 0;
    }
    //Calcular el turnaround time
    procesos[ind_procesos[p]].turnaround = clock()-procesos[ind_procesos[p]].t_start;
    cout << "- Turnaround:\n" << procesos[ind_procesos[p]].turnaround << "ms \n";
    //Paginas en M liberadas
    cout << "- Paginas en M liberadas:\n";
    for(int i: procesos[ind_procesos[p]].pagM) cout << i << " ";
    cout << "\n";
    //Paginas en S liberadas
    cout << "- Paginas en S liberadas:\n";
    for(int j: procesos[ind_procesos[p]].pagS) cout << j << " ";
    cout << "\n\n";
    //Borro registro de paginas del proceso
    procesos[ind_procesos[p]].pagM.clear();
    procesos[ind_procesos[p]].pagS.clear();
}

//Funcion F que despliega las estadisticas del programa y el rendimiento de los algoritmos
//Ademas reinica todas las estructuras de datos para poder recibir un conjunto de instrucciones nuevas
void F() {
  cout << "Estadísticas finales\n";
  cout << "--------------\n";
  turnaroundPromedio=0;

  //Se imprimen estadisticas
  for(auto &it: ind_procesos){
      turnaroundPromedio+=procesos[it.second].turnaround;
      cout << "Turnaround de proceso " << it.first << ": " << procesos[it.second].turnaround << "\n";
  }
  cout << "\n";
  turnaroundPromedio/=procesos.size();
  cout << "Turnaround promedio: " << turnaroundPromedio << "ms" << endl;
  cout << "\n";
  for(auto &it: ind_procesos){
      cout << "Page Faults del proceso " << it.first << ": " << procesos[it.second].page_faults << "\n";
  }
  cout << "\n";
  cout << "Total de swap-ins: " << sIn << "\n";
  cout << "Total de swap-outs: " << sOut << "\n";
  cout << "\n";

  //Se liberan todos los datos
  sIn=0;
  sOut=0;
  while(!queueM.empty()) queueM.pop();
  ind_procesos.clear();
  procesos.clear();
  fill(M.begin(),M.end(), 0);
  fill(S.begin(),S.end(), 0);
}

int main(){

    //Se abre el archivo
    ifstream arch("casos.txt");

    //Se inicializan variables
    string comando;
    int dato1, dato2, dato3;
    string linea;

    //Se pide cual algoritmo a utilizar para realizar el reemplazo de paginas
    cout << "Bienvenido!\n";
    cout << "Ingresa el algoritmo a utilizar (f - FIFO o l - LRU): ";
    cin >> opcion;


    while(getline(arch, linea)){
        istringstream datos(linea);
        datos >> comando;
        switch(comando[0]|32){
            //Accesar dirección virtual
            case 'a':
                if(datos >> dato1 >> dato2 >> dato3)
                    A(dato1,dato2,dato3);
            break;

            //Comentario
            case 'c':
                if(linea.length() > 2)
                    cout << "C " << linea.substr(2);
                cout << "\n";
            break;

           //Exit (sale el programa)
            case 'e':
                cout << "Gracias por utilizarme!\nHasta la proxima! :)\n";
                return 0;
            break;

            //Fin (devolver turnaround promedio, num de page faults, num total de swap-in y swap-out)
            case 'f':
              F();
            break;

            //Liberar páginas de un proceso
            case 'l':
                //Liberar memoria en dato1
                if(datos >> dato1);
                    L(dato1);
            break;

            //Cargar un proceso
            case 'p':
                if(datos >> dato1 >> dato2)
                    P(dato1,dato2);
            break;

            //Ignorar linea en caso default
        }
    }

    arch.close();
    return 0;
}
