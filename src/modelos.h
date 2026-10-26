// modelos.h - Estructuras de datos (struct) del programa
#pragma once

#include <string>
#include "config.h"

// Curso: una linea de datos/pensum.txt
//   codigo|nombre|creditos|ciclo|prerrequisito|creditos_minimos
struct Curso {
    std::string codigo;        // "090007" -> string para no perder el 0 inicial
    std::string nombre;        // "Algebra Lineal"
    int         creditos;      // 4, 5 o 6
    int         ciclo;         // semestre del pensum: 1 a 10
    std::string prerrequisito; // codigo del curso requerido, o SIN_PRERREQUISITO
    int         creditosMinimos; // creditos aprobados requeridos (0 = no aplica)
};

// El indice c de estado[c] y puntos[c] corresponde a cursos[c] del pensum.
// puntos[c][k] guarda un componente; -1 significa que aun no se califico.
// El promedio se calcula cuando se necesita.
struct Estudiante {
    std::string carne;    // "0900-26-1234"
    std::string nombre;
    std::string carrera;  // se toma de los primeros 4 digitos del carne
    int    estado[MAX_CURSOS];
    double puntos[MAX_CURSOS][NUM_COMPONENTES];
};
