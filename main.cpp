#include <iostream>
#include <stdio.h>

using namespace std;

int main(){

    int opc;
    do{
        cout << "|=========SIMULADOR DE EXAMENES========|\n";
        cout << "|----------------MENU------------------|\n";
        cout << "| - GENERAR UN EXAMEN............... 1 |" << endl;
        cout << "| - MODIFICAR UN EXAMEN............. 2 |" << endl;
        cout << "| - APLICAR UN EXAMEN............... 3 |" << endl;
        cout << "| - SALIR........................... 4 |" << endl;
        cout << "|SELECCIONE UNA OPCION: ";
        cin >> opc;

        switch (opc){
            case 1: break;
            case 2: break;
            case 3: break;
            case 4: return 0; break;
            default: 
                system("cls");
                cout << "Error, esta opcion no existe!! \n --"; 
                system("pause");
            break;
        }   
        if(opc != 4) system("cls");
    }while(opc != 4);

    return 0;
}