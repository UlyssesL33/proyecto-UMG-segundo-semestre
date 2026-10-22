// =====================================================================
// notas.h - Cursos de un estudiante y sus notas
// ---------------------------------------------------------------------
// Pasos 8, 9, 10 y 11 del roadmap:
//   8. Marcar cursos aprobados sin nota (ciclos anteriores)
//   9. Inscribir un curso (queda "en curso")
//  10. Ingresar los puntos netos de P1, P2, Zona y Final
//  11. Promedio del estudiante entre sus cursos
// =====================================================================
#pragma once

#include <string>
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

// Promedio del estudiante ENTRE SUS CURSOS: promedio de la nota final de
// cada curso que ya tiene sus 4 componentes calificados. Los cursos en
// curso (incompletos) y los aprobados sin notas no entran.
// cursosConNota devuelve cuantos cursos se promediaron (0 = sin promedio).
double promedioEstudiante(const Estudiante& e, int numCursos, int& cursosConNota);

// El promedio como texto con color: verde si alcanza NOTA_MINIMA, rojo
// si no, y "-" gris si no tiene cursos con nota.
std::string promedioConColor(double promedio, int cursosConNota, int ancho);

// Cuenta cuantos cursos del estudiante estan en ese estado.
int contarCursosEnEstado(const Estudiante& e, int numCursos, int estado);

// Puntos como texto: -1 -> "-", 12.5 -> "12.5", 10 -> "10"
std::string textoPuntos(double valor);

// Texto del estado: "Aprobado", "En curso", ...
const char* textoEstado(int estado);

// El texto del estado con su color: verde aprobado, rojo reprobado,
// amarillo en curso. "ancho" rellena con espacios (0 = sin relleno).
std::string estadoConColor(int estado, int ancho);

// ---------- Pantallas ----------

// Muestra los cursos que el estudiante ha llevado, con notas y estado.
void mostrarCursosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos);

// Opcion 2 del menu principal: pide un carne y muestra el submenu de
// cursos y notas para ese estudiante.
void menuCursosYNotas(Estudiante estudiantes[], int numEstudiantes,
                      const Curso cursos[], int numCursos);
