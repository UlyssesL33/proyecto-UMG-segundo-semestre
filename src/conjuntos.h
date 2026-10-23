// =====================================================================
// conjuntos.h - Conjuntos de cursos (Matematica Discreta, paso 15)
// ---------------------------------------------------------------------
// Un conjunto de cursos se guarda como un arreglo de bool del tamano del
// pensum (su "vector caracteristico"):
//
//     A[c] == true   significa   el curso c PERTENECE al conjunto A
//
// Con esa idea, cada operacion de conjuntos es una operacion logica
// aplicada posicion por posicion:
//
//     union         A u B   ->  A[c] || B[c]     (esta en A  O  en B)
//     interseccion  A n B   ->  A[c] && B[c]     (esta en A  Y  en B)
//     diferencia    A - B   ->  A[c] && !B[c]    (esta en A  y NO en B)
//     subconjunto   A <= B  ->  para todo c: si A[c] entonces B[c]
//
// Los conjuntos que usa el programa (S es el universo):
//     S = Pensum (los 50 cursos)    A = Aprobados    C = En curso
//     R = Reprobados                P = Pendientes = S - A
// =====================================================================
#pragma once

#include <string>
#include "modelos.h"

// ---------- Construir conjuntos ----------

// S: todos los cursos del pensum (el conjunto universo).
void conjuntoPensum(bool S[], int numCursos);

// Cursos del estudiante que estan en un estado (APROBADO, EN_CURSO, ...).
void conjuntoPorEstado(const Estudiante& e, int numCursos, int estado, bool resultado[]);

// ---------- Operaciones ----------

void unionConjuntos(const bool A[], const bool B[], bool resultado[], int numCursos);
void interseccion(const bool A[], const bool B[], bool resultado[], int numCursos);
void diferencia(const bool A[], const bool B[], bool resultado[], int numCursos);
bool esSubconjunto(const bool A[], const bool B[], int numCursos);
int  cardinalidad(const bool A[], int numCursos);   // |A| = cuantos elementos tiene

// Suma de creditos de los cursos del conjunto.
int creditosDe(const bool A[], const Curso cursos[], int numCursos);

// ---------- Mostrar ----------

// Imprime  Nombre = { 090001, 090002, ... }   |Nombre| = n
void mostrarConjunto(const std::string& nombre, const bool A[],
                     const Curso cursos[], int numCursos);

// Imprime los cursos del conjunto, uno por linea, con su nombre.
void mostrarConjuntoDetallado(const bool A[], const Curso cursos[], int numCursos);

// ---------- Pantallas (opcion 9 del menu) ----------

// Los conjuntos de un estudiante y las operaciones entre ellos.
void pantallaConjuntosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos);

// Compara dos estudiantes: cursos aprobados en comun, solo de uno, etc.
void pantallaCompararEstudiantes(const Estudiante& e1, const Estudiante& e2,
                                 const Curso cursos[], int numCursos);
