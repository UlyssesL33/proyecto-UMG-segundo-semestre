// =====================================================================
// estudiantes.cpp - Registro y manejo de estudiantes
// =====================================================================
#include "estudiantes.h"
#include "config.h"
#include "validaciones.h"

#include <iostream>
#include <iomanip>   // setw: alinear columnas

using namespace std;

// ---------------------------------------------------------------------
// Recorre todos los cursos posibles y los deja sin cursar y sin notas.
// Es importante: un struct nuevo trae "basura" en sus arreglos hasta
// que se le asigna un valor.
// ---------------------------------------------------------------------
void inicializarEstudiante(Estudiante& e) {
    e.carne = "";
    e.nombre = "";
    e.carrera = "";

    for (int c = 0; c < MAX_CURSOS; c++) {
        e.estado[c] = NO_CURSADO;
        for (int k = 0; k < NUM_COMPONENTES; k++) {
            e.puntos[c][k] = SIN_CALIFICAR;
        }
    }
}

// ---------------------------------------------------------------------
// Busqueda secuencial por carne (igual que buscarCurso por codigo).
// ---------------------------------------------------------------------
int buscarEstudiante(const Estudiante estudiantes[], int numEstudiantes,
                     const string& carne) {
    for (int i = 0; i < numEstudiantes; i++) {
        if (estudiantes[i].carne == carne) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------
// Pasos:
//   1. Revisar que quede espacio en el arreglo.
//   2. Pedir el carne (leerCarne ya valida formato y carrera 0900).
//   3. Revisar que el carne no este registrado.
//   4. Pedir el nombre.
//   5. La carrera se toma de los primeros 4 digitos del carne.
//   6. Guardar en la siguiente posicion libre y aumentar el contador.
// ---------------------------------------------------------------------
bool registrarEstudiante(Estudiante estudiantes[], int& numEstudiantes) {
    cout << "\n--- Registrar estudiante ---\n";

    if (numEstudiantes >= MAX_ESTUDIANTES) {
        cout << "  No hay espacio: ya hay " << MAX_ESTUDIANTES << " estudiantes registrados.\n";
        return false;
    }

    string carne = leerCarne("Carne (ej. 0900-26-9368): ");

    if (buscarEstudiante(estudiantes, numEstudiantes, carne) != -1) {
        cout << "  Error: el carne " << carne << " ya esta registrado.\n";
        return false;
    }

    Estudiante nuevo;
    inicializarEstudiante(nuevo);
    nuevo.carne   = carne;
    nuevo.nombre  = leerTexto("Nombre completo: ");
    nuevo.carrera = carne.substr(0, 4);

    // La siguiente posicion libre es justamente numEstudiantes:
    // si hay 3 estudiantes, ocupan 0, 1 y 2, y el nuevo va en 3.
    estudiantes[numEstudiantes] = nuevo;
    numEstudiantes++;

    cout << "\n  Estudiante registrado:\n";
    cout << "    Carne:   " << nuevo.carne << "\n";
    cout << "    Nombre:  " << nuevo.nombre << "\n";
    cout << "    Carrera: " << nuevo.carrera << " - " << NOMBRE_CARRERA << "\n";
    cout << "  Total de estudiantes: " << numEstudiantes << "\n";
    return true;
}

// ---------------------------------------------------------------------
// Ficha de un estudiante. En el paso 11 se le agregan sus cursos,
// notas y promedio.
// ---------------------------------------------------------------------
void mostrarEstudiante(const Estudiante& e) {
    cout << "\n  Carne:   " << e.carne;
    cout << "\n  Nombre:  " << e.nombre;
    cout << "\n  Carrera: " << e.carrera << " - " << NOMBRE_CARRERA << "\n";
}

// ---------------------------------------------------------------------
// Pasos:
//   1. Si no hay estudiantes, no tiene sentido preguntar.
//   2. Pedir el carne y buscarlo.
//   3. Mostrar la ficha, o avisar que no existe.
// ---------------------------------------------------------------------
void consultarEstudiante(const Estudiante estudiantes[], int numEstudiantes) {
    cout << "\n--- Consultar estudiante ---\n";

    if (numEstudiantes == 0) {
        cout << "  No hay estudiantes registrados.\n";
        return;
    }

    string carne = leerCarne("Carne a buscar: ");
    int pos = buscarEstudiante(estudiantes, numEstudiantes, carne);

    if (pos == -1) {
        cout << "  No existe un estudiante con carne " << carne << ".\n";
    } else {
        mostrarEstudiante(estudiantes[pos]);
    }
}

// ---------------------------------------------------------------------
// Recorre el arreglo de 0 a numEstudiantes-1 y muestra una fila por
// estudiante. Solo se recorren las posiciones ocupadas, nunca las 100.
// ---------------------------------------------------------------------
void listarEstudiantes(const Estudiante estudiantes[], int numEstudiantes) {
    cout << "\n--- Listado de estudiantes ---\n";

    if (numEstudiantes == 0) {
        cout << "  No hay estudiantes registrados.\n";
        return;
    }

    cout << left << setw(5) << "No." << setw(16) << "Carne"
         << setw(36) << "Nombre" << "Carrera\n";

    for (int i = 0; i < numEstudiantes; i++) {
        cout << left << setw(5) << (i + 1)
             << setw(16) << estudiantes[i].carne
             << setw(36) << estudiantes[i].nombre
             << estudiantes[i].carrera << "\n";
    }

    cout << "\nTotal: " << numEstudiantes << " estudiante(s).\n";
}

// ---------------------------------------------------------------------
// Como en la lista, la posicion i del arreglo se muestra como i + 1,
// asi que al elegir el numero n se devuelve la posicion n - 1.
// ---------------------------------------------------------------------
int elegirEstudiante(const Estudiante estudiantes[], int numEstudiantes) {
    if (numEstudiantes == 0) {
        cout << "  No hay estudiantes registrados.\n";
        return -1;
    }

    cout << "\n  Estudiantes:\n";
    for (int i = 0; i < numEstudiantes; i++) {
        cout << "  " << right << setw(2) << (i + 1) << ". " << left
             << setw(16) << estudiantes[i].carne << estudiantes[i].nombre << "\n";
    }
    cout << "   0. Cancelar\n";

    int eleccion = leerEntero("  Seleccione el estudiante: ", 0, numEstudiantes);
    if (eleccion == 0) {
        return -1;
    }
    return eleccion - 1;
}
