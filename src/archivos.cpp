// =====================================================================
// archivos.cpp - Guardar y cargar estudiantes
// =====================================================================
#include "archivos.h"
#include "config.h"
#include "validaciones.h"   // recortar, partirLinea, textoAEntero, formatoCarneValido
#include "cursos.h"         // buscarCurso
#include "estudiantes.h"    // inicializarEstudiante, buscarEstudiante
#include "pantalla.h"       // mensajeAviso

#include <iostream>
#include <fstream>   // ofstream (escribir) e ifstream (leer)

using namespace std;

// ---------------------------------------------------------------------
// Recorre los estudiantes. Por cada uno escribe su linea E y luego una
// linea C por cada curso que haya llevado.
// ---------------------------------------------------------------------
bool guardarEstudiantes(const Estudiante estudiantes[], int numEstudiantes,
                        const Curso cursos[], int numCursos,
                        const string& ruta) {
    ofstream archivo(ruta.c_str());   // abre para escribir; borra lo anterior

    if (!archivo.is_open()) {
        return false;
    }

    archivo << "# Estudiantes - Control Academico UMG\n";
    archivo << "# E|carne|nombre|carrera\n";
    archivo << "# C|carne|codigoCurso|estado|parcial1|parcial2|zona|final\n";

    for (int i = 0; i < numEstudiantes; i++) {
        const Estudiante& e = estudiantes[i];

        archivo << "E|" << e.carne << "|" << e.nombre << "|" << e.carrera << "\n";

        for (int c = 0; c < numCursos; c++) {
            if (e.estado[c] != NO_CURSADO) {
                archivo << "C|" << e.carne << "|" << cursos[c].codigo << "|" << e.estado[c];
                for (int k = 0; k < NUM_COMPONENTES; k++) {
                    archivo << "|" << e.puntos[c][k];
                }
                archivo << "\n";
            }
        }
    }

    archivo.close();
    return true;
}

// ---------------------------------------------------------------------
// Lee el archivo linea por linea. Segun la primera letra:
//   E -> agrega un estudiante nuevo al arreglo
//   C -> busca a ese estudiante y le asigna el estado y notas del curso
// Cualquier linea con errores se avisa y se ignora, para que un archivo
// danado no meta datos invalidos al programa.
// ---------------------------------------------------------------------
int cargarEstudiantes(Estudiante estudiantes[],
                      const Curso cursos[], int numCursos,
                      const string& ruta) {
    ifstream archivo(ruta.c_str());

    if (!archivo.is_open()) {
        return -1;
    }

    const int MAX_CAMPOS = 8;   // la linea C es la mas larga: 8 campos
    string campos[MAX_CAMPOS];
    string linea;
    int numEstudiantes = 0;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        linea = recortar(linea);

        if (linea.empty() || linea[0] == '#') {
            continue;
        }

        int n = partirLinea(linea, campos, MAX_CAMPOS);

        // ---------------- Linea de estudiante ----------------
        if (campos[0] == "E") {
            if (n != 4 || !formatoCarneValido(campos[1]) || campos[2].empty()) {
                mensajeAviso("Linea " + to_string(numeroLinea) + " de estudiantes mal escrita, se ignora.");
                continue;
            }
            if (buscarEstudiante(estudiantes, numEstudiantes, campos[1]) != -1) {
                mensajeAviso("Linea " + to_string(numeroLinea) + ": carne " + campos[1]
                             + " repetido, se ignora.");
                continue;
            }
            if (numEstudiantes == MAX_ESTUDIANTES) {
                mensajeAviso("El archivo tiene mas de " + to_string(MAX_ESTUDIANTES)
                             + " estudiantes; se ignoran los demas.");
                break;
            }

            Estudiante e;
            inicializarEstudiante(e);
            e.carne   = campos[1];
            e.nombre  = campos[2];
            e.carrera = campos[3];

            estudiantes[numEstudiantes] = e;
            numEstudiantes++;
        }
        // ---------------- Linea de curso ----------------
        else if (campos[0] == "C") {
            int pos    = buscarEstudiante(estudiantes, numEstudiantes, campos[1]);
            int curso  = buscarCurso(cursos, numCursos, campos[2]);
            int estado = 0;

            bool valida = (n == 8) && pos != -1 && curso != -1
                       && textoAEntero(campos[3], estado)
                       && estado >= NO_CURSADO && estado <= REPROBADO;

            // Cada componente: -1 (sin calificar) o entre 0 y su maximo
            double puntos[NUM_COMPONENTES];
            for (int k = 0; k < NUM_COMPONENTES && valida; k++) {
                valida = textoADecimal(campos[4 + k], puntos[k])
                      && (puntos[k] == SIN_CALIFICAR
                          || (puntos[k] >= 0 && puntos[k] <= PUNTOS_MAXIMOS[k]));
            }

            if (!valida) {
                mensajeAviso("Linea " + to_string(numeroLinea) + " de cursos mal escrita, se ignora.");
                continue;
            }

            estudiantes[pos].estado[curso] = estado;
            for (int k = 0; k < NUM_COMPONENTES; k++) {
                estudiantes[pos].puntos[curso][k] = puntos[k];
            }
        }
        else {
            mensajeAviso("Linea " + to_string(numeroLinea) + " de estudiantes no reconocida, se ignora.");
        }
    }

    archivo.close();
    return numEstudiantes;
}
