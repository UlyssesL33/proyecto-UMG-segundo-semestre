// conjuntos.cpp - Conjuntos de cursos (Matematica Discreta, paso 15)
#include "conjuntos.h"
#include "config.h"
#include "pantalla.h"

#include <iostream>

using namespace std;

// Construir conjuntos

void conjuntoPensum(bool S[], int numCursos) {
    for (int c = 0; c < numCursos; c++) {
        S[c] = true;   // todo curso del pensum pertenece al universo
    }
}

void conjuntoPorEstado(const Estudiante& e, int numCursos, int estado, bool resultado[]) {
    for (int c = 0; c < numCursos; c++) {
        resultado[c] = (e.estado[c] == estado);
    }
}

// Operaciones: cada una recorre las posiciones y aplica un operador
// logico. Por eso conjuntos y logica proposicional van de la mano.

void unionConjuntos(const bool A[], const bool B[], bool resultado[], int numCursos) {
    for (int c = 0; c < numCursos; c++) {
        resultado[c] = A[c] || B[c];
    }
}

void interseccion(const bool A[], const bool B[], bool resultado[], int numCursos) {
    for (int c = 0; c < numCursos; c++) {
        resultado[c] = A[c] && B[c];
    }
}

void diferencia(const bool A[], const bool B[], bool resultado[], int numCursos) {
    for (int c = 0; c < numCursos; c++) {
        resultado[c] = A[c] && !B[c];
    }
}

// A es subconjunto de B si NO existe un elemento que este en A y no en B.
// Basta encontrar uno solo para saber que no lo es.
bool esSubconjunto(const bool A[], const bool B[], int numCursos) {
    for (int c = 0; c < numCursos; c++) {
        if (A[c] && !B[c]) {
            return false;
        }
    }
    return true;
}

int cardinalidad(const bool A[], int numCursos) {
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (A[c]) {
            cantidad++;
        }
    }
    return cantidad;
}

int creditosDe(const bool A[], const Curso cursos[], int numCursos) {
    int total = 0;
    for (int c = 0; c < numCursos; c++) {
        if (A[c]) {
            total += cursos[c].creditos;
        }
    }
    return total;
}

// Mostrar

// Ejemplo:   A = { 090001, 090002, 090008 }   |A| = 3
// Si son muchos, se parte en varias lineas de 7 codigos.
void mostrarConjunto(const string& nombre, const bool A[],
                     const Curso cursos[], int numCursos) {
    const int POR_LINEA = 7;
    string sangria(nombre.length() + 7, ' ');

    cout << "  " << pintar(nombre, CIAN) << " = { ";

    int escritos = 0;
    for (int c = 0; c < numCursos; c++) {
        if (!A[c]) {
            continue;
        }
        if (escritos > 0) {
            cout << ", ";
            if (escritos % POR_LINEA == 0) {
                cout << "\n" << sangria;
            }
        }
        cout << cursos[c].codigo;
        escritos++;
    }

    cout << (escritos > 0 ? " }" : "}")
         << pintar("   |" + nombre + "| = " + to_string(escritos), GRIS) << "\n";
}

void mostrarConjuntoDetallado(const bool A[], const Curso cursos[], int numCursos) {
    int escritos = 0;
    for (int c = 0; c < numCursos; c++) {
        if (A[c]) {
            cout << "     " << columna(cursos[c].codigo, 8) << cursos[c].nombre << "\n";
            escritos++;
        }
    }
    if (escritos == 0) {
        cout << pintar("     (conjunto vacio)", GRIS) << "\n";
    }
}

// Muestra "Verdadero" en verde o "Falso" en rojo.
static string verdad(bool valor) {
    return valor ? pintar("Verdadero", VERDE) : pintar("Falso", ROJO);
}

// Pantalla: conjuntos de un estudiante

void pantallaConjuntosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos) {
    bool S[MAX_CURSOS], A[MAX_CURSOS], C[MAX_CURSOS], R[MAX_CURSOS];
    bool P[MAX_CURSOS], L[MAX_CURSOS], temporal[MAX_CURSOS], AnC[MAX_CURSOS];

    conjuntoPensum(S, numCursos);
    conjuntoPorEstado(e, numCursos, APROBADO, A);
    conjuntoPorEstado(e, numCursos, EN_CURSO, C);
    conjuntoPorEstado(e, numCursos, REPROBADO, R);

    concepto("Un conjunto es un arreglo de bool");
    lineaConcepto("X[c] = true  significa que el curso c pertenece al conjunto X.");
    lineaConcepto("S = Pensum (universo)   A = Aprobados   C = En curso   R = Reprobados");

    subtitulo("Conjuntos de " + e.nombre);
    cout << "  " << pintar("S", CIAN) << " = { todo el pensum }"
         << pintar("   |S| = " + to_string(cardinalidad(S, numCursos)), GRIS) << "\n";
    mostrarConjunto("A", A, cursos, numCursos);
    mostrarConjunto("C", C, cursos, numCursos);
    mostrarConjunto("R", R, cursos, numCursos);

    // ---- Diferencia: pendientes
    diferencia(S, A, P, numCursos);
    concepto("Diferencia de conjuntos:  P = S - A");
    lineaConcepto("Pendientes = cursos del pensum que NO pertenecen a Aprobados.");
    lineaConcepto("En codigo:  P[c] = S[c] && !A[c]");
    mostrarConjunto("P", P, cursos, numCursos);

    int nA = cardinalidad(A, numCursos), nP = cardinalidad(P, numCursos), nS = cardinalidad(S, numCursos);
    cout << pintar("  Comprobacion: |A| + |P| = |S|  ->  ", GRIS)
         << nA << " + " << nP << " = " << (nA + nP) << "  "
         << ((nA + nP == nS) ? pintar("[OK]", VERDE) : pintar("[X]", ROJO)) << "\n";

    // ---- Union: cursos llevados alguna vez
    unionConjuntos(A, C, temporal, numCursos);
    unionConjuntos(temporal, R, L, numCursos);
    concepto("Union de conjuntos:  L = A u C u R");
    lineaConcepto("Llevados = cursos que estan en A, O en C, O en R.");
    lineaConcepto("En codigo:  L[c] = A[c] || C[c] || R[c]");
    mostrarConjunto("L", L, cursos, numCursos);

    // ---- Interseccion y subconjunto: propiedades que siempre se cumplen
    interseccion(A, C, AnC, numCursos);
    concepto("Interseccion y subconjunto");
    lineaConcepto("A n C = cursos aprobados Y en curso a la vez. Debe ser vacio.");
    lineaConcepto("A <= S : todo curso aprobado pertenece al pensum.");
    cout << pintar("  |A n C| = 0 :  ", GRIS) << verdad(cardinalidad(AnC, numCursos) == 0) << "\n";
    cout << pintar("  A <= S      :  ", GRIS) << verdad(esSubconjunto(A, S, numCursos)) << "\n";
    cout << pintar("  P <= S      :  ", GRIS) << verdad(esSubconjunto(P, S, numCursos)) << "\n";

    cout << "\n" << pintar("  Creditos de A: ", GRIS) << creditosDe(A, cursos, numCursos)
         << pintar("   Creditos de P: ", GRIS) << creditosDe(P, cursos, numCursos)
         << pintar("   (suman los ", GRIS) << creditosDe(S, cursos, numCursos)
         << pintar(" del pensum)", GRIS) << "\n";
}

// Pantalla: comparar dos estudiantes

void pantallaCompararEstudiantes(const Estudiante& e1, const Estudiante& e2,
                                 const Curso cursos[], int numCursos) {
    bool A1[MAX_CURSOS], A2[MAX_CURSOS];
    bool comun[MAX_CURSOS], solo1[MAX_CURSOS], solo2[MAX_CURSOS], todos[MAX_CURSOS];

    conjuntoPorEstado(e1, numCursos, APROBADO, A1);
    conjuntoPorEstado(e2, numCursos, APROBADO, A2);

    interseccion(A1, A2, comun, numCursos);
    diferencia(A1, A2, solo1, numCursos);
    diferencia(A2, A1, solo2, numCursos);
    unionConjuntos(A1, A2, todos, numCursos);

    concepto("Operaciones entre dos conjuntos");
    lineaConcepto("A1 = aprobados de " + e1.nombre + "    A2 = aprobados de " + e2.nombre);
    lineaConcepto("A1 n A2 : en comun     A1 - A2 : solo el primero     A1 u A2 : entre los dos");

    subtitulo("Conjuntos");
    mostrarConjunto("A1", A1, cursos, numCursos);
    mostrarConjunto("A2", A2, cursos, numCursos);

    subtitulo("A1 n A2  (interseccion: aprobados por ambos)");
    mostrarConjuntoDetallado(comun, cursos, numCursos);

    subtitulo("A1 - A2  (diferencia: solo " + e1.nombre + ")");
    mostrarConjuntoDetallado(solo1, cursos, numCursos);

    subtitulo("A2 - A1  (diferencia: solo " + e2.nombre + ")");
    mostrarConjuntoDetallado(solo2, cursos, numCursos);

    int nU = cardinalidad(todos, numCursos);
    int n1 = cardinalidad(A1, numCursos), n2 = cardinalidad(A2, numCursos), nI = cardinalidad(comun, numCursos);

    concepto("Principio de inclusion-exclusion");
    lineaConcepto("|A1 u A2| = |A1| + |A2| - |A1 n A2|");
    cout << pintar("  ", GRIS) << nU << " = " << n1 << " + " << n2 << " - " << nI << "  "
         << ((nU == n1 + n2 - nI) ? pintar("[OK]", VERDE) : pintar("[X]", ROJO)) << "\n";
}
