// =====================================================================
// cursos.h - Catalogo de cursos (pensum de Ingenieria en Sistemas)
// =====================================================================
#pragma once

#include <string>
#include "modelos.h"

// Lee el archivo del pensum y llena el arreglo cursos[].
// Devuelve cuantos cursos se cargaron, o -1 si no se pudo abrir el archivo.
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
