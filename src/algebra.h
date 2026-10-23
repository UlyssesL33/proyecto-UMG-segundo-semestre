// =====================================================================
// algebra.h - Analisis matricial (Algebra Lineal, pasos 19 y 20)
// ---------------------------------------------------------------------
// Para un ciclo del pensum se arman dos matrices de m filas (estudiantes)
// por n columnas (cursos del ciclo):
//
//   P[i][j] = nota del estudiante i en el curso j   (0 si no la tiene)
//   M[i][j] = 1 si el estudiante i tiene esa nota, 0 si no
//
// M es necesaria para no confundir "no tiene nota" con "saco 0": al
// dividir se usa M para contar solo las notas que existen.
//
// Paso 19:  transpuesta P^T  y promedio de cada curso:
//           suma de notas    s = P^T * 1      (1 = vector de unos)
//           cantidad notas   k = M^T * 1
//           promedio curso j = s[j] / k[j]
//
// Paso 20:  promedio ponderado por creditos de cada estudiante:
//           c = vector de creditos de los cursos del ciclo
//           ponderado[i] = (P * c)[i] / (M * c)[i]
//           y la nota necesaria en un curso:  acumulado + x = NOTA_MINIMA
// =====================================================================
#pragma once

#include "modelos.h"
#include "config.h"

// Tamanos maximos de las matrices: filas = estudiantes, columnas = cursos
// de un ciclo (en el pensum ningun ciclo tiene mas de 5).
const int MAX_FILAS    = MAX_ESTUDIANTES;
const int MAX_COLUMNAS = 10;

// ---------- Operaciones con matrices ----------

// AT = transpuesta de A.  A es m x n,  AT es n x m:  AT[j][i] = A[i][j]
void transpuesta(const double A[][MAX_COLUMNAS], int m, int n, double AT[][MAX_FILAS]);

// r = A * v   (A es m x n, v tiene n elementos, r tiene m elementos)
void matrizPorVector(const double A[][MAX_COLUMNAS], int m, int n,
                     const double v[], double r[]);

// r = AT * v  (AT es n x m, v tiene m elementos, r tiene n elementos)
// Es la misma operacion; se repite porque en C++ la segunda dimension de
// un arreglo 2D debe ser fija, y la transpuesta tiene otra forma.
void transpuestaPorVector(const double AT[][MAX_FILAS], int n, int m,
                          const double v[], double r[]);

// ---------- Opcion 8 del menu ----------

void menuAlgebra(const Estudiante estudiantes[], int numEstudiantes,
                 const Curso cursos[], int numCursos);
