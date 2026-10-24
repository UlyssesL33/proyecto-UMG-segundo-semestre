// =====================================================================
// arboles.cpp - Bosque de arboles binarios de prerrequisitos
// =====================================================================
#include "arboles.h"
#include "config.h"
#include "pantalla.h"
#include "cursos.h"        // buscarCurso, elegirCurso
#include "logica.h"        // estaHabilitado
#include "estudiantes.h"   // inicializarEstudiante

#include <iostream>
#include <string>

using namespace std;

// =====================================================================
// Estructura del arbol
// =====================================================================

int padreDe(const Curso cursos[], int numCursos, int curso) {
    if (cursos[curso].prerrequisito == SIN_PRERREQUISITO) {
        return -1;   // raiz: no depende de ningun curso
    }
    return buscarCurso(cursos, numCursos, cursos[curso].prerrequisito);
}

bool validarArbolesBinarios(const Curso cursos[], int numCursos) {
    if (numCursos < 0 || numCursos > MAX_CURSOS) {
        mensajeError("Cantidad de cursos fuera de la capacidad del pensum.");
        return false;
    }
    int cantidadHijos[MAX_CURSOS] = {};
    for (int c = 0; c < numCursos; c++) {
        for (int anterior = 0; anterior < c; anterior++) {
            if (cursos[anterior].codigo == cursos[c].codigo) {
                mensajeError("Codigo de curso repetido: " + cursos[c].codigo + ".");
                return false;
            }
        }
        if (cursos[c].prerrequisito == SIN_PRERREQUISITO) {
            continue;
        }
        int padre = padreDe(cursos, numCursos, c);
        if (padre == -1) {
            mensajeError("El prerrequisito de " + cursos[c].codigo + " no existe.");
            return false;
        }
        cantidadHijos[padre]++;
        if (cantidadHijos[padre] > MAX_HIJOS_BINARIOS) {
            mensajeError(cursos[padre].codigo + " tiene mas de dos hijos. El arbol debe ser binario.");
            return false;
        }
    }

    // Una cadena valida llega a una raiz en no mas de numCursos pasos.
    for (int c = 0; c < numCursos; c++) {
        int actual = c;
        int pasos = 0;
        while (actual != -1 && pasos < numCursos) {
            actual = padreDe(cursos, numCursos, actual);
            pasos++;
        }
        if (actual != -1) {
            mensajeError("Dependencia circular en el curso " + cursos[c].codigo + ".");
            return false;
        }
    }
    return true;
}

HijosBinarios hijosBinariosDe(const Curso cursos[], int numCursos, int curso) {
    HijosBinarios hijos = {-1, -1};
    for (int c = 0; c < numCursos; c++) {
        if (cursos[c].prerrequisito == cursos[curso].codigo) {
            if (hijos.izquierdo == -1) {
                hijos.izquierdo = c;
            } else {
                hijos.derecho = c;
                break;
            }
        }
    }
    return hijos;
}

int hijosDe(const Curso cursos[], int numCursos, int curso, int hijos[]) {
    HijosBinarios ramas = hijosBinariosDe(cursos, numCursos, curso);
    int cantidad = 0;
    if (ramas.izquierdo != -1) hijos[cantidad++] = ramas.izquierdo;
    if (ramas.derecho != -1) hijos[cantidad++] = ramas.derecho;
    return cantidad;
}

// ---------------------------------------------------------------------
// Recursion: los descendientes de un curso son sus hijos MAS los
// descendientes de cada hijo. Se detiene sola en las hojas (0 hijos).
// ---------------------------------------------------------------------
int contarDescendientes(const Curso cursos[], int numCursos, int curso) {
    HijosBinarios hijos = hijosBinariosDe(cursos, numCursos, curso);
    int total = 0;
    if (hijos.izquierdo != -1) {
        total += 1 + contarDescendientes(cursos, numCursos, hijos.izquierdo);
    }
    if (hijos.derecho != -1) {
        total += 1 + contarDescendientes(cursos, numCursos, hijos.derecho);
    }
    return total;
}

// ---------------------------------------------------------------------
// Recursion: una hoja tiene altura 1. Cualquier otro curso tiene
// 1 + la altura de su hijo mas "alto".
// ---------------------------------------------------------------------
int altura(const Curso cursos[], int numCursos, int curso) {
    HijosBinarios hijos = hijosBinariosDe(cursos, numCursos, curso);
    int izquierda = hijos.izquierdo == -1 ? 0 : altura(cursos, numCursos, hijos.izquierdo);
    int derecha = hijos.derecho == -1 ? 0 : altura(cursos, numCursos, hijos.derecho);
    return 1 + (izquierda > derecha ? izquierda : derecha);
}

// =====================================================================
// Dibujo del arbol
// =====================================================================

// Marca de un curso segun el estudiante.
static string marca(const Estudiante& e, const Curso cursos[], int numCursos, int curso) {
    switch (e.estado[curso]) {
        case APROBADO:  return pintar("[A]", VERDE);
        case EN_CURSO:  return pintar("[C]", AMARILLO);
        case REPROBADO: return pintar("[R]", ROJO);
        default: break;
    }
    if (estaHabilitado(e, cursos, numCursos, curso)) {
        return pintar("[H]", CIAN);
    }
    return pintar("[B]", GRIS);
}

// ---------------------------------------------------------------------
// Dibuja un curso y, recursivamente, todo lo que cuelga de el:
//
//   Algoritmos
//   `-- Programacion I            <- ultimo hijo: "`-- "
//       |-- ...                   <- hijo que no es el ultimo: "|-- "
//
// "prefijo" acumula las lineas verticales de los niveles de arriba.
// Si conMarcas es false se dibuja solo el pensum, sin el estudiante.
// ---------------------------------------------------------------------
static void dibujarRama(const Estudiante& e, bool conMarcas, const Curso cursos[], int numCursos,
                        int curso, const string& prefijo, bool esUltimo, bool esRaiz,
                        bool esDerecho = false) {
    string conector = esRaiz ? "" : (esUltimo ? "`-- " : "|-- ");

    cout << "  " << pintar(prefijo + conector, GRIS);
    if (!esRaiz) {
        cout << pintar(esDerecho ? "D: " : "I: ", GRIS);
    }
    if (conMarcas) {
        cout << marca(e, cursos, numCursos, curso) << " ";
    }
    cout << cursos[curso].codigo << " " << cursos[curso].nombre;
    if (cursos[curso].creditosMinimos > 0) {
        cout << pintar("  (min. " + to_string(cursos[curso].creditosMinimos) + " creditos)", GRIS);
    }
    cout << "\n";

    string prefijoHijos = prefijo + (esRaiz ? "" : (esUltimo ? "    " : "|   "));

    HijosBinarios hijos = hijosBinariosDe(cursos, numCursos, curso);
    if (hijos.izquierdo != -1) {
        dibujarRama(e, conMarcas, cursos, numCursos, hijos.izquierdo,
                    prefijoHijos, hijos.derecho == -1, false);
    }
    if (hijos.derecho != -1) {
        dibujarRama(e, conMarcas, cursos, numCursos, hijos.derecho,
                    prefijoHijos, true, false, true);
    }
}

// =====================================================================
// Paso 17: arbol del estudiante
// =====================================================================

void pantallaArbolEstudiante(const Estudiante& e, const Curso cursos[], int numCursos) {
    concepto("Arbol binario: cada curso tiene hasta dos hijos");
    lineaConcepto("I = hijo izquierdo; D = hijo derecho, segun su orden en el pensum.");
    lineaConcepto("Los cursos sin prerrequisito son las raices del bosque binario.");
    lineaConcepto("Raiz = curso sin prerrequisito.  Hoja = curso del que no depende ningun otro.");
    lineaConcepto("Se dibuja con RECURSION: cada curso se dibuja y luego dibuja a sus hijos.");

    cout << "\n  " << pintar("[A]", VERDE) << " aprobado  " << pintar("[C]", AMARILLO) << " en curso  "
         << pintar("[R]", ROJO) << " reprobado  " << pintar("[H]", CIAN) << " habilitado  "
         << pintar("[B]", GRIS) << " bloqueado\n";

    int hijos[MAX_HIJOS_BINARIOS];

    subtitulo("Ramas con cadena de prerrequisitos");
    for (int c = 0; c < numCursos; c++) {
        if (padreDe(cursos, numCursos, c) == -1 && hijosDe(cursos, numCursos, c, hijos) > 0) {
            dibujarRama(e, true, cursos, numCursos, c, "", true, true);
        }
    }

    subtitulo("Cursos sin cadena (raices sin hijos)");
    int enLinea = 0;
    for (int c = 0; c < numCursos; c++) {
        if (padreDe(cursos, numCursos, c) == -1 && hijosDe(cursos, numCursos, c, hijos) == 0) {
            string nombre = cursos[c].nombre;
            if (nombre.length() > 28) {
                nombre = nombre.substr(0, 27) + ".";
            }
            cout << "  " << marca(e, cursos, numCursos, c) << " " << columna(nombre, 30);
            enLinea++;
            if (enLinea % 2 == 0) {
                cout << "\n";
            }
        }
    }
    if (enLinea % 2 != 0) {
        cout << "\n";
    }
}

// =====================================================================
// Paso 18: que bloquea reprobar un curso
// =====================================================================

void pantallaBloqueos(const Curso cursos[], int numCursos) {
    // Solo tiene sentido para cursos que son prerrequisito de otro
    int opciones[MAX_CURSOS];
    int cantidad = 0;
    int hijos[MAX_HIJOS_BINARIOS];
    for (int c = 0; c < numCursos; c++) {
        if (hijosDe(cursos, numCursos, c, hijos) > 0) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }

    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos que son prerrequisito de otros");
    if (curso == -1) {
        return;
    }

    int bloqueados = contarDescendientes(cursos, numCursos, curso);

    concepto("Subarbol");
    lineaConcepto("El subarbol de un curso son el y todos sus descendientes (hijos, nietos...).");
    lineaConcepto("Si se reprueba el curso, TODO su subarbol queda bloqueado.");

    subtitulo("Subarbol de " + cursos[curso].nombre);
    Estudiante vacio;
    inicializarEstudiante(vacio);
    dibujarRama(vacio, false, cursos, numCursos, curso, "", true, true);

    cout << "\n  Reprobar " << pintar(cursos[curso].nombre, CIAN) << " bloquea "
         << pintar(to_string(bloqueados) + " curso(s)", ROJO) << " en cadena.\n";

    // Camino hacia arriba: lo que se necesita para llegar a este curso
    string camino = cursos[curso].nombre;
    int actual = padreDe(cursos, numCursos, curso);
    while (actual != -1) {
        camino = cursos[actual].nombre + " -> " + camino;
        actual = padreDe(cursos, numCursos, actual);
    }
    cout << pintar("  Camino desde la raiz: ", GRIS) << camino << "\n";
    cout << pintar("  Nota: ademas, reprobar cualquier curso resta creditos para los cursos", GRIS) << "\n"
         << pintar("  que piden un minimo de creditos aprobados.", GRIS) << "\n";
}

// =====================================================================
// Paso 18: altura de los arboles y cadena mas larga
// =====================================================================

void pantallaProfundidad(const Curso cursos[], int numCursos) {
    concepto("Altura de un arbol binario (recursiva)");
    lineaConcepto("altura(hoja) = 1");
    lineaConcepto("altura(c) = 1 + max(altura izquierda, altura derecha)");
    lineaConcepto("Cada nivel es un semestre: la altura es el minimo de semestres de esa cadena.");

    subtitulo("Arboles del pensum con mas de un nivel");
    cout << pintar("  " + columna("Raiz", 46) + columna("Altura", 8) + "Descendientes", GRIS) << "\n";

    int hijos[MAX_HIJOS_BINARIOS];
    int mejorRaiz = -1;
    int mejorAltura = 0;

    for (int c = 0; c < numCursos; c++) {
        if (padreDe(cursos, numCursos, c) != -1 || hijosDe(cursos, numCursos, c, hijos) == 0) {
            continue;
        }
        int h = altura(cursos, numCursos, c);
        cout << "  " << columna(cursos[c].codigo + " " + cursos[c].nombre, 46)
             << columna(to_string(h), 8) << contarDescendientes(cursos, numCursos, c) << "\n";

        if (h > mejorAltura) {
            mejorAltura = h;
            mejorRaiz = c;
        }
    }

    if (mejorRaiz == -1) {
        return;
    }

    // Recorrer la cadena mas larga: desde la raiz, bajar siempre por el
    // hijo con mayor altura hasta llegar a una hoja.
    subtitulo("Cadena de prerrequisitos mas larga (altura " + to_string(mejorAltura) + ")");
    int actual = mejorRaiz;
    int nivel = 1;
    while (actual != -1) {
        cout << "  " << pintar(to_string(nivel) + ".", CIAN) << " " << cursos[actual].nombre
             << pintar("  (ciclo " + to_string(cursos[actual].ciclo) + ")", GRIS) << "\n";

        int n = hijosDe(cursos, numCursos, actual, hijos);
        int siguiente = -1;
        int alturaSiguiente = 0;
        for (int i = 0; i < n; i++) {
            int h = altura(cursos, numCursos, hijos[i]);
            if (h > alturaSiguiente) {
                alturaSiguiente = h;
                siguiente = hijos[i];
            }
        }
        actual = siguiente;
        nivel++;
    }
    cout << "\n  Se necesitan al menos " << pintar(to_string(mejorAltura) + " semestres", CIAN)
         << " para completar esta cadena, sin reprobar ninguno.\n";
}
