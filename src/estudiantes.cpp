// estudiantes.cpp - Registro y manejo de estudiantes
#include "estudiantes.h"
#include "config.h"
#include "validaciones.h"
#include "pantalla.h"
#include "notas.h"
#include "cursos.h"

#include <iostream>

using namespace std;

// Un estudiante nuevo empieza sin cursos ni notas.
void inicializarEstudiante(Estudiante& e) {
    e.carne = "";
    e.nombre = "";
    e.carrera = "";

    for (int c = 0; c < MAX_CURSOS; c++) {
        reiniciarCurso(e, c, NO_CURSADO);
    }
}

// Busqueda secuencial por carne (igual que buscarCurso por codigo).
int buscarEstudiante(const Estudiante estudiantes[], int numEstudiantes,
                     const string& carne) {
    for (int i = 0; i < numEstudiantes; i++) {
        if (estudiantes[i].carne == carne) {
            return i;
        }
    }
    return -1;
}

// El carne debe ser unico y pertenecer a la carrera configurada.
bool registrarEstudiante(Estudiante estudiantes[], int& numEstudiantes) {
    if (numEstudiantes >= MAX_ESTUDIANTES) {
        mensajeError("No hay espacio: ya hay " + to_string(MAX_ESTUDIANTES) + " estudiantes registrados.");
        return false;
    }

    string carne = leerCarne("  Carne (ej. 0900-26-9368) | Cancelar = 0:  ");

        // Si ingresa 0, regresa al menu sin registrar.
    if (carne == "0") {
        mensajeAviso("Registro cancelado.");
        return false;
    }
    

    if (buscarEstudiante(estudiantes, numEstudiantes, carne) != -1) {
        mensajeError("El carne " + carne + " ya esta registrado.");
        return false;
    }

    Estudiante nuevo;
    inicializarEstudiante(nuevo);
    nuevo.carne   = carne;
    nuevo.nombre  = leerTexto("  Nombre completo: ");
    nuevo.carrera = carne.substr(0, 4);

    estudiantes[numEstudiantes] = nuevo;
    numEstudiantes++;

    cout << "\n";
    mensajeExito("Estudiante registrado (" + to_string(numEstudiantes) + " en total).");
    mostrarEstudiante(nuevo);
    return true;
}

// Datos basicos de un estudiante.
void mostrarEstudiante(const Estudiante& e) {
    cout << "\n" << pintar("  Carne:   ", GRIS) << e.carne << "\n";
    cout << pintar("  Nombre:  ", GRIS) << e.nombre << "\n";
    cout << pintar("  Carrera: ", GRIS) << e.carrera << " - " << NOMBRE_CARRERA << "\n";
}

// Resumen academico: promedio, cursos por estado y creditos.
static void mostrarResumen(const Estudiante& e, const Curso cursos[], int numCursos) {
    int conNota = 0;
    double promedio = promedioEstudiante(e, numCursos, conNota);

    subtitulo("Resumen");
    cout << pintar("  Promedio:           ", GRIS) << promedioConColor(promedio, conNota, 0)
         << pintar("  (" + to_string(conNota) + " curso(s) con nota)", GRIS) << "\n";
    cout << pintar("  Cursos aprobados:   ", GRIS) << contarCursosEnEstado(e, numCursos, APROBADO)
         << pintar("    Reprobados: ", GRIS) << contarCursosEnEstado(e, numCursos, REPROBADO)
         << pintar("    En curso: ", GRIS) << contarCursosEnEstado(e, numCursos, EN_CURSO) << "\n";
    cout << pintar("  Creditos aprobados: ", GRIS) << creditosAprobados(e, cursos, numCursos)
         << " de " << totalCreditosPensum(cursos, numCursos) << "\n";
}

void consultarEstudiante(const Estudiante estudiantes[], int numEstudiantes,
                         const Curso cursos[], int numCursos) {
    if (numEstudiantes == 0) {
        mensajeAviso("No hay estudiantes registrados.");
        return;
    }

    string carne = leerCarne("  Carne a buscar: ");
    int pos = buscarEstudiante(estudiantes, numEstudiantes, carne);

    if (pos == -1) {
        mensajeError("No existe un estudiante con carne " + carne + ".");
        return;
    }

    const Estudiante& e = estudiantes[pos];
    mostrarEstudiante(e);
    mostrarResumen(e, cursos, numCursos);
    mostrarCursosEstudiante(e, cursos, numCursos);
}

// La situacion global depende del promedio, no de aprobar todos los cursos.
void listarEstudiantes(const Estudiante estudiantes[], int numEstudiantes, int numCursos) {
    if (numEstudiantes == 0) {
        mensajeAviso("No hay estudiantes registrados.");
        return;
    }

    cout << "\n" << pintar("  " + columna("No.", 5) + columna("Carne", 16) + columna("Nombre", 32)
                          + columna("Promedio", 10) + "Situacion", GRIS) << "\n";

    int aprobados = 0, reprobados = 0, sinNotas = 0;

    for (int i = 0; i < numEstudiantes; i++) {
        int conNota = 0;
        double promedio = promedioEstudiante(estudiantes[i], numCursos, conNota);

        string situacion;
        if (conNota == 0) {
            situacion = pintar("Sin notas", GRIS);
            sinNotas++;
        } else if (promedio >= NOTA_MINIMA) {
            situacion = pintar("Aprobado", VERDE);
            aprobados++;
        } else {
            situacion = pintar("Reprobado", ROJO);
            reprobados++;
        }

        cout << "  " << columna(to_string(i + 1), 5)
             << columna(estudiantes[i].carne, 16)
             << columna(estudiantes[i].nombre, 32)
             << promedioConColor(promedio, conNota, 10)
             << situacion << "\n";
    }

    cout << "\n" << pintar("  Total: ", GRIS) << numEstudiantes
         << pintar("    Aprobados: ", GRIS) << aprobados
         << pintar("    Reprobados: ", GRIS) << reprobados
         << pintar("    Sin notas: ", GRIS) << sinNotas << "\n";
    cout << pintar("  Situacion segun el promedio entre cursos (minimo "
                   + textoDecimal(NOTA_MINIMA, 0) + ").", GRIS) << "\n";
}

// modificar. Las notas se corrigen en la opcion 2; aqui se
// cambian los datos personales.
void modificarEstudiante(Estudiante estudiantes[], int numEstudiantes) {
    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }

    Estudiante& e = estudiantes[pos];   // referencia: se modifica el original
    mostrarEstudiante(e);

    subtitulo("Que desea modificar?");
    opcionMenu(1, "Nombre");
    opcionMenu(2, "Carne");
    opcionMenu(0, "Cancelar");
    cout << pintar("  (Las notas se corrigen en la opcion 2 del menu principal.)", GRIS) << "\n";
    int opcion = leerEntero("\n  Seleccione: ", 0, 2);

    if (opcion == 1) {
        e.nombre = leerTexto("  Nuevo nombre: ");
        mensajeExito("Nombre actualizado.");
    } else if (opcion == 2) {
        string nuevo = leerCarne("  Nuevo carne: ");
        int otro = buscarEstudiante(estudiantes, numEstudiantes, nuevo);

        // El carne debe seguir siendo unico (puede repetirse el suyo propio)
        if (otro != -1 && otro != pos) {
            mensajeError("El carne " + nuevo + " ya pertenece a " + estudiantes[otro].nombre + ".");
            return;
        }
        e.carne   = nuevo;
        e.carrera = nuevo.substr(0, 4);
        mensajeExito("Carne actualizado.");
    } else {
        return;
    }

    mostrarEstudiante(e);
}

// eliminar.
// Para no dejar un "hueco" en el arreglo, cada estudiante que esta
// despues del eliminado se recorre una posicion hacia atras:
//
//   antes:  [Ana] [Luis] [Eva] [Juan]     eliminar Luis (posicion 1)
//   despues:[Ana] [Eva] [Juan]            numEstudiantes: 4 -> 3
void eliminarEstudiante(Estudiante estudiantes[], int& numEstudiantes,
                        const Curso cursos[], int numCursos) {
    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }

    mostrarEstudiante(estudiantes[pos]);
    mostrarResumen(estudiantes[pos], cursos, numCursos);

    cout << "\n";
    mensajeAviso("Se eliminaran el estudiante y todas sus notas.");
    if (leerEntero("  Confirma la eliminacion? (1 = Si, 0 = No): ", 0, 1) == 0) {
        mensajeAviso("Eliminacion cancelada.");
        return;
    }

    string nombre = estudiantes[pos].nombre;

    for (int i = pos; i < numEstudiantes - 1; i++) {
        estudiantes[i] = estudiantes[i + 1];
    }
    numEstudiantes--;

    mensajeExito(nombre + " eliminado. Quedan " + to_string(numEstudiantes) + " estudiante(s).");
}

// Como en la lista, la posicion i del arreglo se muestra como i + 1,
// asi que al elegir el numero n se devuelve la posicion n - 1.
int elegirEstudiante(const Estudiante estudiantes[], int numEstudiantes) {
    if (numEstudiantes == 0) {
        mensajeAviso("No hay estudiantes registrados.");
        return -1;
    }

    subtitulo("Elija un estudiante:");
    for (int i = 0; i < numEstudiantes; i++) {
        opcionMenu(i + 1, columna(estudiantes[i].carne, 16) + estudiantes[i].nombre);
    }
    opcionMenu(0, "Cancelar");

    int eleccion = leerEntero("\n  Seleccione: ", 0, numEstudiantes);
    if (eleccion == 0) {
        return -1;
    }
    return eleccion - 1;
}
