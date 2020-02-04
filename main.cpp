#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(){
    ifstream arch("ArchivoTrabajo.txt");
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