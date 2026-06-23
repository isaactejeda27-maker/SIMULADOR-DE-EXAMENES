#include "funciones.h" //libreria para funciones
#include <fstream> //libreria para manejo de textos
#include <sstream> //manejo de cadenas y conversiones
#include <direct.h> //control de directorios en windows
#include <cctype> //manejo de caracteres en la tabla ASCII
#include <io.h> //archivos de bajo nivel, solo para windows

//nombramiento de invocacion de los espacios de texto
using std::cin;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::to_string;

//copiamos textos desde el archivo caracter por caracter, Mas seguro para el buffer
static void copiarTexto(const string& src, char* dst, int maxTam) {
    int copiados = 0;
    for (int i = 0; i < maxTam - 1 && i < (int)src.size(); ++i) {
        dst[i] = src[i];
        ++copiados;
    }
    dst[copiados] = '\0';
}

//creamos un nodo en la memoria dinamica con sus punteros en nulos
static nodo* crearNodo(const pregunta& info) {
    nodo* nuevo = new nodo;
    nuevo->info = info;
    nuevo->sig = nullptr;
    nuevo->atras = nullptr;
    return nuevo;
}

//libreracion de la memoria dinamica
static void liberarLista(nodo* inicio) {
    while (inicio != nullptr) {
        nodo* siguiente = inicio->sig;
        delete inicio;
        inicio = siguiente;
    }
}

//insercion de nodo a la lista
static void anadirNodo(nodo*& inicio, nodo*& fin, const pregunta& info) {
    nodo* nuevo = crearNodo(info);
    if (inicio == nullptr) {
        inicio = fin = nuevo;
    } else {
        fin->sig = nuevo;
        nuevo->atras = fin;
        fin = nuevo;
    }
}

static vector<string> listarExamenes() {
    vector<string> archivos;
    _finddata_t datos;
    intptr_t manejador = _findfirst("Examenes\\*.txt", &datos);
    if (manejador == -1) return archivos;
    do {
        archivos.push_back(datos.name);
    } while (_findnext(manejador, &datos) == 0);
    _findclose(manejador);
    return archivos;
}

static bool elegirArchivo(const vector<string>& archivos, string& seleccionado) {
    if (archivos.empty()) {
        cout << "No se encontraron examenes en la carpeta Examenes.\n";
        cout << "Presiona cualquier tecla para volver...";
        getch();
        return false;
    }

    while (true) {
        cout << "Archivos disponibles:\n";
        for (int i = 0; i < (int)archivos.size(); ++i) {
            cout << "  " << (i + 1) << ") " << archivos[i] << "\n";
        }
        cout << "Selecciona el numero del examen (0 para cancelar): ";

        string linea;
        getline(cin, linea);
        if (linea.empty()) {
            continue;
        }

        int opcion = 0;
        stringstream ss(linea);
        if (!(ss >> opcion) || opcion < 0 || opcion > (int)archivos.size()) {
            cout << "Opcion invalida. Intenta de nuevo.\n";
            continue;
        }

        if (opcion == 0) {
            return false;
        }

        seleccionado = archivos[opcion - 1];
        return true;
    }
}

static int contarPreguntas(nodo* inicio) {
    int total = 0;
    while (inicio != nullptr) {
        ++total;
        inicio = inicio->sig;
    }
    return total;
}

Key obtenerTecla() {
    int c = _getch();
    if (c == 224) {
        c = _getch();
        if (c == 72) return Key::UP;
        if (c == 80) return Key::DOWN;
        if (c == 75) return Key::LEFT;
        if (c == 77) return Key::RIGHT;
    } else if (c == '\r' || c == '\n') {
        return Key::ENTER;
    }
    return Key::NONE;
}   

void dibujarMenuPrincipal(const vector<string>& opciones, int seleccionada) {
    limpiarPantalla();
    cout << "|=========SIMULADOR DE EXAMENES========|\n";
    cout << "|----------------MENU------------------|\n";

    for (size_t i = 0; i < opciones.size(); ++i) {
        if (i == seleccionada)
            cout << "\033[7m > " << opciones[i] << " \033[0m\n";
        else
            cout << "  " << opciones[i] << "\n";
    }
}

void generarExamen() {
    cout << "\033[2J\033[H";
    cout << "==== GENERADOR DE EXAMENES ====" << "\n";
    cout << "Crea un examen en la carpeta Examenes\n\n";

    string nombreArchivo;
    while (true) {
        cout << "Nombre del examen (sin .txt, sin espacios): ";
        getline(cin, nombreArchivo);
        if (nombreArchivo.empty()) {
            cout << "El nombre no puede estar vacio.\n";
            continue;
        }

        bool valido = true;
        for (char c : nombreArchivo) {
            if (isspace(static_cast<unsigned char>(c))) {
                valido = false;
                break;
            }
        }

        if (!valido) {
            cout << "No uses espacios en el nombre. Intenta de nuevo.\n";
            continue;
        }
        break;
    }

    const char carpeta[] = "Examenes";
    _mkdir(carpeta);

    string ruta = string(carpeta) + "/" + nombreArchivo + ".txt";
    ofstream archivo(ruta);
    if (!archivo) {
        cout << "\nError: no se pudo crear el archivo '" << ruta << "'\n";
        cout << "Asegurate de tener permisos y que la carpeta exista.\n";
        cout << "Presiona cualquier tecla para volver...";
        getch();
        return;
    }

    archivo << ":p;EXAMEN " << nombreArchivo << "\n\n";
    int preguntaNum = 1;

    while (true) {
        cout << "\033[2J\033[H";
        cout << "==== GENERADOR DE EXAMENES ====" << "\n";
        cout << "Archivo: " << ruta << "\n";
        cout << "Pregunta " << preguntaNum << "\n";

        string textoPregunta;
        cout << "Ingresa la pregunta:\n> ";
        getline(cin, textoPregunta);
        if (textoPregunta.empty()) {
            cout << "La pregunta no puede estar vacia.\n";
            continue;
        }
        archivo << ":p;" << textoPregunta << "\n";

        for (int i = 1; i <= 4; ++i) {
            string opcion;
            while (true) {
                cout << "Opcion " << i << ":\n> ";
                getline(cin, opcion);
                if (opcion.empty()) {
                    cout << "La opcion no puede estar vacia.\n";
                    continue;
                }
                archivo << ":op" << i << ";" << opcion << "\n";
                break;
            }
        }

        int correcta = 0;
        while (true) {
            string linea;
            cout << "Opcion correcta (1-4): ";
            getline(cin, linea);
            if (linea.size() == 1 && linea[0] >= '1' && linea[0] <= '4') {
                correcta = linea[0] - '0';
                break;
            }
            cout << "Respuesta invalida. Ingresa un numero entre 1 y 4.\n";
        }
        archivo << ":r;op" << correcta << "\n";

        int puntos = 0;
        while (true) {
            string linea;
            cout << "Puntos asignados para esta pregunta: ";
            getline(cin, linea);
            stringstream ss(linea);
            if (ss >> puntos && puntos > 0) {
                break;
            }
            cout << "Ingresa un numero entero mayor que 0.\n";
        }
        archivo << puntos << ".\n\n";

        preguntaNum++;
        string opcionFinal;
        cout << "Agregar otra pregunta? (s/n): ";
        getline(cin, opcionFinal);
        if (opcionFinal.empty() || (opcionFinal[0] != 's' && opcionFinal[0] != 'S')) {
            break;
        }
    }

    archivo.close();
    cout << "\nExamen guardado en: " << ruta << "\n";
    cout << "Presiona cualquier tecla para volver al menu...";
    getch();
}

// Carga el examen en una lista doblemente enlazada.
static bool cargarExamenDesdeArchivo(const string& ruta, nodo*& inicio, nodo*& fin, int& totalPuntos) {
    //Inicializamos 
    inicio = fin = nullptr;
    totalPuntos = 0;

    //validamos si se pudo leer el archivo
    ifstream archivo(ruta);
    if (!archivo) return false;

    //empleo de una cadena string para lectura de reactivos
    string linea;
    while (getline(archivo, linea)) { 
        if (linea.rfind(":p;", 0) != 0) { //encontrar un reactivo
            continue;
        }

        if (linea.rfind(":p;EXAMEN", 0) == 0) {
            continue; // Cabecera del examen, no es un reactivo.
        }

        //inicializamos a un dato tipo pregunta
        pregunta preguntaActual{};
        copiarTexto(linea.substr(3), preguntaActual.p, MAX_TEXTO_P);
        preguntaActual.s = 0;
        preguntaActual.r = 0;
        preguntaActual.puntos = 0;


        for (int i = 1; i <= 4; ++i) {
            if (!getline(archivo, linea)) break;
            string prefix = ":op" + to_string(i) + ";";
            if (linea.rfind(prefix, 0) == 0) { 
                const string texto = linea.substr(prefix.size());
                if (i == 1) copiarTexto(texto, preguntaActual.op1, MAX_TEXTO_OP);
                if (i == 2) copiarTexto(texto, preguntaActual.op2, MAX_TEXTO_OP);
                if (i == 3) copiarTexto(texto, preguntaActual.op3, MAX_TEXTO_OP);
                if (i == 4) copiarTexto(texto, preguntaActual.op4, MAX_TEXTO_OP);
            }
        }

        if (!getline(archivo, linea)) break;
        if (linea.rfind(":r;op", 0) == 0 && linea.size() >= 6) {
            preguntaActual.r = linea[5] - '0';
        }

        if (!getline(archivo, linea)) break;
        string puntosTexto;
        for (char c : linea) {
            if (isdigit(static_cast<unsigned char>(c))) puntosTexto.push_back(c);
            else break;
        }
        if (!puntosTexto.empty()) {
            preguntaActual.puntos = atoi(puntosTexto.c_str());
        }

        anadirNodo(inicio, fin, preguntaActual);
        totalPuntos += preguntaActual.puntos;
    }

    return inicio != nullptr;
}

// Guarda la lista de reactivos de vuelta en el archivo.
// Complejidad: O(n) donde n es el numero de preguntas en memoria.
static bool guardarExamenAArchivo(const string& ruta, nodo* inicio) {
    ofstream archivo(ruta);
    if (!archivo) return false;

    nodo* actual = inicio;
    while (actual != nullptr) {
        pregunta& q = actual->info;
        archivo << ":p;" << q.p << "\n";
        archivo << ":op1;" << q.op1 << "\n";
        archivo << ":op2;" << q.op2 << "\n";
        archivo << ":op3;" << q.op3 << "\n";
        archivo << ":op4;" << q.op4 << "\n";
        archivo << ":r;op" << q.r << "\n";
        archivo << q.puntos << ".\n\n";
        actual = actual->sig;
    }

    return true;
}

static void mostrarPreguntaParaEdicion(nodo* actual, int indice, int total) {
    cout << "\033[2J\033[H";
    cout << "==== EDITOR DE EXAMENES ====\n";
    cout << "Reactivo " << indice << " de " << total << "\n";
    cout << "Pregunta: " << actual->info.p << "\n";
    cout << "1) " << actual->info.op1 << "\n";
    cout << "2) " << actual->info.op2 << "\n";
    cout << "3) " << actual->info.op3 << "\n";
    cout << "4) " << actual->info.op4 << "\n";
    cout << "Respuesta correcta: op" << actual->info.r << "\n";
    cout << "Puntos: " << actual->info.puntos << "\n\n";
    cout << "[Izq/Dch] moverse, E editar, G guardar, S salir\n";
}

static void editarPregunta(nodo* actual) {
    cout << "\033[2J\033[H";
    cout << "==== EDITAR REACTIVO ====\n";
    cout << "Pregunta actual: " << actual->info.p << "\n";

    string texto;
    cout << "Nueva pregunta (deja vacio para mantener actual):\n> ";
    getline(cin, texto);
    if (!texto.empty()) copiarTexto(texto, actual->info.p, MAX_TEXTO_P);

    for (int i = 1; i <= 4; ++i) {
        const char* actualTexto = (i == 1 ? actual->info.op1 : i == 2 ? actual->info.op2 : i == 3 ? actual->info.op3 : actual->info.op4);
        cout << "Opcion " << i << " actual: " << actualTexto << "\n";
        cout << "Nueva opcion " << i << " (deja vacio para mantener actual):\n> ";
        getline(cin, texto);
        if (texto.empty()) continue;
        if (i == 1) copiarTexto(texto, actual->info.op1, MAX_TEXTO_OP);
        if (i == 2) copiarTexto(texto, actual->info.op2, MAX_TEXTO_OP);
        if (i == 3) copiarTexto(texto, actual->info.op3, MAX_TEXTO_OP);
        if (i == 4) copiarTexto(texto, actual->info.op4, MAX_TEXTO_OP);
    }

    while (true) {
        cout << "Respuesta correcta actual: op" << actual->info.r << "\n";
        cout << "Nueva respuesta correcta (1-4, enter para mantener): ";
        getline(cin, texto);
        if (texto.empty()) break;
        if (texto.size() == 1 && texto[0] >= '1' && texto[0] <= '4') {
            actual->info.r = texto[0] - '0';
            break;
        }
        cout << "Valor invalido.\n";
    }

    while (true) {
        cout << "Puntos actuales: " << actual->info.puntos << "\n";
        cout << "Nuevos puntos (enter para mantener): ";
        getline(cin, texto);
        if (texto.empty()) break;
        stringstream ss(texto);
        int puntos;
        if (ss >> puntos && puntos > 0) {
            actual->info.puntos = puntos;
            break;
        }
        cout << "Ingresa un numero entero mayor que 0.\n";
    }
}

void modificarExamen() {
    cout << "\033[2J\033[H";
    cout << "==== MODIFICAR EXAMEN ====\n";
    const char carpeta[] = "Examenes";
    _mkdir(carpeta);

    vector<string> archivos = listarExamenes();
    string nombreArchivo;
    if (!elegirArchivo(archivos, nombreArchivo)) {
        return;
    }

    string ruta = string(carpeta) + "/" + nombreArchivo;
    nodo* inicio = nullptr;
    nodo* fin = nullptr;
    int totalPuntos = 0;
    if (!cargarExamenDesdeArchivo(ruta, inicio, fin, totalPuntos)) {
        cout << "No se pudo cargar el examen.\n";
        cout << "Presiona cualquier tecla para volver...";
        _getch();
        liberarLista(inicio);
        return;
    }

    nodo* actual = inicio;
    int indice = 1;
    const int total = contarPreguntas(inicio);
    int seleccionMenu = 0; // 0: editar, 1: guardar, 2: anterior, 3: siguiente, 4: salir

    while (true) {
        dibujarMenuModificacion(actual, indice, total, seleccionMenu);
        Key tecla = obtenerTecla();

        switch (tecla) {
            case Key::UP:
                if (seleccionMenu > 0) --seleccionMenu;
                break;
            case Key::DOWN:
                if (seleccionMenu < 4) ++seleccionMenu;
                break;
            case Key::LEFT:
                if (actual->atras != nullptr) {
                    actual = actual->atras;
                    --indice;
                    seleccionMenu = 0;
                }
                break;
            case Key::RIGHT:
                if (actual->sig != nullptr) {
                    actual = actual->sig;
                    ++indice;
                    seleccionMenu = 0;
                }
                break;
            case Key::ENTER:
                if (seleccionMenu == 0) {
                    // Editar la pregunta actual (ventana de texto normal)
                    editarPregunta(actual);
                } else if (seleccionMenu == 1) {
                    // Guardar
                    if (guardarExamenAArchivo(ruta, inicio)) {
                        cout << "Examen guardado correctamente.\n";
                    } else {
                        cout << "Error al guardar.\n";
                    }
                    cout << "Presiona cualquier tecla para continuar...";
                    _getch();
                } else if (seleccionMenu == 2) {
                    if (actual->atras != nullptr) {
                        actual = actual->atras;
                        --indice;
                        seleccionMenu = 0;
                    }
                } else if (seleccionMenu == 3) {
                    if (actual->sig != nullptr) {
                        actual = actual->sig;
                        ++indice;
                        seleccionMenu = 0;
                    }
                } else if (seleccionMenu == 4) {
                    // Salir sin guardar
                    break;
                }
                break;
            default:
                break;
        }

        if (tecla == Key::ENTER && seleccionMenu == 4) break; // salir del bucle
    }

    liberarLista(inicio);
}

static void mostrarPreguntaParaAplicacion(nodo* actual, int indice, int total) {
    cout << "\033[2J\033[H";
    cout << "==== APLICAR EXAMEN ====\n";
    cout << "Reactivo " << indice << " de " << total << "\n";
    cout << "Pregunta: " << actual->info.p << "\n";
    cout << "1) " << actual->info.op1 << "\n";
    cout << "2) " << actual->info.op2 << "\n";
    cout << "3) " << actual->info.op3 << "\n";
    cout << "4) " << actual->info.op4 << "\n";
    cout << "Respuesta seleccionada: ";
    if (actual->info.s == 0) cout << "(sin responder)\n";
    else cout << "op" << actual->info.s << "\n";
    cout << "Puntos de esta pregunta: " << actual->info.puntos << "\n\n";
    cout << "[Izq/Dch] moverse, 1-4 responder, F finalizar prueba\n";
}

void aplicarExamen() {
    cout << "\033[2J\033[H";
    cout << "==== APLICAR EXAMEN ====\n";
    const char carpeta[] = "Examenes";
    _mkdir(carpeta);

    vector<string> archivos = listarExamenes();
    string nombreArchivo;
    if (!elegirArchivo(archivos, nombreArchivo)) {
        return;
    }

    string ruta = string(carpeta) + "/" + nombreArchivo;
    nodo* inicio = nullptr;
    nodo* fin = nullptr;
    int totalPuntos = 0;
    if (!cargarExamenDesdeArchivo(ruta, inicio, fin, totalPuntos)) {
        cout << "No se pudo cargar el examen.\n";
        cout << "Presiona cualquier tecla para volver...";
        _getch();
        liberarLista(inicio);
        return;
    }

    nodo* actual = inicio;
    int indice = 1;
    const int total = contarPreguntas(inicio);
    int cursor = 0;  

    while (true) {
        dibujarMenuAplicacion(actual, indice, total, cursor);
        Key tecla = obtenerTecla();

        switch (tecla) {
            case Key::UP:
                if (cursor > 0) --cursor;
                break;
            case Key::DOWN:
                if (cursor < 4) ++cursor;
                break;
            case Key::LEFT:
                if (actual->atras != nullptr) {
                    actual = actual->atras;
                    --indice;
                    cursor = 0; 
                }
                break;
            case Key::RIGHT:
                if (actual->sig != nullptr) {
                    actual = actual->sig;
                    ++indice;
                    cursor = 0;
                }
                break;
            case Key::ENTER:
                if (cursor >= 0 && cursor <= 3) {
                    actual->info.s = cursor + 1;
                    
                } else if (cursor == 4) {
                    // Finalizar
                    goto finalizar_examen;
                }
                break;
            default:
                break;
        }
    }

    finalizar_examen:
    // Calcular puntuación
    int puntosLogrados = 0;
    nodo* recorrido = inicio;
    int preguntasContadas = 0;
    while (recorrido != nullptr) {
        if (recorrido->info.s == recorrido->info.r) {
            puntosLogrados += recorrido->info.puntos;
        }
        ++preguntasContadas;
        recorrido = recorrido->sig;
    }

    cout << "\033[2J\033[H";
    cout << "==== RESULTADO DEL EXAMEN ====\n";
    cout << "Total preguntas: " << preguntasContadas << "\n";
    cout << "Puntos obtenidos: " << puntosLogrados << " / " << totalPuntos << "\n";
    cout << "Presiona cualquier tecla para volver al menu...";
    _getch();

    liberarLista(inicio);
}

void dibujarMenuAplicacion(nodo* actual, int indice, int total, int cursor) {
    limpiarPantalla();
    cout << "==== APLICAR EXAMEN ====\n";
    cout << "Reactivo " << indice << " de " << total << "\n";
    cout << "Pregunta: " << actual->info.p << "\n\n";

    const char* textos[] = {actual->info.op1, actual->info.op2,
                            actual->info.op3, actual->info.op4};
    for (int i = 0; i < 4; ++i) {
        bool esRespuestaGuardada = (actual->info.s == i + 1);
        bool esCursor = (cursor == i);

        cout << (esRespuestaGuardada ? " * " : "   ");
        if (esCursor) cout << "\033[7m";
        cout << (i + 1) << ") " << textos[i] << "\033[0m\n";
    }

    bool esCursorFinal = (cursor == 4);
    cout << "\n   ";
    if (esCursorFinal) cout << "\033[7m";
    cout << "FINALIZAR examen\033[0m\n";

    cout << "\nUsa las flechas para moverte, Enter para seleccionar.\n";
}

void dibujarMenuModificacion(nodo* actual, int indice, int total, int seleccionMenu) {
    limpiarPantalla();
    cout << "==== EDITOR DE EXAMENES ====\n";
    cout << "Reactivo " << indice << " de " << total << "\n";
    cout << "Pregunta: " << actual->info.p << "\n\n";

    cout << "1) " << actual->info.op1 << "\n";
    cout << "2) " << actual->info.op2 << "\n";
    cout << "3) " << actual->info.op3 << "\n";
    cout << "4) " << actual->info.op4 << "\n\n";

    cout << "Respuesta correcta: op" << actual->info.r << "\n";
    cout << "Puntos: " << actual->info.puntos << "\n\n";

    vector<string> acciones = {
        "EDITAR esta pregunta",
        "GUARDAR examen",
        "Ir a la pregunta ANTERIOR",
        "Ir a la pregunta SIGUIENTE",
        "SALIR sin guardar"
    };

    for (int i = 0; i < (int)acciones.size(); ++i) {
        if (i == seleccionMenu)
            cout << "\033[7m > " << acciones[i] << " \033[0m\n";
        else
            cout << "   " << acciones[i] << "\n";
    }
    cout << "\nUsa las flechas para elegir accion, Enter para ejecutar.\n";
}

void limpiarPantalla() {
    system("cls");
}