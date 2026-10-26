// cursos.h - Catalogo de cursos (pensum de Ingenieria en Sistemas)
#pragma once

#include <string>
#include "modelos.h"

// Lee el archivo del pensum y llena el arreglo cursos[].
// Devuelve cuantos cursos se cargaron, o -1 si no se pudo abrir el archivo.
// Devuelve -2 si los prerrequisitos no forman un bosque binario valido.
int cargarPensum(Curso cursos[], const std::string& ruta);

// Busca un curso por su codigo ("090008").
// Devuelve su posicion en el arreglo, o -1 si no existe.
int buscarCurso(const Curso cursos[], int numCursos, const std::string& codigo);

// Suma los creditos de todos los cursos del pensum.
int totalCreditosPensum(const Curso cursos[], int numCursos);

// Muestra el pensum agrupado por ciclo, con su prerrequisito.
void listarPensum(const Curso cursos[], int numCursos);

// Muestra los datos de un solo curso.
void mostrarCurso(const Curso cursos[], int numCursos, int posicion);

// Devuelve el numero del ciclo mas alto del pensum (10 en Sistemas).
int ultimoCiclo(const Curso cursos[], int numCursos);

// Muestra una lista numerada de cursos y deja elegir uno por su numero.
//   opciones[]  -> posiciones (en el arreglo cursos) de los cursos a mostrar
//   cantidad    -> cuantos cursos hay en opciones[]
// Devuelve la posicion del curso elegido en el arreglo cursos,
// o -1 si el usuario cancela (0) o si la lista esta vacia.
int elegirCurso(const Curso cursos[], const int opciones[], int cantidad,
                const std::string& titulo);
