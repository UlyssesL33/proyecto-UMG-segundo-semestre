// archivos.h - Guardar y cargar estudiantes en datos/estudiantes.txt
// Formato del archivo (texto plano, separado por '|'):
//
//   E|carne|nombre|carrera
//   C|carne|codigoCurso|estado|parcial1|parcial2|zona|final
//
// Una linea E por estudiante. Debajo, una linea C por cada curso que ese
// estudiante haya llevado (estado distinto de NO_CURSADO). Un componente
// sin calificar se guarda como -1. Las lineas que empiezan con '#' son
// comentarios.
//
// Ejemplo:
//   E|0900-26-9368|Ulises Lopez|0900
//   C|0900-26-9368|090008|2|10|15|30|30
#pragma once

#include <string>
#include "modelos.h"

// Escribe todos los estudiantes en el archivo (lo reemplaza completo).
// Devuelve true si se pudo guardar.
bool guardarEstudiantes(const Estudiante estudiantes[], int numEstudiantes,
                        const Curso cursos[], int numCursos,
                        const std::string& ruta);

// Lee el archivo y llena el arreglo de estudiantes.
// Devuelve cuantos estudiantes se cargaron, o -1 si el archivo no existe
// (lo normal la primera vez que se usa el programa).
int cargarEstudiantes(Estudiante estudiantes[],
                      const Curso cursos[], int numCursos,
                      const std::string& ruta);
