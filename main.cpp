#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Proceso{
    vector<int> pagM;
    vector<int> pagS;
    int t_start;
    int page_faults=0;
    int turnaround=-1;
    Proceso(int t_s) : t_start(t_s) { }
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
        procesos[ind_procesos[p]] = Proceso(t_actual);
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