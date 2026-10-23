// =====================================================================
// arboles.h - El pensum como bosque de arboles (pasos 17 y 18)
// ---------------------------------------------------------------------
// Cada curso tiene A LO SUMO un curso prerrequisito. Si vemos el
// prerrequisito como el "padre" de un curso, el pensum queda como un
// BOSQUE: varios arboles separados.
//
//   Algoritmos                      <- raiz (no tiene padre)
//   `-- Programacion I              <- hijo de Algoritmos
//       `-- Programacion II
//           `-- ...
//
// No hace falta guardar el arbol aparte: el padre de cada curso ya esta
// en cursos[i].prerrequisito, y los hijos se encuentran buscando que
// cursos tienen a ese curso como prerrequisito.
// =====================================================================
#pragma once

#include "modelos.h"

// Posicion del padre (prerrequisito) de un curso, o -1 si es raiz.
int padreDe(const Curso cursos[], int numCursos, int curso);

// Llena hijos[] con las posiciones de los cursos que tienen a "curso"
// como prerrequisito. Devuelve cuantos hijos tiene.
int hijosDe(const Curso cursos[], int numCursos, int curso, int hijos[]);

// Cantidad de descendientes (hijos, nietos, ...) de un curso. Recursiva.
int contarDescendientes(const Curso cursos[], int numCursos, int curso);

// Altura: cuantos cursos hay en la cadena mas larga que empieza en
// "curso" (el mismo cuenta). Recursiva:  altura = 1 + max(altura hijos)
int altura(const Curso cursos[], int numCursos, int curso);

// ---------- Pantallas (opcion 9 del menu) ----------

// Paso 17: el pensum dibujado como arbol, marcando cada curso segun el
// estudiante: [A] aprobado, [C] en curso, [R] reprobado, [H] habilitado,
// [B] bloqueado.
void pantallaArbolEstudiante(const Estudiante& e, const Curso cursos[], int numCursos);

// Paso 18: elegir un curso y ver todo lo que bloquea reprobarlo (su subarbol).
void pantallaBloqueos(const Curso cursos[], int numCursos);

// Paso 18: altura de cada arbol y la cadena de prerrequisitos mas larga.
void pantallaProfundidad(const Curso cursos[], int numCursos);
