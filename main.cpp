#include "funciones.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(){

    vector<string> opciones = {
        "GENERAR UN EXAMEN",
        "MODIFICAR UN EXAMEN",
        "APLICAR UN EXAMEN",
        "SALIR"
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
                if (opciones[seleccionada] == "SALIR") {
                    ejecutando = false;
                } else if (opciones[seleccionada] == "GENERAR UN EXAMEN") {
                    generarExamen();
                } else if (opciones[seleccionada] == "MODIFICAR UN EXAMEN") {
                    modificarExamen();
                } else if (opciones[seleccionada] == "APLICAR UN EXAMEN") {
                    aplicarExamen();
                } else {
                    cout << "\nOpcion en desarrollo: " << opciones[seleccionada] << "\n";
                    cout << "Presiona cualquier tecla para continuar...";
                    getch();
                }
                break;
            default:
                break;
        }
    }

    return 0;
}
