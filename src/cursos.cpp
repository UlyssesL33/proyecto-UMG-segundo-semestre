// =====================================================================
// cursos.cpp - Carga y consulta del pensum
// =====================================================================
#include "cursos.h"
#include "config.h"
#include "validaciones.h"   // recortar, partirLinea, textoAEntero
#include "pantalla.h"

#include <iostream>
#include <fstream>   // ifstream: leer archivos
#include <sstream>   // stringstream: armar el texto del requisito

using namespace std;

// ---------------------------------------------------------------------
// Lee datos/pensum.txt linea por linea.
//   - Las lineas vacias y las que empiezan con '#' se ignoran.
//   - Cada linea valida tiene 6 campos separados por '|'.
//   - Si una linea esta mal escrita se avisa y se salta.
// ---------------------------------------------------------------------
int cargarPensum(Curso cursos[], const string& ruta) {
    ifstream archivo(ruta.c_str());

    if (!archivo.is_open()) {
        return -1;
    }

    const int NUM_CAMPOS = 6;
    string campos[NUM_CAMPOS];
    string linea;
    int numCursos = 0;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;
        linea = recortar(linea);

        if (linea.empty() || linea[0] == '#') {
            continue;  // comentario o linea en blanco
        }

        if (numCursos == MAX_CURSOS) {
            mensajeAviso("El pensum tiene mas de " + to_string(MAX_CURSOS)
                         + " cursos; se ignoran los demas.");
            break;
        }

        int encontrados = partirLinea(linea, campos, NUM_CAMPOS);

        Curso c;
        bool valida = (encontrados == NUM_CAMPOS)
                   && textoAEntero(campos[2], c.creditos)
                   && textoAEntero(campos[3], c.ciclo)
                   && textoAEntero(campos[5], c.creditosMinimos);

        if (!valida) {
            mensajeAviso("Linea " + to_string(numeroLinea) + " del pensum mal escrita, se ignora.");
            continue;
        }

        c.codigo        = campos[0];
        c.nombre        = campos[1];
        c.prerrequisito = campos[4];

        cursos[numCursos] = c;
        numCursos++;
    }

    archivo.close();

    // Revision de integridad: cada prerrequisito debe ser un curso que exista
    for (int i = 0; i < numCursos; i++) {
        string req = cursos[i].prerrequisito;
        if (req != SIN_PRERREQUISITO && buscarCurso(cursos, numCursos, req) == -1) {
            mensajeAviso("El prerrequisito " + req + " de " + cursos[i].codigo
                         + " no existe en el pensum.");
        }
    }

    return numCursos;
}

// ---------------------------------------------------------------------
// Busqueda secuencial: recorre el arreglo hasta encontrar el codigo.
// ---------------------------------------------------------------------
int buscarCurso(const Curso cursos[], int numCursos, const string& codigo) {
    for (int i = 0; i < numCursos; i++) {
        if (cursos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------
int totalCreditosPensum(const Curso cursos[], int numCursos) {
    int total = 0;
    for (int i = 0; i < numCursos; i++) {
        total += cursos[i].creditos;
    }
    return total;
}

// ---------------------------------------------------------------------
// Texto que describe el requisito de un curso:
//   "090008 Algoritmos", "70 creditos aprobados" o "-"
// ---------------------------------------------------------------------
static string describirRequisito(const Curso cursos[], int numCursos, const Curso& c) {
    if (c.prerrequisito != SIN_PRERREQUISITO) {
        int pos = buscarCurso(cursos, numCursos, c.prerrequisito);
        if (pos != -1) {
            return cursos[pos].codigo + " " + cursos[pos].nombre;
        }
        return c.prerrequisito + " (no encontrado)";
    }
    if (c.creditosMinimos > 0) {
        stringstream ss;
        ss << c.creditosMinimos << " creditos aprobados";
        return ss.str();
    }
    return "-";
}

// ---------------------------------------------------------------------
// Recorre los ciclos del 1 al ultimo y, en cada uno, muestra sus cursos.
// ---------------------------------------------------------------------
void listarPensum(const Curso cursos[], int numCursos) {
    int cicloMayor = ultimoCiclo(cursos, numCursos);

    for (int ciclo = 1; ciclo <= cicloMayor; ciclo++) {
        int creditosCiclo = 0;
        for (int i = 0; i < numCursos; i++) {
            if (cursos[i].ciclo == ciclo) {
                creditosCiclo += cursos[i].creditos;
            }
        }

        subtitulo("Ciclo " + to_string(ciclo) + pintar("   (" + to_string(creditosCiclo) + " creditos)", GRIS));
        cout << pintar("  " + columna("Codigo", 8) + columna("Curso", 46)
                       + columna("Cr.", 5) + "Requisito", GRIS) << "\n";

        for (int i = 0; i < numCursos; i++) {
            if (cursos[i].ciclo == ciclo) {
                cout << "  " << columna(cursos[i].codigo, 8)
                     << columna(cursos[i].nombre, 46)
                     << columna(to_string(cursos[i].creditos), 5)
                     << describirRequisito(cursos, numCursos, cursos[i]) << "\n";
            }
        }
    }

    cout << "\n" << pintar("  Total: ", GRIS) << numCursos << " cursos, "
         << totalCreditosPensum(cursos, numCursos) << " creditos\n";
}

// ---------------------------------------------------------------------
void mostrarCurso(const Curso cursos[], int numCursos, int posicion) {
    const Curso& c = cursos[posicion];
    cout << "\n" << pintar("  Codigo:     ", GRIS) << c.codigo << "\n"
         << pintar("  Nombre:     ", GRIS) << c.nombre << "\n"
         << pintar("  Creditos:   ", GRIS) << c.creditos << "\n"
         << pintar("  Ciclo:      ", GRIS) << c.ciclo << "\n"
         << pintar("  Requisito:  ", GRIS) << describirRequisito(cursos, numCursos, c) << "\n";
}

// ---------------------------------------------------------------------
int ultimoCiclo(const Curso cursos[], int numCursos) {
    int mayor = 0;
    for (int i = 0; i < numCursos; i++) {
        if (cursos[i].ciclo > mayor) {
            mayor = cursos[i].ciclo;
        }
    }
    return mayor;
}

// ---------------------------------------------------------------------
// Lista numerada: el usuario elige por numero en vez de escribir codigos.
// El numero que ve el usuario (1, 2, 3...) no es la posicion real del
// curso; por eso opciones[] guarda las posiciones reales:
//   el usuario elige 2  ->  se devuelve opciones[1]
// ---------------------------------------------------------------------
int elegirCurso(const Curso cursos[], const int opciones[], int cantidad,
                const string& titulo) {
    subtitulo(titulo + ":");

    if (cantidad == 0) {
        mensajeAviso("No hay cursos para elegir.");
        return -1;
    }

    for (int i = 0; i < cantidad; i++) {
        const Curso& c = cursos[opciones[i]];
        opcionMenu(i + 1, columna(c.codigo, 8) + columna(c.nombre, 46)
                          + pintar("ciclo " + to_string(c.ciclo), GRIS));
    }
    opcionMenu(0, "Cancelar");

    int eleccion = leerEntero("\n  Seleccione: ", 0, cantidad);
    if (eleccion == 0) {
        return -1;
    }
    return opciones[eleccion - 1];
}
