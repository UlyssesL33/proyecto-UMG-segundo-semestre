// Control Academico - Ingenieria en Sistemas UMG
// Proyecto integrador: Algoritmos, Algebra Lineal y Matematica Discreta
// Menu principal: cada opcion llama a la funcion de su modulo.

#include <iostream>
#include <string>

#include "config.h"
#include "modelos.h"
#include "pantalla.h"
#include "validaciones.h"
#include "cursos.h"
#include "estudiantes.h"
#include "archivos.h"
#include "notas.h"
#include "estadisticas.h"
#include "discreta.h"
#include "algebra.h"

using namespace std;

// Datos compartidos por las opciones del menu.
Curso cursos[MAX_CURSOS];
int   numCursos = 0;

Estudiante estudiantes[MAX_ESTUDIANTES];
int        numEstudiantes = 0;

// Nombre de cada opcion del menu, en la posicion de su numero.
// Se usa en el menu y en el encabezado de cada pantalla.
const int NUM_OPCIONES = 12;
const string NOMBRES_OPCION[NUM_OPCIONES] = {
    "Salir",                                  // 0
    "Registrar estudiante",                   // 1
    "Cursos y notas de un estudiante",        // 2
    "Consultar estudiante (por carne)",       // 3
    "Listar estudiantes (global o por curso)",// 4
    "Modificar estudiante",                   // 5
    "Eliminar estudiante",                    // 6
    "Estadisticas",                           // 7
    "Algebra Lineal",                         // 8
    "Matematica Discreta",                    // 9
    "Guardar informacion",                    // 10
    "Ver pensum"                              // 11
};

// Opcion 11: consultar el pensum completo o un curso por su codigo.
void menuPensum() {
    opcionMenu(1, "Ver pensum completo por ciclo");
    opcionMenu(2, "Buscar curso por codigo");
    opcionMenu(0, "Regresar");
    int sub = leerEntero("\n  Seleccione: ", 0, 2);

    if (sub == 1) {
        listarPensum(cursos, numCursos);
    } else if (sub == 2) {
        string codigo = leerTexto("  Codigo del curso (ej. 090008): ");
        int pos = buscarCurso(cursos, numCursos, codigo);
        if (pos == -1) {
            mensajeError("No existe un curso con codigo " + codigo + ".");
        } else {
            mostrarCurso(cursos, numCursos, pos);
        }
    }
}

// Listado global con promedios o listado de un curso.
void menuListar() {
    opcionMenu(1, "Global: todos los estudiantes con su promedio");
    opcionMenu(2, "Por curso: estudiantes y notas de un curso");
    opcionMenu(0, "Regresar");
    int sub = leerEntero("\n  Seleccione: ", 0, 2);

    if (sub == 1) {
        listarEstudiantes(estudiantes, numEstudiantes, numCursos);
    } else if (sub == 2) {
        listarPorCurso(estudiantes, numEstudiantes, cursos, numCursos);
    }
}

// Opcion 10 (y al salir): guarda los estudiantes en el archivo.
void guardarInformacion() {
    if (guardarEstudiantes(estudiantes, numEstudiantes, cursos, numCursos, RUTA_ESTUDIANTES)) {
        mensajeExito("Informacion guardada en " + RUTA_ESTUDIANTES + " ("
                     + to_string(numEstudiantes) + " estudiante(s)).");
    } else {
        mensajeError("No se pudo escribir " + RUTA_ESTUDIANTES + ".");
    }
}

void mostrarMenu() {
    encabezado("Menu principal");

    cout << pintar("  Estudiantes: ", GRIS) << numEstudiantes
         << pintar("    Cursos del pensum: ", GRIS) << numCursos
         << pintar("    Nota minima: ", GRIS) << NOTA_MINIMA << "\n";

    grupoMenu("ESTUDIANTES");
    for (int i = 1; i <= 6; i++) {
        opcionMenu(i, NOMBRES_OPCION[i]);
    }

    grupoMenu("REPORTES Y ANALISIS");
    for (int i = 7; i <= 9; i++) {
        opcionMenu(i, NOMBRES_OPCION[i]);
    }

    grupoMenu("SISTEMA");
    opcionMenu(10, NOMBRES_OPCION[10]);
    opcionMenu(11, NOMBRES_OPCION[11]);
    opcionMenu(0,  NOMBRES_OPCION[0]);
    cout << "\n";
    separador();
}

// Carga el pensum y los estudiantes guardados.
// Devuelve false si el programa no puede continuar (no hay pensum).
bool cargarDatos() {
    encabezado("Inicio");

    // Sin pensum el programa no puede funcionar.
    numCursos = cargarPensum(cursos, RUTA_PENSUM);

    if (numCursos == -1) {
        mensajeError("No se encontro el archivo " + RUTA_PENSUM + ".");
        mensajeAviso("Ejecute el programa desde la carpeta raiz del proyecto.");
        return false;
    }
    if (numCursos == -2) {
        mensajeError("El pensum no forma un bosque de arboles binarios valido.");
        return false;
    }
    if (numCursos == 0) {
        mensajeError("El archivo " + RUTA_PENSUM + " no tiene cursos validos.");
        return false;
    }
    mensajeExito("Pensum cargado: " + to_string(numCursos) + " cursos, "
                 + to_string(totalCreditosPensum(cursos, numCursos)) + " creditos.");

    // Estudiantes de una sesion anterior. Si el archivo no existe todavia
    // (primera vez), simplemente se empieza sin estudiantes.
    numEstudiantes = cargarEstudiantes(estudiantes, cursos, numCursos, RUTA_ESTUDIANTES);

    if (numEstudiantes == -1) {
        numEstudiantes = 0;
        mensajeAviso("No hay estudiantes guardados todavia.");
    } else {
        mensajeExito("Estudiantes cargados: " + to_string(numEstudiantes) + ".");
    }
    return true;
}

int main() {
    iniciarConsola();   // activa los colores en Windows

    if (!cargarDatos()) {
        pausar();
        return 1;
    }
    pausar();

    int opcion;

    do {
        mostrarMenu();

        // leerEntero vuelve a preguntar si se escriben letras o un numero
        // fuera de 0-11, asi que aqui opcion siempre es valida.
        opcion = leerEntero("  Seleccione una opcion: ", 0, NUM_OPCIONES - 1);

        // Cada opcion se abre en una pantalla limpia con su propio titulo
        encabezado("Menu principal > " + NOMBRES_OPCION[opcion]);

        switch (opcion) {
            case 1:  registrarEstudiante(estudiantes, numEstudiantes);                break;
            case 2:  menuCursosYNotas(estudiantes, numEstudiantes, cursos, numCursos); break;
            case 3:  consultarEstudiante(estudiantes, numEstudiantes, cursos, numCursos); break;
            case 4:  menuListar();                                                    break;
            case 5:  modificarEstudiante(estudiantes, numEstudiantes);                break;
            case 6:  eliminarEstudiante(estudiantes, numEstudiantes, cursos, numCursos); break;
            case 7:  mostrarEstadisticas(estudiantes, numEstudiantes, cursos, numCursos); break;
            case 8:  menuAlgebra(estudiantes, numEstudiantes, cursos, numCursos);     break;
            case 9:  menuDiscreta(estudiantes, numEstudiantes, cursos, numCursos);    break;
            case 10: guardarInformacion();                                            break;
            case 11: menuPensum();                                                    break;
            case 0:
                // Evita perder lo registrado si se olvido la opcion 10
                if (leerEntero("  Guardar antes de salir? (1 = Si, 0 = No): ", 0, 1) == 1) {
                    guardarInformacion();
                }
                cout << "\n  Hasta pronto.\n";
                break;
            default:
                break;   // no deberia pasar: leerEntero ya valido 0-11
        }

        // Los submenus de las opciones 2, 8 y 9 ya hacen sus propias pausas
        if (opcion != 0 && opcion != 2 && opcion != 8 && opcion != 9) {
            pausar();
        }

    } while (opcion != 0);

    return 0;
}
