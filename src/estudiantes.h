// =====================================================================
// estudiantes.h - Registro y manejo de estudiantes
// =====================================================================
#pragma once

#include <string>
#include "modelos.h"

// Deja un estudiante "en blanco": ningun curso cursado y sin notas.
void inicializarEstudiante(Estudiante& e);

// Busca un estudiante por carne.
// Devuelve su posicion en el arreglo, o -1 si no existe.
int buscarEstudiante(const Estudiante estudiantes[], int numEstudiantes,
                     const std::string& carne);

// Pide los datos de un estudiante nuevo y lo agrega al final del arreglo.
// numEstudiantes se pasa por referencia (&) porque la funcion lo aumenta.
// Devuelve true si se registro, false si no (arreglo lleno o carne repetido).
bool registrarEstudiante(Estudiante estudiantes[], int& numEstudiantes);

// Muestra la ficha de un estudiante (carne, nombre, carrera).
void mostrarEstudiante(const Estudiante& e);

// Opcion 3: pide un carne y muestra la ficha de ese estudiante.
void consultarEstudiante(const Estudiante estudiantes[], int numEstudiantes);

// Opcion 4: muestra todos los estudiantes registrados en una tabla.
void listarEstudiantes(const Estudiante estudiantes[], int numEstudiantes);
