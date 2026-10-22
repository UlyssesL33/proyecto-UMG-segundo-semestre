// =====================================================================
// modelos.h - Estructuras de datos (struct) del programa
// =====================================================================
#pragma once

#include <string>
#include "config.h"

// ---------------------------------------------------------------------
// Curso: una linea de datos/pensum.txt
//   codigo|nombre|creditos|ciclo|prerrequisito|creditos_minimos
// ---------------------------------------------------------------------
struct Curso {
    std::string codigo;        // "090007" -> string para no perder el 0 inicial
    std::string nombre;        // "Algebra Lineal"
    int         creditos;      // 4, 5 o 6
    int         ciclo;         // semestre del pensum: 1 a 10
    std::string prerrequisito; // codigo del curso requerido, o SIN_PRERREQUISITO
    int         creditosMinimos; // creditos aprobados requeridos (0 = no aplica)
};

// ---------------------------------------------------------------------
// Estudiante
//
// Los arreglos estado[] y puntos[][] van "en paralelo" con el arreglo
// de cursos del pensum: la posicion c se refiere al curso cursos[c].
//
//   estado[c]               -> NO_CURSADO, EN_CURSO, APROBADO o REPROBADO
//   puntos[c][PARCIAL_1]    -> puntos netos del parcial 1 en ese curso
//   puntos[c][FINAL]        -> ... o SIN_CALIFICAR si aun no hay nota
//
// Ejemplo: si cursos[7] es Algoritmos, estado[7] dice como le fue al
// estudiante en Algoritmos y puntos[7] guarda sus 4 componentes.
//
// El promedio NO se guarda: se calcula cada vez a partir de puntos[][],
// asi nunca queda desactualizado si se modifica una nota.
// ---------------------------------------------------------------------
struct Estudiante {
    std::string carne;    // "0900-26-1234"
    std::string nombre;
    std::string carrera;  // se toma de los primeros 4 digitos del carne
    int    estado[MAX_CURSOS];
    double puntos[MAX_CURSOS][NUM_COMPONENTES];
};
