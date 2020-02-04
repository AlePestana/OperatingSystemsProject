#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

int main(){

    //Se abre el archivo
    ifstream arch("ArchivoTrabajo.txt");

    //Se inicializan variables
    vector<int> M(128,0);
    vector<int> S(128,0);
    unordered_map<int, vector<int>> debug_map;
    char comando;
    int dato1, dato2, dato3;
    string linea;

    while(getline(arch, linea)){
        istringstream datos(linea);
        datos >> comando;
        switch(comando|32){
            //Accesar dirección virtual
            case 'a': 
                if(datos >> dato1 >> dato2 >> dato3)

            break;

            //Comentario
            case 'c': 
                if(linea.length() > 2)
                cout << linea.substr(2) << "\n";
            break;

            //Fin (devolver turnaround promedio, num de page faults, num total de swap-in y swap-out)
            case 'f': 
                

            break;

            //Exit
            case 'e': 
            
            break;

            //Liberar páginas de un proceso
            case 'l': 
                datos >> dato1;
                //Liberar memoria en dato1

            break;
            
            //Cargar un proceso
            case 'p': 
                if(datos >> dato1 >> dato2)

            break;
            
            //Ignorar linea en caso default
        }
    }

    arch.close();
    return 0;
}