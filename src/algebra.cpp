// =====================================================================
// algebra.cpp - Analisis matricial (Algebra Lineal, pasos 19 y 20)
// =====================================================================
#include "algebra.h"
#include "pantalla.h"
#include "validaciones.h"   // leerEntero
#include "cursos.h"         // ultimoCiclo, elegirCurso
#include "estudiantes.h"    // elegirEstudiante
#include "notas.h"          // notasCompletas, notaCurso, textoPuntos

#include <iostream>
#include <string>

using namespace std;

// =====================================================================
// Operaciones con matrices
// =====================================================================

void transpuesta(const double A[][MAX_COLUMNAS], int m, int n, double AT[][MAX_FILAS]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            AT[j][i] = A[i][j];   // la fila i pasa a ser la columna i
        }
    }
}

// Cada elemento del resultado es el producto punto de una fila por v:
//   r[i] = A[i][0]*v[0] + A[i][1]*v[1] + ... + A[i][n-1]*v[n-1]
void matrizPorVector(const double A[][MAX_COLUMNAS], int m, int n,
                     const double v[], double r[]) {
    for (int i = 0; i < m; i++) {
        r[i] = 0;
        for (int j = 0; j < n; j++) {
            r[i] += A[i][j] * v[j];
        }
    }
}

void transpuestaPorVector(const double AT[][MAX_FILAS], int n, int m,
                          const double v[], double r[]) {
    for (int j = 0; j < n; j++) {
        r[j] = 0;
        for (int i = 0; i < m; i++) {
            r[j] += AT[j][i] * v[i];
        }
    }
}

// =====================================================================
// Armar las matrices de un ciclo
// =====================================================================

// Datos de las matrices de un ciclo: que estudiante es cada fila y que
// curso es cada columna.
struct MatricesCiclo {
    double P[MAX_FILAS][MAX_COLUMNAS];   // notas
    double M[MAX_FILAS][MAX_COLUMNAS];   // 1 si hay nota, 0 si no
    int    fila[MAX_FILAS];              // posicion del estudiante de cada fila
    int    columna[MAX_COLUMNAS];        // posicion del curso de cada columna
    int    m;                            // cantidad de filas (estudiantes)
    int    n;                            // cantidad de columnas (cursos)
};

// Columnas: los cursos del ciclo. Filas: los estudiantes que tienen al
// menos una nota completa en esos cursos (los demas serian filas de ceros).
static void armarMatrices(const Estudiante estudiantes[], int numEstudiantes,
                          const Curso cursos[], int numCursos, int ciclo, MatricesCiclo& mc) {
    mc.n = 0;
    for (int c = 0; c < numCursos && mc.n < MAX_COLUMNAS; c++) {
        if (cursos[c].ciclo == ciclo) {
            mc.columna[mc.n] = c;
            mc.n++;
        }
    }

    mc.m = 0;
    for (int e = 0; e < numEstudiantes; e++) {
        bool tieneAlguna = false;
        for (int j = 0; j < mc.n; j++) {
            if (notasCompletas(estudiantes[e], mc.columna[j])) {
                tieneAlguna = true;
            }
        }
        if (!tieneAlguna) {
            continue;
        }

        int i = mc.m;
        mc.fila[i] = e;
        for (int j = 0; j < mc.n; j++) {
            int c = mc.columna[j];
            if (notasCompletas(estudiantes[e], c)) {
                mc.P[i][j] = notaCurso(estudiantes[e], c);
                mc.M[i][j] = 1;
            } else {
                mc.P[i][j] = 0;
                mc.M[i][j] = 0;
            }
        }
        mc.m++;
    }
}

// Pide un ciclo y arma sus matrices. Devuelve false si no hay datos.
static bool pedirCicloYArmar(const Estudiante estudiantes[], int numEstudiantes,
                             const Curso cursos[], int numCursos, MatricesCiclo& mc, int& ciclo) {
    int mayor = ultimoCiclo(cursos, numCursos);
    ciclo = leerEntero("  Ciclo a analizar (1 a " + to_string(mayor) + ", 0 = cancelar): ", 0, mayor);
    if (ciclo == 0) {
        return false;
    }

    armarMatrices(estudiantes, numEstudiantes, cursos, numCursos, ciclo, mc);
    if (mc.m == 0) {
        mensajeAviso("Ningun estudiante tiene notas completas en cursos del ciclo " + to_string(ciclo) + ".");
        mensajeAviso("Los cursos aprobados sin notas no tienen un numero que poner en la matriz.");
        return false;
    }
    return true;
}

// =====================================================================
// Mostrar matrices y vectores
// =====================================================================

// Encabezado de columnas con el codigo de cada curso
static void encabezadoCursos(const MatricesCiclo& mc, const Curso cursos[], int anchoEtiqueta) {
    cout << "  " << string(anchoEtiqueta, ' ');
    for (int j = 0; j < mc.n; j++) {
        cout << pintar(columna(cursos[mc.columna[j]].codigo, 9), GRIS);
    }
    cout << "\n";
}

// Muestra P o M. Los ceros de "no tiene nota" se ven en gris.
static void mostrarMatriz(const string& nombre, const double A[][MAX_COLUMNAS], const MatricesCiclo& mc,
                          const Estudiante estudiantes[], const Curso cursos[], bool esIndicadora) {
    subtitulo(nombre + "  (" + to_string(mc.m) + " x " + to_string(mc.n) + ")");
    encabezadoCursos(mc, cursos, 16);

    for (int i = 0; i < mc.m; i++) {
        cout << "  " << pintar(columna(estudiantes[mc.fila[i]].carne, 16), GRIS);
        for (int j = 0; j < mc.n; j++) {
            string valor = esIndicadora ? textoDecimal(A[i][j], 0) : textoDecimal(A[i][j], 1);
            cout << (mc.M[i][j] == 0 ? pintar(columna(valor, 9), GRIS) : columna(valor, 9));
        }
        cout << "\n";
    }
}

// Lista "columna -> curso" para leer la matriz
static void leyendaCursos(const MatricesCiclo& mc, const Curso cursos[]) {
    for (int j = 0; j < mc.n; j++) {
        cout << pintar("  " + cursos[mc.columna[j]].codigo + " = " + cursos[mc.columna[j]].nombre, GRIS) << "\n";
    }
}

// =====================================================================
// Paso 19: matriz P, transpuesta y promedio por curso
// =====================================================================

static void pantallaMatrizCiclo(const Estudiante estudiantes[], int numEstudiantes,
                                const Curso cursos[], int numCursos) {
    MatricesCiclo mc;
    int ciclo;
    if (!pedirCicloYArmar(estudiantes, numEstudiantes, cursos, numCursos, mc, ciclo)) {
        return;
    }

    concepto("Matriz de notas P  (filas = estudiantes, columnas = cursos)");
    lineaConcepto("P[i][j] = nota del estudiante i en el curso j  (0 si no tiene nota)");
    lineaConcepto("M[i][j] = 1 si esa nota existe, 0 si no  (para no confundirla con un 0 real)");

    mostrarMatriz("P", mc.P, mc, estudiantes, cursos, false);
    mostrarMatriz("M", mc.M, mc, estudiantes, cursos, true);
    leyendaCursos(mc, cursos);

    // ---- Transpuesta
    double PT[MAX_COLUMNAS][MAX_FILAS];
    double MT[MAX_COLUMNAS][MAX_FILAS];
    transpuesta(mc.P, mc.m, mc.n, PT);
    transpuesta(mc.M, mc.m, mc.n, MT);

    concepto("Transpuesta:  P^T[j][i] = P[i][j]");
    lineaConcepto("Las filas de P pasan a ser columnas: ahora cada FILA es un curso.");

    subtitulo("P^T  (" + to_string(mc.n) + " x " + to_string(mc.m) + ")");
    cout << "  " << string(8, ' ');
    for (int i = 0; i < mc.m; i++) {
        cout << pintar(columna("E" + to_string(i + 1), 7), GRIS);
    }
    cout << "\n";
    for (int j = 0; j < mc.n; j++) {
        cout << "  " << pintar(columna(cursos[mc.columna[j]].codigo, 8), GRIS);
        for (int i = 0; i < mc.m; i++) {
            string valor = textoDecimal(PT[j][i], 1);
            cout << (MT[j][i] == 0 ? pintar(columna(valor, 7), GRIS) : columna(valor, 7));
        }
        cout << "\n";
    }
    for (int i = 0; i < mc.m; i++) {
        cout << pintar("  E" + to_string(i + 1) + " = " + estudiantes[mc.fila[i]].nombre, GRIS) << "\n";
    }

    // ---- Producto por el vector de unos
    double unos[MAX_FILAS];
    for (int i = 0; i < mc.m; i++) {
        unos[i] = 1;
    }
    double suma[MAX_COLUMNAS], cantidad[MAX_COLUMNAS];
    transpuestaPorVector(PT, mc.n, mc.m, unos, suma);
    transpuestaPorVector(MT, mc.n, mc.m, unos, cantidad);

    concepto("Promedio por curso con producto matriz-vector");
    lineaConcepto("s = P^T * 1   suma las notas de cada curso   (1 = vector de " + to_string(mc.m) + " unos)");
    lineaConcepto("k = M^T * 1   cuenta cuantas notas tiene cada curso");
    lineaConcepto("promedio del curso j = s[j] / k[j]");

    subtitulo("Resultado");
    cout << pintar("  " + columna("Curso", 46) + columna("s", 9) + columna("k", 5) + "Promedio", GRIS) << "\n";
    for (int j = 0; j < mc.n; j++) {
        int c = mc.columna[j];
        cout << "  " << columna(cursos[c].codigo + " " + cursos[c].nombre, 46)
             << columna(textoDecimal(suma[j], 1), 9) << columna(textoDecimal(cantidad[j], 0), 5);
        if (cantidad[j] == 0) {
            cout << pintar("-", GRIS);
        } else {
            cout << promedioConColor(suma[j] / cantidad[j], 1, 0);
        }
        cout << "\n";
    }
    cout << pintar("  Coincide con el promedio por curso de la opcion 7 (Estadisticas).", GRIS) << "\n";
}

// =====================================================================
// Paso 20 (a): promedio ponderado por creditos  P * c
// =====================================================================

static void pantallaPonderado(const Estudiante estudiantes[], int numEstudiantes,
                              const Curso cursos[], int numCursos) {
    MatricesCiclo mc;
    int ciclo;
    if (!pedirCicloYArmar(estudiantes, numEstudiantes, cursos, numCursos, mc, ciclo)) {
        return;
    }

    // Vector de creditos de los cursos del ciclo
    double c[MAX_COLUMNAS];
    string textoC = "c = [ ";
    for (int j = 0; j < mc.n; j++) {
        c[j] = cursos[mc.columna[j]].creditos;
        textoC += textoDecimal(c[j], 0) + (j < mc.n - 1 ? ", " : " ]");
    }

    concepto("Promedio ponderado:  (P * c) / (M * c)");
    lineaConcepto("c = vector con los creditos de cada curso del ciclo");
    lineaConcepto("P * c = suma de (nota x creditos)    M * c = creditos de los cursos con nota");
    lineaConcepto("Un curso de mas creditos pesa mas en el promedio.");

    mostrarMatriz("P", mc.P, mc, estudiantes, cursos, false);
    cout << "\n  " << pintar(textoC, CIAN) << "\n";
    leyendaCursos(mc, cursos);

    double Pc[MAX_FILAS], Mc[MAX_FILAS];
    matrizPorVector(mc.P, mc.m, mc.n, c, Pc);
    matrizPorVector(mc.M, mc.m, mc.n, c, Mc);

    subtitulo("Resultado por estudiante (ciclo " + to_string(ciclo) + ")");
    cout << pintar("  " + columna("Carne", 16) + columna("Nombre", 24) + columna("P*c", 9)
                   + columna("M*c", 6) + columna("Ponderado", 11) + "Simple", GRIS) << "\n";

    for (int i = 0; i < mc.m; i++) {
        // Promedio simple de las notas existentes, para comparar
        double suma = 0;
        int cuantas = 0;
        for (int j = 0; j < mc.n; j++) {
            suma += mc.P[i][j];
            cuantas += (int)mc.M[i][j];
        }

        const Estudiante& e = estudiantes[mc.fila[i]];
        cout << "  " << columna(e.carne, 16) << columna(e.nombre, 24)
             << columna(textoDecimal(Pc[i], 1), 9) << columna(textoDecimal(Mc[i], 0), 6)
             << promedioConColor(Pc[i] / Mc[i], 1, 11)
             << textoDecimal(suma / cuantas, 2) << "\n";
    }
    cout << pintar("  Si todos los cursos tienen los mismos creditos, ponderado = simple.", GRIS) << "\n";
}

// =====================================================================
// Paso 20 (b): nota necesaria para aprobar un curso en curso
// =====================================================================

static void pantallaNotaNecesaria(const Estudiante estudiantes[], int numEstudiantes,
                                  const Curso cursos[], int numCursos) {
    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }
    const Estudiante& e = estudiantes[pos];

    int opciones[MAX_CURSOS];
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == EN_CURSO) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }
    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos en curso de " + e.nombre);
    if (curso == -1) {
        return;
    }

    // Lo acumulado y lo que todavia se puede ganar
    double acumulado = 0;
    double maximoPendiente = 0;
    string pendientes = "";

    subtitulo(cursos[curso].nombre);
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        double puntos = e.puntos[curso][k];
        cout << "  " << columna(NOMBRES_COMPONENTES[k], 12);
        if (puntos == SIN_CALIFICAR) {
            cout << pintar("pendiente  (max. " + textoDecimal(PUNTOS_MAXIMOS[k], 0) + ")", AMARILLO) << "\n";
            maximoPendiente += PUNTOS_MAXIMOS[k];
            pendientes += (pendientes.empty() ? "" : " + ") + NOMBRES_COMPONENTES[k];
        } else {
            cout << textoPuntos(puntos) << " / " << textoDecimal(PUNTOS_MAXIMOS[k], 0) << "\n";
            acumulado += puntos;
        }
    }

    double x = NOTA_MINIMA - acumulado;

    concepto("Ecuacion lineal con una incognita");
    lineaConcepto("acumulado + x = NOTA_MINIMA     ->     x = NOTA_MINIMA - acumulado");
    lineaConcepto("Restriccion:  0 <= x <= puntos maximos de lo pendiente");
    cout << "  " << pintar("|  ", GRIS) << textoPuntos(acumulado) << " + x = " << textoDecimal(NOTA_MINIMA, 0)
         << "     ->     x = " << textoDecimal(NOTA_MINIMA, 0) << " - " << textoPuntos(acumulado)
         << " = " << textoDecimal(x, 1) << "\n";

    subtitulo("Resultado");
    cout << pintar("  Pendiente: ", GRIS) << pendientes
         << pintar("   (puede ganar hasta " + textoDecimal(maximoPendiente, 0) + " puntos)", GRIS) << "\n";

    if (x <= 0) {
        mensajeExito("Ya acumula " + textoPuntos(acumulado) + ": aprueba aunque saque 0 en lo pendiente.");
    } else if (x > maximoPendiente) {
        mensajeError("No alcanza: necesitaria " + textoDecimal(x, 1) + " de " + textoDecimal(maximoPendiente, 0)
                     + " posibles. Con todo lo maximo llegaria a " + textoDecimal(acumulado + maximoPendiente, 1) + ".");
    } else {
        mensajeAviso("Necesita " + textoDecimal(x, 1) + " de " + textoDecimal(maximoPendiente, 0)
                     + " puntos (" + textoDecimal(x / maximoPendiente * 100, 0) + "% de lo pendiente).");
    }
}

// =====================================================================
// Submenu de la opcion 8
// =====================================================================

void menuAlgebra(const Estudiante estudiantes[], int numEstudiantes,
                 const Curso cursos[], int numCursos) {
    const string RUTA = "Menu principal > Algebra Lineal";
    const string NOMBRES[4] = {
        "", "Matriz de notas y transpuesta", "Promedio ponderado por creditos",
        "Nota necesaria para aprobar"
    };
    int opcion;

    do {
        encabezado(RUTA);
        grupoMenu("MATRICES");
        opcionMenu(1, NOMBRES[1] + pintar("     (P, M, P^T, promedio = P^T * 1)", GRIS));
        opcionMenu(2, NOMBRES[2] + pintar("   (P * c)", GRIS));
        grupoMenu("ECUACIONES");
        opcionMenu(3, NOMBRES[3] + pintar("       (acumulado + x = " + textoDecimal(NOTA_MINIMA, 0) + ")", GRIS));
        opcionMenu(0, "Regresar al menu principal");
        cout << "\n";
        separador();
        opcion = leerEntero("  Seleccione: ", 0, 3);

        if (opcion == 0) {
            break;
        }
        encabezado(RUTA + " > " + NOMBRES[opcion]);

        if (opcion == 1) pantallaMatrizCiclo(estudiantes, numEstudiantes, cursos, numCursos);
        if (opcion == 2) pantallaPonderado(estudiantes, numEstudiantes, cursos, numCursos);
        if (opcion == 3) pantallaNotaNecesaria(estudiantes, numEstudiantes, cursos, numCursos);

        pausar();
    } while (opcion != 0);
}
