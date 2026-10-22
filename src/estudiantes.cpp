// =====================================================================
// estudiantes.cpp - Registro y manejo de estudiantes
// =====================================================================
#include "estudiantes.h"
#include "config.h"
#include "validaciones.h"

#include <iostream>

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
