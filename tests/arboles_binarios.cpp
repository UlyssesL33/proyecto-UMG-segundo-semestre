#include "arboles.h"
#include "cursos.h"
#include "estudiantes.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static int errores = 0;

static void comprobar(bool condicion, const string& caso) {
    if (!condicion) {
        cerr << "FALLO: " << caso << "\n";
        errores++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso desde la raiz: prueba.exe ruta_archivo_temporal\n";
        return 2;
    }

    Curso cursos[MAX_CURSOS] = {
        {"A", "Raiz", 5, 1, SIN_PRERREQUISITO, 0},
        {"B", "Izquierdo", 5, 2, "A", 0},
        {"C", "Derecho", 5, 2, "A", 0},
        {"D", "Nieto", 5, 3, "B", 0},
        {"E", "Otra raiz", 5, 1, SIN_PRERREQUISITO, 0}
    };
    comprobar(validarArbolesBinarios(cursos, 5), "bosque binario valido");
    comprobar(validarArbolesBinarios(cursos, 0), "bosque vacio");
    HijosBinarios hijos = hijosBinariosDe(cursos, 5, 0);
    comprobar(hijos.izquierdo == 1 && hijos.derecho == 2, "dos hijos ordenados");
    hijos = hijosBinariosDe(cursos, 5, 1);
    comprobar(hijos.izquierdo == 3 && hijos.derecho == -1, "un hijo");
    hijos = hijosBinariosDe(cursos, 5, 2);
    comprobar(hijos.izquierdo == -1 && hijos.derecho == -1, "hoja");
    comprobar(altura(cursos, 5, 0) == 3, "altura con dos ramas");
    comprobar(altura(cursos, 5, -1) == 0, "altura de una rama vacia");
    comprobar(contarDescendientes(cursos, 5, 0) == 3, "descendientes de ambas ramas");
    comprobar(altura(cursos, 5, 2) == 1 && contarDescendientes(cursos, 5, 2) == 0,
              "altura y descendientes de una hoja");
    int posiciones[MAX_HIJOS_BINARIOS];
    comprobar(hijosDe(cursos, 5, 0, posiciones) == 2 && posiciones[0] == 1 && posiciones[1] == 2,
              "compatibilidad de hijosDe con las reglas de notas");

    cursos[4].prerrequisito = "A";
    comprobar(!validarArbolesBinarios(cursos, 5), "rechazar tercer hijo");
    cursos[4].prerrequisito = SIN_PRERREQUISITO;
    cursos[0].prerrequisito = "D";
    comprobar(!validarArbolesBinarios(cursos, 5), "rechazar ciclo entre cursos");
    cursos[0].prerrequisito = "A";
    comprobar(!validarArbolesBinarios(cursos, 1), "rechazar autorreferencia");
    cursos[0].prerrequisito = SIN_PRERREQUISITO;
    cursos[4].prerrequisito = "NO_EXISTE";
    comprobar(!validarArbolesBinarios(cursos, 5), "rechazar padre inexistente");
    cursos[4].prerrequisito = SIN_PRERREQUISITO;
    cursos[4].codigo = "A";
    comprobar(!validarArbolesBinarios(cursos, 5), "rechazar codigos duplicados");

    ofstream temporal(argv[1]);
    temporal << "A|Raiz|5|1|000000|0\nB|Uno|5|2|A|0\n"
             << "C|Dos|5|2|A|0\nD|Tres|5|2|A|0\n";
    temporal.close();
    comprobar(!temporal.fail(), "preparar archivo temporal");
    comprobar(cargarPensum(cursos, argv[1]) == -2, "cargador rechaza pensum no binario");

    int cantidad = cargarPensum(cursos, "datos/pensum.txt");
    comprobar(cantidad == 50, "cargar los 50 cursos reales");
    if (cantidad == 50) {
        int visitados = 0;
        for (int c = 0; c < cantidad; c++) {
            if (padreDe(cursos, cantidad, c) == -1) {
                visitados += 1 + contarDescendientes(cursos, cantidad, c);
            }
        }
        comprobar(visitados == cantidad, "conservar todos los cursos del bosque");
        int precalculo = buscarCurso(cursos, cantidad, "090006");
        comprobar(precalculo != -1, "encontrar Precalculo");
        if (precalculo != -1) {
            comprobar(altura(cursos, cantidad, precalculo) == 5 &&
                      contarDescendientes(cursos, cantidad, precalculo) == 6,
                      "conservar las dos cadenas de Precalculo");
        }

        Estudiante estudiante;
        inicializarEstudiante(estudiante);
        ostringstream pantalla;
        streambuf* salidaOriginal = cout.rdbuf(pantalla.rdbuf());
        pantallaArbolEstudiante(estudiante, cursos, cantidad);
        cout.rdbuf(salidaOriginal);
        comprobar(pantalla.str().find("I: ") != string::npos &&
                  pantalla.str().find("D: ") != string::npos,
                  "mostrar los lados izquierdo y derecho");
    }

    cout << (errores == 0 ? "Todas las pruebas de arboles binarios pasaron.\n"
                         : "Hay pruebas fallidas.\n");
    return errores == 0 ? 0 : 1;
}
