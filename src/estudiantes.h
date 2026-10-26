// estudiantes.h - Registro y manejo de estudiantes
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

// Opcion 3: pide un carne y muestra la ficha completa del estudiante:
// datos, resumen (promedio, aprobados, creditos) y tabla de cursos.
void consultarEstudiante(const Estudiante estudiantes[], int numEstudiantes,
                         const Curso cursos[], int numCursos);

// Opcion 4 (listado global): todos los estudiantes con su promedio.
void listarEstudiantes(const Estudiante estudiantes[], int numEstudiantes, int numCursos);

// Opcion 5: cambiar el nombre o el carne de un estudiante.
void modificarEstudiante(Estudiante estudiantes[], int numEstudiantes);

// Opcion 6: elimina un estudiante y recorre el arreglo para no dejar
// huecos. numEstudiantes se pasa por referencia porque disminuye.
void eliminarEstudiante(Estudiante estudiantes[], int& numEstudiantes,
                        const Curso cursos[], int numCursos);

// Muestra la lista numerada de estudiantes y deja elegir uno por numero.
// Devuelve su posicion en el arreglo, o -1 si cancela o no hay estudiantes.
int elegirEstudiante(const Estudiante estudiantes[], int numEstudiantes);
