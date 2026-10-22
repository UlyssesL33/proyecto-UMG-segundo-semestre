// =====================================================================
// estadisticas.h - Reportes y estadisticas (paso 13)
// ---------------------------------------------------------------------
// Dos niveles, como acordo el equipo:
//   - GLOBAL:   por estudiante, segun su promedio entre cursos
//   - POR CURSO: segun la nota de cada estudiante en ese curso
// =====================================================================
#pragma once

#include "modelos.h"

// Opcion 4 (listado por curso): elige un curso y muestra a sus
// estudiantes con sus notas, mas aprobados y reprobados de ese curso.
void listarPorCurso(const Estudiante estudiantes[], int numEstudiantes,
                    const Curso cursos[], int numCursos);

// Opcion 7: promedio general, mayor promedio, aprobados/reprobados
// globales y un resumen de cada curso que tenga estudiantes.
void mostrarEstadisticas(const Estudiante estudiantes[], int numEstudiantes,
                         const Curso cursos[], int numCursos);
