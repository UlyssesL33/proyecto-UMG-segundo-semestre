// =====================================================================
// notas.h - Cursos de un estudiante y sus notas
// ---------------------------------------------------------------------
// Pasos 8, 9 y 10 del roadmap:
//   8. Marcar cursos aprobados sin nota (ciclos anteriores)
//   9. Inscribir un curso (queda "en curso")
//  10. Ingresar los puntos netos de P1, P2, Zona y Final
// =====================================================================
#pragma once

#include "modelos.h"

// ---------- Calculos sobre un curso de un estudiante ----------

// true si el curso tiene al menos un componente calificado.
bool tieneNotas(const Estudiante& e, int curso);

// true si los 4 componentes ya estan calificados.
bool notasCompletas(const Estudiante& e, int curso);

// Suma de los componentes calificados (los -1 no cuentan). De 0 a 100.
double notaCurso(const Estudiante& e, int curso);

// Decide el estado segun las notas:
//   faltan componentes -> EN_CURSO
//   los 4 completos    -> APROBADO si nota >= NOTA_MINIMA, si no REPROBADO
void actualizarEstado(Estudiante& e, int curso);

// Suma los creditos de los cursos aprobados del estudiante.
int creditosAprobados(const Estudiante& e, const Curso cursos[], int numCursos);

// Texto del estado: "Aprobado", "En curso", ...
const char* textoEstado(int estado);

// ---------- Pantallas ----------

// Muestra los cursos que el estudiante ha llevado, con notas y estado.
void mostrarCursosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos);

// Opcion 2 del menu principal: pide un carne y muestra el submenu de
// cursos y notas para ese estudiante.
void menuCursosYNotas(Estudiante estudiantes[], int numEstudiantes,
                      const Curso cursos[], int numCursos);
