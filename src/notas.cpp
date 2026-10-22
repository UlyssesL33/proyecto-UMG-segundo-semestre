// =====================================================================
// notas.cpp - Cursos de un estudiante y sus notas
// =====================================================================
#include "notas.h"
#include "config.h"
#include "validaciones.h"
#include "cursos.h"        // elegirCurso, ultimoCiclo, totalCreditosPensum
#include "estudiantes.h"   // elegirEstudiante

#include <iostream>
#include <iomanip>   // setw
#include <sstream>   // stringstream: armar textos con numeros
#include <string>

using namespace std;

// =====================================================================
// Calculos
// =====================================================================

bool tieneNotas(const Estudiante& e, int curso) {
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        if (e.puntos[curso][k] != SIN_CALIFICAR) {
            return true;
        }
    }
    return false;
}

bool notasCompletas(const Estudiante& e, int curso) {
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        if (e.puntos[curso][k] == SIN_CALIFICAR) {
            return false;
        }
    }
    return true;
}

double notaCurso(const Estudiante& e, int curso) {
    double suma = 0;
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        if (e.puntos[curso][k] != SIN_CALIFICAR) {
            suma += e.puntos[curso][k];
        }
    }
    return suma;
}

void actualizarEstado(Estudiante& e, int curso) {
    if (!notasCompletas(e, curso)) {
        e.estado[curso] = EN_CURSO;
    } else if (notaCurso(e, curso) >= NOTA_MINIMA) {
        e.estado[curso] = APROBADO;
    } else {
        e.estado[curso] = REPROBADO;
    }
}

int creditosAprobados(const Estudiante& e, const Curso cursos[], int numCursos) {
    int total = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == APROBADO) {
            total += cursos[c].creditos;
        }
    }
    return total;
}

const char* textoEstado(int estado) {
    switch (estado) {
        case EN_CURSO:  return "En curso";
        case APROBADO:  return "Aprobado";
        case REPROBADO: return "Reprobado";
        default:        return "No cursado";
    }
}

// =====================================================================
// Funciones auxiliares de este archivo
// =====================================================================

// Convierte unos puntos a texto: -1 -> "-", 12.5 -> "12.5", 10 -> "10"
static string textoPuntos(double valor) {
    if (valor == SIN_CALIFICAR) {
        return "-";
    }
    stringstream ss;
    ss << valor;
    return ss.str();
}

// Pide un ciclo del pensum. Devuelve el ciclo, o 0 si el usuario cancela.
static int elegirCiclo(const Curso cursos[], int numCursos) {
    int mayor = ultimoCiclo(cursos, numCursos);
    stringstream mensaje;
    mensaje << "Ciclo (1 a " << mayor << ", 0 = cancelar): ";
    return leerEntero(mensaje.str(), 0, mayor);
}

// Llena opciones[] con los cursos de un ciclo que el estudiante todavia
// no ha aprobado ni esta llevando (no cursados o reprobados).
// Devuelve cuantos encontro.
static int cursosPendientesDelCiclo(const Estudiante& e, const Curso cursos[], int numCursos,
                                    int ciclo, int opciones[]) {
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        bool pendiente = (e.estado[c] == NO_CURSADO || e.estado[c] == REPROBADO);
        if (cursos[c].ciclo == ciclo && pendiente) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }
    return cantidad;
}

// Pide los puntos de un componente. Acepta de 0 al maximo, o -1 para
// dejarlo sin calificar. Muestra el valor actual como referencia.
static double leerPuntos(int componente, double actual) {
    stringstream mensaje;
    mensaje << "  " << NOMBRES_COMPONENTES[componente]
            << " (0 a " << PUNTOS_MAXIMOS[componente]
            << ", -1 = sin calificar) [actual: " << textoPuntos(actual) << "]: ";

    while (true) {
        double valor = leerDecimal(mensaje.str(), SIN_CALIFICAR, PUNTOS_MAXIMOS[componente]);
        // leerDecimal acepta todo entre -1 y el maximo; aqui se descartan
        // valores como -0.5, que no son ni "sin calificar" ni una nota.
        if (valor == SIN_CALIFICAR || valor >= 0) {
            return valor;
        }
        cout << "  Error: use -1 o un valor desde 0.\n";
    }
}

// =====================================================================
// Paso 8: marcar cursos aprobados de ciclos anteriores (sin notas)
// =====================================================================

// Marca un curso como aprobado. Devuelve true si cambio algo.
static bool marcarAprobado(Estudiante& e, int curso) {
    if (e.estado[curso] == APROBADO) {
        return false;   // ya estaba aprobado
    }
    if (e.estado[curso] == EN_CURSO) {
        return false;   // se aprueba ingresando sus notas, no marcandolo
    }

    // NO_CURSADO o REPROBADO (lo aprobo despues en otro semestre)
    e.estado[curso] = APROBADO;
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = SIN_CALIFICAR;
    }
    return true;
}

static void opcionMarcarCurso(Estudiante& e, const Curso cursos[], int numCursos) {
    int ciclo = elegirCiclo(cursos, numCursos);
    if (ciclo == 0) {
        return;
    }

    int opciones[MAX_CURSOS];
    int cantidad = cursosPendientesDelCiclo(e, cursos, numCursos, ciclo, opciones);
    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos sin aprobar del ciclo");
    if (curso == -1) {
        return;
    }

    marcarAprobado(e, curso);
    cout << "  " << cursos[curso].nombre << " marcado como aprobado (sin notas).\n";
}

// Marca de una vez todos los cursos de un ciclo. Util para registrar
// rapido a un estudiante de tercer semestre o mas.
static void opcionMarcarCiclo(Estudiante& e, const Curso cursos[], int numCursos) {
    int ciclo = elegirCiclo(cursos, numCursos);
    if (ciclo == 0) {
        return;
    }
    int marcados = 0;

    for (int c = 0; c < numCursos; c++) {
        if (cursos[c].ciclo == ciclo && marcarAprobado(e, c)) {
            cout << "  Aprobado: " << cursos[c].codigo << " " << cursos[c].nombre << "\n";
            marcados++;
        }
    }
    cout << "  Cursos marcados: " << marcados
         << " (los que estaban aprobados o en curso no se tocan).\n";
}

// =====================================================================
// Paso 9: inscribir un curso
// =====================================================================

// Por ahora muestra los cursos pendientes del ciclo elegido.
// En el paso 16 esta lista se cambia por los cursos HABILITADOS
// (prerrequisito aprobado y creditos minimos cumplidos).
static void opcionInscribir(Estudiante& e, const Curso cursos[], int numCursos) {
    int ciclo = elegirCiclo(cursos, numCursos);
    if (ciclo == 0) {
        return;
    }

    int opciones[MAX_CURSOS];
    int cantidad = cursosPendientesDelCiclo(e, cursos, numCursos, ciclo, opciones);
    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos que puede inscribir del ciclo");
    if (curso == -1) {
        return;
    }

    if (e.estado[curso] == REPROBADO) {
        cout << "  El curso estaba reprobado: se inscribe de nuevo y sus notas anteriores se borran.\n";
    }

    // Se guarda solo el intento mas reciente: notas en blanco
    e.estado[curso] = EN_CURSO;
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = SIN_CALIFICAR;
    }
    cout << "  Inscrito en " << cursos[curso].nombre << ".\n";
}

// =====================================================================
// Paso 10: ingresar los puntos de un curso
// =====================================================================

// Solo se listan los cursos que pueden llevar notas: los que estan en
// curso y los que ya tienen notas (por si hay que corregir alguna).
// Los aprobados sin notas (ciclos anteriores) no aparecen.
static void opcionIngresarPuntos(Estudiante& e, const Curso cursos[], int numCursos) {
    int opciones[MAX_CURSOS];
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == EN_CURSO || tieneNotas(e, c)) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }

    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos en curso o con notas");
    if (curso == -1) {
        if (cantidad == 0) {
            cout << "  Primero inscriba un curso (opcion 3).\n";
        }
        return;
    }

    cout << "\n  Ingrese los puntos netos obtenidos:\n";
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = leerPuntos(k, e.puntos[curso][k]);
    }

    actualizarEstado(e, curso);

    cout << "\n  Nota del curso: " << textoPuntos(notaCurso(e, curso)) << " / 100\n";
    cout << "  Estado: " << textoEstado(e.estado[curso]) << "\n";
    if (!notasCompletas(e, curso)) {
        cout << "  (Faltan componentes por calificar; el curso sigue en curso.)\n";
    }
}

// =====================================================================
// Pantalla con los cursos del estudiante
// =====================================================================

void mostrarCursosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos) {
    cout << "\n  Cursos de " << e.nombre << ":\n";
    cout << "  " << left << setw(8) << "Codigo" << setw(46) << "Curso"
         << setw(5) << "P1" << setw(5) << "P2" << setw(6) << "Zona"
         << setw(7) << "Final" << setw(7) << "Total" << "Estado\n";

    int llevados = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == NO_CURSADO) {
            continue;
        }
        llevados++;

        cout << "  " << left << setw(8) << cursos[c].codigo << setw(46) << cursos[c].nombre
             << setw(5) << textoPuntos(e.puntos[c][PARCIAL_1])
             << setw(5) << textoPuntos(e.puntos[c][PARCIAL_2])
             << setw(6) << textoPuntos(e.puntos[c][ZONA])
             << setw(7) << textoPuntos(e.puntos[c][FINAL])
             << setw(7) << (tieneNotas(e, c) ? textoPuntos(notaCurso(e, c)) : string("-"))
             << textoEstado(e.estado[c]) << "\n";
    }

    if (llevados == 0) {
        cout << "  (No ha llevado ningun curso.)\n";
    }

    cout << "\n  Creditos aprobados: " << creditosAprobados(e, cursos, numCursos)
         << " de " << totalCreditosPensum(cursos, numCursos) << "\n";
}

// =====================================================================
// Submenu de la opcion 2
// =====================================================================

void menuCursosYNotas(Estudiante estudiantes[], int numEstudiantes,
                      const Curso cursos[], int numCursos) {
    cout << "\n--- Cursos y notas ---\n";

    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }

    // Referencia: los cambios se hacen directamente sobre estudiantes[pos]
    Estudiante& e = estudiantes[pos];
    int opcion;

    do {
        cout << "\n=== " << e.nombre << " (" << e.carne << ") ===\n";
        cout << "1. Marcar curso aprobado (ciclos anteriores, sin notas)\n";
        cout << "2. Marcar aprobado un ciclo completo\n";
        cout << "3. Inscribir curso\n";
        cout << "4. Ingresar / actualizar puntos de un curso\n";
        cout << "5. Ver cursos del estudiante\n";
        cout << "0. Regresar al menu principal\n";
        opcion = leerEntero("Seleccione: ", 0, 5);

        switch (opcion) {
            case 1: opcionMarcarCurso(e, cursos, numCursos);     break;
            case 2: opcionMarcarCiclo(e, cursos, numCursos);     break;
            case 3: opcionInscribir(e, cursos, numCursos);       break;
            case 4: opcionIngresarPuntos(e, cursos, numCursos);  break;
            case 5: mostrarCursosEstudiante(e, cursos, numCursos); break;
            default: break;
        }
    } while (opcion != 0);

    cout << "\n  Recuerde guardar con la opcion 10 del menu principal.\n";
}
