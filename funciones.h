#ifndef FUNCIONES_H   
#define FUNCIONES_H

#include <iostream>
#include <string>
#include <vector> 
#include <conio.h>

using std::string;
using std::vector;
using std::cout;

#define MAX_TEXTO_P 150
#define MAX_TEXTO_OP 50

struct pregunta{
    char p[MAX_TEXTO_P];
    char op1[MAX_TEXTO_OP];
    char op2[MAX_TEXTO_OP];
    char op3[MAX_TEXTO_OP];
    char op4[MAX_TEXTO_OP];
    int s;         
    int r;          
    int puntos;
    int numpre;
};

struct examen {
    string nombre;
    vector<pregunta> preguntas;
    int totPuntos;
};

// Nodo para la lista doblemente enlazada de preguntas
struct nodo {
    pregunta info;
    nodo* sig;
    nodo* atras;
};

enum class Key {
    UP, DOWN, LEFT, RIGHT, ENTER, NONE
};

// Declaraciones de funciones
Key obtenerTecla();
void dibujarMenuPrincipal(const vector<string>& opciones, int seleccionada);
void generarExamen();
void modificarExamen();
void aplicarExamen();

#endif // FUNCIONES_H
