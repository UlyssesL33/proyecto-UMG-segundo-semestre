// =====================================================================
// Control Academico - Ingenieria en Sistemas UMG
// Proyecto integrador: Algoritmos, Algebra Lineal y Matematica Discreta
// ---------------------------------------------------------------------
// Menu principal. Las opciones que dicen "En construccion" se iran
// llenando paso a paso segun el roadmap del equipo.
// =====================================================================

#include <iostream>
#include <string>
#include <cstdlib>   // system()

#include "config.h"        // constantes (NOTA_MINIMA, MAX_CURSOS, ...)
#include "modelos.h"       // struct Curso y struct Estudiante
#include "validaciones.h"  // leerEntero, leerDecimal, leerTexto, leerCarne
#include "cursos.h"        // cargarPensum, buscarCurso, listarPensum
#include "estudiantes.h"   // registrar, buscar, consultar y listar estudiantes
#include "archivos.h"      // guardarEstudiantes, cargarEstudiantes

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

// Limpia la consola. Se usa solo desde aqui para que, si algun dia
// hay que cambiar el comando, se cambie en un unico lugar.
void limpiarPantalla() {
    system("cls");
}

// Detiene el programa hasta que el usuario presione Enter,
// para que alcance a leer el resultado antes de volver al menu.
void pausar() {
    cout << "\nPresione Enter para continuar...";
    string basura;
    getline(cin, basura);
}

// ---------------------------------------------------------------------
// Opcion 11: consultar el pensum completo o un curso por su codigo.
// ---------------------------------------------------------------------
void menuPensum() {
    cout << "\n1. Ver pensum completo por ciclo\n";
    cout << "2. Buscar curso por codigo\n";
    int sub = leerEntero("Seleccione: ", 1, 2);

    if (sub == 1) {
        listarPensum(cursos, numCursos);
    } else {
        string codigo = leerTexto("Codigo del curso (ej. 090008): ");
        int pos = buscarCurso(cursos, numCursos, codigo);
        if (pos == -1) {
            cout << "  No existe un curso con codigo " << codigo << ".\n";
        } else {
            mostrarCurso(cursos, numCursos, pos);
        }
    }
}

// ---------------------------------------------------------------------
// Opcion 10 (y al salir): guarda los estudiantes en el archivo.
// ---------------------------------------------------------------------
void guardarInformacion() {
    if (guardarEstudiantes(estudiantes, numEstudiantes, cursos, numCursos, RUTA_ESTUDIANTES)) {
        cout << "\n  Informacion guardada en " << RUTA_ESTUDIANTES
             << " (" << numEstudiantes << " estudiante(s)).\n";
    } else {
        cout << "\n  Error: no se pudo escribir " << RUTA_ESTUDIANTES << ".\n";
    }
}

void mostrarMenu() {
    cout << "=============================================\n";
    cout << "   CONTROL ACADEMICO - ING. EN SISTEMAS UMG\n";
    cout << "=============================================\n";
    cout << " 1. Registrar estudiante\n";
    cout << " 2. Inscribir curso / ingresar notas\n";
    cout << " 3. Consultar estudiante (por carne)\n";
    cout << " 4. Listar estudiantes (global o por curso)\n";
    cout << " 5. Modificar estudiante\n";
    cout << " 6. Eliminar estudiante\n";
    cout << " 7. Estadisticas\n";
    cout << " 8. Analisis matricial        (Algebra)\n";
    cout << " 9. Arbol y cursos habilitados (Discreta)\n";
    cout << "10. Guardar informacion\n";
    cout << "11. Ver pensum\n";
    cout << " 0. Salir\n";
    cout << "---------------------------------------------\n";
}

int main() {
    // Sin pensum el programa no puede funcionar: se carga antes del menu.
    numCursos = cargarPensum(cursos, RUTA_PENSUM);

    if (numCursos == -1) {
        cout << "Error: no se encontro el archivo " << RUTA_PENSUM << "\n";
        cout << "Ejecute el programa desde la carpeta raiz del proyecto.\n";
        return 1;
    }
    if (numCursos == 0) {
        cout << "Error: el archivo " << RUTA_PENSUM << " no tiene cursos validos.\n";
        return 1;
    }

    cout << "Pensum cargado: " << numCursos << " cursos, "
         << totalCreditosPensum(cursos, numCursos) << " creditos.\n";

    // Estudiantes guardados en una sesion anterior. Si el archivo no
    // existe todavia (primera vez), simplemente se empieza sin estudiantes.
    numEstudiantes = cargarEstudiantes(estudiantes, cursos, numCursos, RUTA_ESTUDIANTES);

    if (numEstudiantes == -1) {
        numEstudiantes = 0;
        cout << "No hay estudiantes guardados todavia (" << RUTA_ESTUDIANTES << " no existe).\n";
    } else {
        cout << "Estudiantes cargados: " << numEstudiantes << ".\n";
    }
    pausar();

    int opcion;

    do {
        limpiarPantalla();
        mostrarMenu();

        // leerEntero vuelve a preguntar si se escriben letras o un numero
        // fuera de 0-11, asi que aqui opcion siempre es valida.
        opcion = leerEntero("Seleccione una opcion: ", 0, 11);

        switch (opcion) {
            case 1:  registrarEstudiante(estudiantes, numEstudiantes);           break;
            case 2:  cout << "\n[Inscribir curso / notas] En construccion.\n";    break;
            case 3:  consultarEstudiante(estudiantes, numEstudiantes);           break;
            case 4:  listarEstudiantes(estudiantes, numEstudiantes);             break;
            case 5:  cout << "\n[Modificar estudiante] En construccion.\n";       break;
            case 6:  cout << "\n[Eliminar estudiante] En construccion.\n";        break;
            case 7:  cout << "\n[Estadisticas] En construccion.\n";               break;
            case 8:  cout << "\n[Analisis matricial] En construccion.\n";         break;
            case 9:  cout << "\n[Arbol y cursos habilitados] En construccion.\n"; break;
            case 10: guardarInformacion();                                       break;
            case 11: menuPensum();                                               break;
            case 0:
                // Evita perder lo registrado si se olvido la opcion 10
                if (leerEntero("\nGuardar antes de salir? (1 = Si, 0 = No): ", 0, 1) == 1) {
                    guardarInformacion();
                }
                cout << "\nSaliendo del programa. Hasta pronto.\n";
                break;
            default: break;  // no deberia pasar: leerEntero ya valido 0-11
        }

        if (opcion != 0) {
            pausar();
        }

    } while (opcion != 0);

    return 0;
}
