#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Proceso{
    vector<int> pagM;
    vector<int> pagS;
    int t_start;
    int page_faults;
    int turnaround;
    Proceso(int t_s) : pagM(), pagS(), t_start(t_s), page_faults(0), turnaround(-1) { }
};

//Memoria real
vector<int> M(128,0);
//Area de swapping
vector<int> S(128,0);
//Tiempo actual
int t_actual;
//Mapa de procesos
unordered_map<int,int> ind_procesos;
vector<Proceso> procesos;
//Queues para Memoria M y Memoria S
queue<int> queueM;
queue<int> queueS;

//Funcion Swap-Out con algoritmo FIFO
void swapOutFIFO(vector<int> &M, vector<int> &S, queue<int> &queueM, queue<int> &queueS, int direccionV, int &page_faults) {
    bool iterar = true;
    //Conocer el numero de marcos por usar
    int numPag = direccionV/16;
    //Para cada pagina a "swappear"...
    for (int i = 0; i < numPag; i++) {
        //El primero que entro
        int indice = queueM.front();
        while (iterar) {
            int i = 0;
            //Si se encuentra una casilla vacía en S
            if (S[i] == 0){
                //Swap Out
                S[i] = M[indice];
                queueS.push(i);
                iterar = false;
            }
            i++;
        }
        //Se cambia la memoria M a 0
        M[indice] = 0;
        queueM.pop();
        //Se agrega errores
        page_faults++;
    }
}
//Funcion Swap-In con algoritmo FIFO
void swapInFIFO(vector<int> &M, vector<int> &S, queue<int> &queueM, queue<int> &queueS, int direccionV, int &page_faults) {
    if (!(find(M.begin(), M.end(), 0) != M.end())) {
        swapOutFIFO(M, S, queueM, queueS, direccionV, page_faults);
    }
    bool iterar = true;
    int numPag = direccionV/16;
    //Para cada pagina a "swappear"...
    for (int i = 0; i < numPag; i++) {
        //El primero que entro
        int indice = queueS.front();
        while (iterar) {
            int i = 0;
            //Si se encuentra una casilla vacía en M
            if (M[i] == 0){
                //Swap In
                M[i] = S[indice];
                queueM.push(i);
                iterar = false;
            }
            i++;
        }
        //Se cambia la memoria S a 0
        S[indice] = 0;
        queueS.pop();
    
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
    cout << "A " << d << " " << p << " " << m << "\n";
}

bool hay_espacio_en_M(int n){
    //Revisar si M tiene espacio disponible de tamano n
    return true;
}


void cargar_a_memoria(int id, int tamano){
    //Se revisa que hay espacio disponible en la memoria M
    if(!hay_espacio_en_M(tamano)){
        //Swap-out de M
        // ++procesos[id].page_faults;
        //Update de pagM y de pagS en el proceso
    }
    //Llenar M en el espacio encontrado
    //Se llena pagM en el proceso
}

/*
Funcion de cargar proceso:
n: Cantidad de bytes de solicitud que se cargan a memoria
p: Numero de proceso

*/
void P(int n, int p){
    cout << "P " << n << " " << p << "\n";
    cout << "--------------\n";
    //Revisa si existe en mapa de procesos, si no, se crea
    if(!ind_procesos.count(p)){
        //Se crea proceso
        Proceso procesoP(t_actual);
        cout << "Se crea el proceso" << endl;
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
    cout << "L " << p << "\n";
    //Se libera la memoria
    for(int i:procesos[ind_procesos[p]].pagM){
        M[i] = 0;
    }
    for(int i:procesos[ind_procesos[p]].pagS){
        S[i] = 0;
    }
    cout << "--------------\n";
    //Calcular el turnaround time
    procesos[ind_procesos[p]].turnaround = t_actual-procesos[ind_procesos[p]].t_start;
    cout << "Turnaround:\n" << procesos[ind_procesos[p]].turnaround << "\n";
    //Paginas en M liberadas
    cout << "Paginas en M:\n";
    for(int i: procesos[ind_procesos[p]].pagM) cout << i << "\n";
    cout << "\n";
    //Paginas en S liberadas
    cout << "Paginas en S:\n";
    for(int j: procesos[ind_procesos[p]].pagS) cout << j << "\n";
    cout << "--------------\n" << endl;
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

            //Fin (devolver turnaround promedio, num de page faults, num total de swap-in y swap-out)
            case 'f': 
                

            break;

            //Exit
            case 'e': 
                
                return 0;            
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
