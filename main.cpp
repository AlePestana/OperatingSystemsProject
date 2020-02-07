#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>

using namespace std;

struct Proceso{
    vector<int> pagM; //Indice del vector M en el que estan las paginas del proceso
    vector<int> pagS; //Indice del vector S (de swapping) en el que estan las paginas del proceso
    clock_t t_start; //Tiempo en el que inicia el proceso
    int page_faults; //Numero de swap-outs
    int turnaround; //Tiempo de salida - Tiempo de inicio
    Proceso(int t_s) : pagM(), pagS(), t_start(t_s), page_faults(0), turnaround(-1) { }
};

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

//Funcion Swap-Out con algoritmo FIFO
void swapOutFIFO(vector<int> &M, vector<int> &S, queue<int> &queueM, int direccionV, int idProceso) {
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
                cout << "Página " << indice << " del proceso " << M[indice] << " swappeada al marco " << j << " del area de swapping";
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
}
//Funcion Swap-In con algoritmo FIFO
void swapInFIFO(vector<int> &M, vector<int> &S, queue<int> &queueM, int direccionV, int idProceso) {
    //Si no hay espacio disponible en M, se hace swap-out
    if (!(find(M.begin(), M.end(), 0) != M.end())) {
        //Ponemos 16 como parametro porque en este caso solo queremos sacar una página para meter la que se nos pide accesar
        swapOutFIFO(M, S, queueM, 16, idProceso);
    }
    bool iterar = true;
    int numPag = direccionV/16;
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
            cout << "Se localizo la pagina  " << numPag << " del proceso " << S[pagMover] << " que estaba en la posicion " << pagMover << " de swapping y se cargo al marco " << j;
            procesos[ind_procesos[idProceso]].pagS.erase(procesos[ind_procesos[idProceso]].pagS.begin() + pagMover);
            procesos[ind_procesos[idProceso]].pagM.push_back(j);
            iterar = false;
        }
        ++j;
    }
    //Se cambia la memoria S a 0
    S[pagMover] = 0;
    
}

//Funcion que libere las paginas del proceso de queue
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
}

//Revisar si M tiene espacio disponible de tamano n
bool hay_espacio_en_M(int n){
    int count=0;
    for(int &i:M) if(i==0) ++count;
    return count>=n;
}


void cargar_a_memoria(int id, int tamano){
    //Se revisa que hay espacio disponible en la memoria M
    if(!hay_espacio_en_M(tamano)){
        //Swap-out de M
        //Update de pagM y de pagS en el proceso
    }
    //Llenar M en el espacio encontrado
    //Se llena pagM en el proceso
    cout << "Indices asignados:\n";
    for(int i=0; i<M.size(); ++i) {
        if(i==0){
            M[i]=id;
            procesos[ind_procesos[id]].pagM.push_back(i);
            cout << i << " ";
        }
    }
    cout << "\n\n";
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
    liberarQueue(M, p, queueM);
    for(int i:procesos[ind_procesos[p]].pagM){
        M[i] = 0;
    }
    for(int i:procesos[ind_procesos[p]].pagS){
        S[i] = 0;
    }
    //Calcular el turnaround time
    procesos[ind_procesos[p]].turnaround = clock()-procesos[ind_procesos[p]].t_start;
    cout << "Turnaround:\n" << procesos[ind_procesos[p]].turnaround << "ms \n";
    //Paginas en M liberadas
    cout << "Paginas en M liberadas:\n";
    for(int i: procesos[ind_procesos[p]].pagM) cout << i << " ";
    cout << "\n";
    //Paginas en S liberadas
    cout << "Paginas en S liberadas:\n";
    for(int j: procesos[ind_procesos[p]].pagS) cout << j << " ";
    //Borro registro de paginas del proceso
    procesos[ind_procesos[p]].pagM.clear();
    procesos[ind_procesos[p]].pagS.clear();
}

int main(){

    //Se abre el archivo
    ifstream arch("ArchivoTrabajo.txt");

    //Se inicializan variables
    string comando;
    int dato1, dato2, dato3;
    string linea;


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
                    cout << linea.substr(2);
                cout << "\n";
            break;

           //Exit (sale el programa) 
            case 'e': 
                cout << "Gracias por utilizarme!\nHasta la proxima! :)\n";
                return 0;            
            break;
            
            //Fin (devolver turnaround promedio, num de page faults, num total de swap-in y swap-out)
            case 'f': 
                turnaroundPromedio=0;

                //Se imprimen estadisticas
                for(auto &it: ind_procesos){
                    turnaroundPromedio+=procesos[it.second].turnaround;
                    cout << "Turnaround de proceso " << it.first << ": " << procesos[it.second].turnaround << "\n";
                }
                turnaroundPromedio/=procesos.size();
                cout << "Turnaround promedio: " << turnaroundPromedio << "ms" << endl;
                for(auto &it: ind_procesos){
                    cout << "Page Faults del proceso " << it.first << ": " << procesos[it.second].page_faults << "\n";
                }
                cout << "Total de swap-ins: " << sIn << "\n";
                cout << "Total de swap-outs: " << sOut << "\n";

                //Se liberan todos los datos
                sIn=0;
                sOut=0;
                while(!queueM.empty()) queueM.pop();
                ind_procesos.clear();
                procesos.clear();
                fill(M.begin(),M.end(), 0);
                fill(S.begin(),S.end(), 0);
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
