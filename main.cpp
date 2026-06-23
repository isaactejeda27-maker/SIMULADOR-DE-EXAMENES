#include "funciones.h"
#include <iostream>

using namespace std;

int main(){

    vector<string> opciones = {
        "Opcion 1",
        "Opcion 2",
        "Opcion 3",
        "Salir"
    };

    int seleccionada = 0;

    bool ejecutando = true;

    while (ejecutando) {
        dibujarMenuPrincipal(opciones, seleccionada);

        Key tecla = obtenerTecla();

        switch(tecla){
            case Key::UP:
                if(seleccionada > 0) seleccionada--;
                break;
            case Key::DOWN:
                if(seleccionada < opciones.size() - 1) seleccionada++;
                break;
            case Key::ENTER:
                cout << "\nSe selecciono la opcion " << opciones[seleccionada] << "\n";
                if (opciones[seleccionada] == "Salir") ejecutando = false;
                else{ 
                    cout << "Presiona Enter para continuar";
                    cin.ignore();
                }
            break;
            default:
            break;

        }

    }

    return 0;
}