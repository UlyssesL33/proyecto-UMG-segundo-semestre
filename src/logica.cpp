// logica.cpp - Regla de habilitacion
#include "logica.h"
#include "config.h"
#include "pantalla.h"
#include "cursos.h"
#include "notas.h"
#include "conjuntos.h"

#include <iostream>

using namespace std;

Habilitacion evaluarHabilitacion(const Estudiante& e, const Curso cursos[], int numCursos,
                                 int curso) {
    Habilitacion h;
    const Curso& c = cursos[curso];

    h.p = (e.estado[curso] != APROBADO);
    h.q = (e.estado[curso] != EN_CURSO);

    // r: si no tiene curso prerrequisito, la proposicion es verdadera
    if (c.prerrequisito == SIN_PRERREQUISITO) {
        h.r = true;
    } else {
        int req = buscarCurso(cursos, numCursos, c.prerrequisito);
        h.r = (req != -1 && e.estado[req] == APROBADO);
    }

    // s: si el curso no pide creditos (minimo 0), siempre se cumple
    h.s = (creditosAprobados(e, cursos, numCursos) >= c.creditosMinimos);

    h.resultado = h.p && h.q && h.r && h.s;
    return h;
}

bool estaHabilitado(const Estudiante& e, const Curso cursos[], int numCursos, int curso) {
    return evaluarHabilitacion(e, cursos, numCursos, curso).resultado;
}

void conjuntoHabilitados(const Estudiante& e, const Curso cursos[], int numCursos, bool H[]) {
    for (int c = 0; c < numCursos; c++) {
        H[c] = estaHabilitado(e, cursos, numCursos, c);
    }
}

// "V" verde o "F" rojo, con ancho fijo para la tabla
static string valorVerdad(bool valor, int ancho) {
    return valor ? pintar(columna("V", ancho), VERDE) : pintar(columna("F", ancho), ROJO);
}

// Tabla de verdad de la regla, solo para los cursos pendientes (P = S - A):
// los aprobados ya se sabe que no estan habilitados (p es falsa).
void pantallaHabilitados(const Estudiante& e, const Curso cursos[], int numCursos) {
    int creditos = creditosAprobados(e, cursos, numCursos);

    concepto("Proposicion logica:  habilitado(c) = p AND q AND r AND s");
    lineaConcepto("p: c no esta aprobado          q: c no esta en curso");
    lineaConcepto("r: su prerrequisito aprobado   s: creditos aprobados >= minimo de c");
    lineaConcepto("Basta UNA proposicion falsa (F) para que el curso no se pueda inscribir.");

    subtitulo("Cursos pendientes de " + e.nombre + "   (creditos aprobados: " + to_string(creditos) + ")");
    cout << pintar("  " + columna("Codigo", 8) + columna("Curso", 46) + columna("p", 3)
                   + columna("q", 3) + columna("r", 3) + columna("s", 3) + "Habilitado", GRIS) << "\n";

    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == APROBADO) {
            continue;
        }
        Habilitacion h = evaluarHabilitacion(e, cursos, numCursos, c);

        cout << "  " << columna(cursos[c].codigo, 8) << columna(cursos[c].nombre, 46)
             << valorVerdad(h.p, 3) << valorVerdad(h.q, 3)
             << valorVerdad(h.r, 3) << valorVerdad(h.s, 3)
             << (h.resultado ? pintar("SI", VERDE) : pintar("no", GRIS)) << "\n";
    }

    // El resultado visto como conjunto
    bool H[MAX_CURSOS], A[MAX_CURSOS], S[MAX_CURSOS], P[MAX_CURSOS];
    conjuntoHabilitados(e, cursos, numCursos, H);
    conjuntoPorEstado(e, numCursos, APROBADO, A);
    conjuntoPensum(S, numCursos);
    diferencia(S, A, P, numCursos);

    concepto("De logica a conjuntos");
    lineaConcepto("H = { c en S | habilitado(c) es verdadera }   y siempre  H <= P");
    mostrarConjunto("H", H, cursos, numCursos);
    cout << pintar("  H <= P : ", GRIS)
         << (esSubconjunto(H, P, numCursos) ? pintar("Verdadero", VERDE) : pintar("Falso", ROJO))
         << pintar("   (todo curso habilitado es un curso pendiente)", GRIS) << "\n";
}
