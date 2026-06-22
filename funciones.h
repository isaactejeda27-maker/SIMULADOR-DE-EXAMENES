#ifndef FUNCIONES_H   // 1. Corregido: "Si NO está definido..."
#define FUNCIONES_H

#include <string>
#include <vector> // Necesario si decides usar vectores dinámicos

// Usamos el espacio de nombres estándar para poder usar 'string' y 'vector'
using std::string;
using std::vector;

struct pregunta {
    string p;       // Texto de la pregunta
    string op1;     // Opción 1
    string op2;     // Opción 2
    string op3;     // Opción 3
    char s;         // Opción seleccionada por el usuario
    char r;         // Respuesta correcta ('a', 'b', 'c')
    int puntos;     // Puntos que otorga esta pregunta
    int numpre;     // Número de la pregunta
};                  

struct examen {
    int totpre;          // Total de preguntas del examen
    int totpuntos;       // Puntos máximos posibles
    string nombre;       // Nombre del alumno o del examen
    
    // 2. Corregido: Ahora el examen puede guardar una lista de múltiples preguntas
    vector<pregunta> reactivos; 
    
    float promedio;      // Calificación final obtenida
};                  

// Declaración de tus funciones aquí abajo, por ejemplo:
// void crearExamen(examen &e);
// void calificarExamen(examen &e);

#endif // FUNCIONES_H
