// =====================================================================
// arboles.h - Bosque de arboles binarios de prerrequisitos
// ---------------------------------------------------------------------
// Cada curso tiene hasta un padre y dos hijos. El primero en el archivo
// es el izquierdo y el segundo, el derecho. -1 significa que no hay hijo.
// El pensum se valida antes de recorrerlo; no es un arbol de busqueda.
// =====================================================================
#pragma once

#include "modelos.h"

const int MAX_HIJOS_BINARIOS = 2;

struct HijosBinarios {
    int izquierdo;
    int derecho;
};

// Rechaza mas de dos hijos, ciclos, codigos repetidos o padres inexistentes.
bool validarArbolesBinarios(const Curso cursos[], int numCursos);

// Posicion del padre (prerrequisito) de un curso, o -1 si es raiz.
int padreDe(const Curso cursos[], int numCursos, int curso);

HijosBinarios hijosBinariosDe(const Curso cursos[], int numCursos, int curso);

// Llena hasta dos posiciones de hijos[]: izquierdo y luego derecho.
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
