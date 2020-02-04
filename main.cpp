#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

//Memoria real
vector<pair<int,int>> M(128,make_pair(0,0));
//Area de swapping
vector<pair<int,int>> S(128,make_pair(0,0));

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

void P(int size, int id){
    cout << "P " << size << " " << id << "\n";
    
}

void L(int id){
    cout << "L " << id << "\n";
}

int main(){

    //Se abre el archivo
    ifstream arch("ArchivoTrabajo.txt");

    //Se inicializan variables
    unordered_map<int, vector<int>> debug_map;
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