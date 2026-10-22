// =====================================================================
// Control Academico - Ingenieria en Sistemas UMG
// Proyecto integrador: Algoritmos, Algebra Lineal y Matematica Discreta
// ---------------------------------------------------------------------
// Menu principal. Las opciones marcadas "(pendiente)" se iran llenando
// paso a paso segun el roadmap del equipo.
// =====================================================================

#include <iostream>
#include <string>

#include "config.h"        // constantes (NOTA_MINIMA, MAX_CURSOS, ...)
#include "modelos.h"       // struct Curso y struct Estudiante
#include "pantalla.h"      // encabezado, colores, mensajes, pausar
#include "validaciones.h"  // leerEntero, leerDecimal, leerTexto, leerCarne
#include "cursos.h"        // cargarPensum, buscarCurso, listarPensum
#include "estudiantes.h"   // registrar, buscar, consultar y listar estudiantes
#include "archivos.h"      // guardarEstudiantes, cargarEstudiantes
#include "notas.h"         // inscribir cursos e ingresar notas
#include "estadisticas.h"  // listado por curso y estadisticas

using namespace std;

// ---------------------------------------------------------------------
// Datos del programa
// Se declaran fuera de main (variables globales) porque los arreglos de
// struct son grandes; dentro de main podrian llenar la pila de memoria.
// ---------------------------------------------------------------------
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
    "Analisis matricial (Algebra Lineal)",    // 8
    "Arbol y cursos habilitados (Discreta)",  // 9
    "Guardar informacion",                    // 10
    "Ver pensum"                              // 11
};

// ---------------------------------------------------------------------
// Opcion 11: consultar el pensum completo o un curso por su codigo.
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Opcion 4: listado global (por promedio) o por curso (por nota).
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Opcion 10 (y al salir): guarda los estudiantes en el archivo.
// ---------------------------------------------------------------------
void guardarInformacion() {
    if (guardarEstudiantes(estudiantes, numEstudiantes, cursos, numCursos, RUTA_ESTUDIANTES)) {
        mensajeExito("Informacion guardada en " + RUTA_ESTUDIANTES + " ("
                     + to_string(numEstudiantes) + " estudiante(s)).");
    } else {
        mensajeError("No se pudo escribir " + RUTA_ESTUDIANTES + ".");
    }
}

// Opcion que todavia no esta hecha: se muestra en gris en el menu.
void opcionPendiente(int numero) {
    opcionMenu(numero, NOMBRES_OPCION[numero] + pintar("  (pendiente)", GRIS));
}

void mostrarMenu() {
    encabezado("Menu principal");

    cout << pintar("  Estudiantes: ", GRIS) << numEstudiantes
         << pintar("    Cursos del pensum: ", GRIS) << numCursos
         << pintar("    Nota minima: ", GRIS) << NOTA_MINIMA << "\n";

    grupoMenu("ESTUDIANTES");
    opcionMenu(1, NOMBRES_OPCION[1]);
    opcionMenu(2, NOMBRES_OPCION[2]);
    opcionMenu(3, NOMBRES_OPCION[3]);
    opcionMenu(4, NOMBRES_OPCION[4]);
    opcionMenu(5, NOMBRES_OPCION[5]);
    opcionMenu(6, NOMBRES_OPCION[6]);

    grupoMenu("REPORTES Y ANALISIS");
    opcionMenu(7, NOMBRES_OPCION[7]);
    opcionPendiente(8);
    opcionPendiente(9);

    grupoMenu("SISTEMA");
    opcionMenu(10, NOMBRES_OPCION[10]);
    opcionMenu(11, NOMBRES_OPCION[11]);
    opcionMenu(0,  NOMBRES_OPCION[0]);
    cout << "\n";
    separador();
}

// ---------------------------------------------------------------------
// Carga el pensum y los estudiantes guardados.
// Devuelve false si el programa no puede continuar (no hay pensum).
// ---------------------------------------------------------------------
bool cargarDatos() {
    encabezado("Inicio");

    // Sin pensum el programa no puede funcionar.
    numCursos = cargarPensum(cursos, RUTA_PENSUM);

    if (numCursos == -1) {
        mensajeError("No se encontro el archivo " + RUTA_PENSUM + ".");
        mensajeAviso("Ejecute el programa desde la carpeta raiz del proyecto.");
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
                mensajeAviso("Esta opcion todavia esta en construccion.");
                break;
        }

        // El submenu de la opcion 2 ya hace sus propias pausas
        if (opcion != 0 && opcion != 2) {
            pausar();
        }

    } while (opcion != 0);

    return 0;
}
