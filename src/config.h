// =====================================================================
// config.h - Constantes globales del programa
// ---------------------------------------------------------------------
// Todos los numeros "fijos" del programa viven aqui. Si el catedratico
// pide cambiar la nota de aprobacion de 61 a 65, se cambia SOLO aqui.
// =====================================================================
#pragma once

#include <string>

// ---------- Capacidad de los arreglos ----------
const int MAX_ESTUDIANTES = 100;  // estudiantes que caben en el arreglo
const int MAX_CURSOS      = 50;   // cursos del pensum de Sistemas

// ---------- Aprobacion ----------
const double NOTA_MINIMA = 61;    // nota minima para aprobar un curso (0-100)

// ---------- Componentes de la nota de un curso ----------
// Cada curso tiene 4 componentes; se ingresan los puntos netos obtenidos.
// La suma de los maximos es 100.
const int NUM_COMPONENTES = 4;

// Posicion de cada componente dentro del arreglo de puntos
const int PARCIAL_1 = 0;
const int PARCIAL_2 = 1;
const int ZONA      = 2;
const int FINAL     = 3;

// Punteo maximo de cada componente, en el mismo orden de arriba
const double PUNTOS_MAXIMOS[NUM_COMPONENTES] = {10, 20, 35, 35};

// Nombre de cada componente, para mostrarlo en pantalla
const std::string NOMBRES_COMPONENTES[NUM_COMPONENTES] = {
    "Parcial 1", "Parcial 2", "Zona", "Final"
};

// Valor que indica "este componente todavia no se ha calificado"
const double SIN_CALIFICAR = -1;

// ---------- Estado de un estudiante en un curso ----------
const int NO_CURSADO = 0;  // nunca lo ha llevado
const int EN_CURSO   = 1;  // inscrito este semestre
const int APROBADO   = 2;  // aprobado (con notas, o marcado sin notas)
const int REPROBADO  = 3;  // lo llevo y no alcanzo la nota minima

// ---------- Carrera ----------
// El carne tiene la forma 0900-AA-NNNN; los primeros 4 digitos son la carrera.
const std::string CODIGO_CARRERA = "0900";
const std::string NOMBRE_CARRERA = "Ingenieria en Sistemas";

// Codigo que en pensum.txt significa "no tiene curso prerrequisito"
const std::string SIN_PRERREQUISITO = "000000";

// ---------- Pantalla ----------
// Si una consola muestra simbolos raros como "[32m" en vez de colores,
// cambiar a false para apagar todos los colores del programa.
const bool USAR_COLORES = true;

// ---------- Archivos ----------
// Rutas relativas: el programa se ejecuta desde la raiz del repositorio.
const std::string RUTA_PENSUM      = "datos/pensum.txt";
const std::string RUTA_ESTUDIANTES = "datos/estudiantes.txt";
