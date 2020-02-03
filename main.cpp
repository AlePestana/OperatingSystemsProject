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
        switch(comando|64){
            //Accesar dirección virtual
            case 'a': break;
            //Comentario
            case 'c': break;
            //Fin (devolver turnaround promedio, num de page faults, num total de swap-in y swap-out)
            case 'f': break;
            //Exit
            case 'e': break;
            //Liberar páginas de un proceso
            case 'l': break;
            //Cargar un proceso
            case 'p': break;
            //Ignorar linea en caso default
        }
    }

    arch.close();
    return 0;
}