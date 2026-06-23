#include "funciones.h"

Key obtenerTecla(){

    int c = getch();

    if(c == 224){
        c = getch();
        if(c == 72) return Key::UP;
        if(c == 80) return Key::DOWN;
    }
    else if (c == '\r' || c == '\n') return Key::ENTER;
    return Key::NONE;
}

void dibujarMenuPrincipal(const vector<string>& opciones, int seleccionada){
    cout << "\033[2J\033[H"; //Limpia la pantalla y reinicia el cursor

    cout << "|SIMULADOR DE EXAMENES|\n\n";

    for(int i = 0; i < opciones.size(); ++i){
        if(i == seleccionada) cout << "\033[7m > " << opciones[i] << " \033[0m\n"; // Resalta la opcion activa
        else cout << "  " << opciones[i] << '\n';
    }
    
}