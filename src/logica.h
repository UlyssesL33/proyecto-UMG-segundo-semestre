// =====================================================================
// logica.h - Regla de habilitacion como proposicion logica (paso 16)
// ---------------------------------------------------------------------
// Un curso c esta HABILITADO para un estudiante si se cumplen las cuatro
// proposiciones a la vez (conjuncion):
//
//   p : c no esta aprobado              (c no pertenece a A)
//   q : c no esta en curso              (c no pertenece a C)
//   r : su prerrequisito esta aprobado  (o no tiene prerrequisito)
//   s : creditos aprobados >= creditos minimos del curso
//
//   habilitado(c) = p AND q AND r AND s
//
// Basta que una sea falsa para que el curso no este habilitado.
// =====================================================================
#pragma once

#include "modelos.h"

// Valor de cada proposicion para un curso y un estudiante.
struct Habilitacion {
    bool p;          // no aprobado
    bool q;          // no en curso
    bool r;          // prerrequisito aprobado (o no tiene)
    bool s;          // creditos suficientes
    bool resultado;  // p && q && r && s
};

Habilitacion evaluarHabilitacion(const Estudiante& e, const Curso cursos[], int numCursos,
                                 int curso);

// Atajo: solo el resultado final.
bool estaHabilitado(const Estudiante& e, const Curso cursos[], int numCursos, int curso);

// Llena H[] con el conjunto de cursos habilitados (H[c] = true).
void conjuntoHabilitados(const Estudiante& e, const Curso cursos[], int numCursos, bool H[]);

// Pantalla (opcion 9): tabla de verdad de la regla para cada curso
// pendiente y el conjunto H de habilitados.
void pantallaHabilitados(const Estudiante& e, const Curso cursos[], int numCursos);
