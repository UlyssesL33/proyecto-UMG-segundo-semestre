// =====================================================================
// estadisticas.cpp - Reportes y estadisticas (paso 13)
// =====================================================================
#include "estadisticas.h"
#include "config.h"
#include "pantalla.h"
#include "cursos.h"   // elegirCurso
#include "notas.h"    // notaCurso, notasCompletas, promedioEstudiante, ...

#include <iostream>
#include <string>

using namespace std;

// ---------------------------------------------------------------------
// Resultados de un curso, calculados recorriendo a todos los estudiantes.
// ---------------------------------------------------------------------
struct ResumenCurso {
    int    llevan;       // estudiantes que lo llevan o lo llevaron
    int    aprobados;
    int    reprobados;
    int    enCurso;
    int    conNota;      // estudiantes con los 4 componentes calificados
    double promedio;     // promedio de la nota de esos estudiantes
    double mayor;        // nota mas alta
    int    posMayor;     // posicion del estudiante con la nota mas alta (-1 = ninguno)
};

static ResumenCurso calcularResumenCurso(const Estudiante estudiantes[], int numEstudiantes,
                                         int curso) {
    ResumenCurso r;
    r.llevan = r.aprobados = r.reprobados = r.enCurso = r.conNota = 0;
    r.promedio = 0;
    r.mayor = -1;
    r.posMayor = -1;

    double suma = 0;

    for (int i = 0; i < numEstudiantes; i++) {
        int estado = estudiantes[i].estado[curso];
        if (estado == NO_CURSADO) {
            continue;
        }
        r.llevan++;

        if (estado == APROBADO)  r.aprobados++;
        if (estado == REPROBADO) r.reprobados++;
        if (estado == EN_CURSO)  r.enCurso++;

        if (notasCompletas(estudiantes[i], curso)) {
            double nota = notaCurso(estudiantes[i], curso);
            suma += nota;
            r.conNota++;
            if (nota > r.mayor) {
                r.mayor = nota;
                r.posMayor = i;
            }
        }
    }

    if (r.conNota > 0) {
        r.promedio = suma / r.conNota;
    }
    return r;
}

// ---------------------------------------------------------------------
// Listado por curso
// ---------------------------------------------------------------------
void listarPorCurso(const Estudiante estudiantes[], int numEstudiantes,
                    const Curso cursos[], int numCursos) {
    if (numEstudiantes == 0) {
        mensajeAviso("No hay estudiantes registrados.");
        return;
    }

    // Solo se ofrecen los cursos que al menos un estudiante ha llevado
    int opciones[MAX_CURSOS];
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i].estado[c] != NO_CURSADO) {
                opciones[cantidad] = c;
                cantidad++;
                break;   // ya se sabe que alguien lo lleva; siguiente curso
            }
        }
    }

    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos con estudiantes");
    if (curso == -1) {
        return;
    }

    subtitulo(cursos[curso].codigo + "  " + cursos[curso].nombre);
    cout << pintar("  " + columna("Carne", 16) + columna("Nombre", 30) + columna("P1", 5)
                   + columna("P2", 5) + columna("Zona", 6) + columna("Final", 7)
                   + columna("Total", 7) + "Estado", GRIS) << "\n";

    for (int i = 0; i < numEstudiantes; i++) {
        const Estudiante& e = estudiantes[i];
        if (e.estado[curso] == NO_CURSADO) {
            continue;
        }
        cout << "  " << columna(e.carne, 16) << columna(e.nombre, 30)
             << columna(textoPuntos(e.puntos[curso][PARCIAL_1]), 5)
             << columna(textoPuntos(e.puntos[curso][PARCIAL_2]), 5)
             << columna(textoPuntos(e.puntos[curso][ZONA]), 6)
             << columna(textoPuntos(e.puntos[curso][FINAL]), 7)
             << columna(tieneNotas(e, curso) ? textoPuntos(notaCurso(e, curso)) : string("-"), 7)
             << estadoConColor(e.estado[curso], 0) << "\n";
    }

    ResumenCurso r = calcularResumenCurso(estudiantes, numEstudiantes, curso);

    subtitulo("Resumen del curso");
    cout << pintar("  Estudiantes: ", GRIS) << r.llevan
         << pintar("    Aprobados: ", GRIS) << pintar(to_string(r.aprobados), VERDE)
         << pintar("    Reprobados: ", GRIS) << pintar(to_string(r.reprobados), ROJO)
         << pintar("    En curso: ", GRIS) << pintar(to_string(r.enCurso), AMARILLO) << "\n";

    if (r.conNota > 0) {
        cout << pintar("  Promedio del curso: ", GRIS) << promedioConColor(r.promedio, r.conNota, 0)
             << pintar("  (" + to_string(r.conNota) + " con nota completa)", GRIS) << "\n";
        cout << pintar("  Nota mas alta:      ", GRIS) << textoPuntos(r.mayor)
             << " - " << estudiantes[r.posMayor].nombre << "\n";
    } else {
        cout << pintar("  Nadie tiene todavia la nota completa en este curso.", GRIS) << "\n";
    }
}

// ---------------------------------------------------------------------
// Estadisticas generales
// ---------------------------------------------------------------------
void mostrarEstadisticas(const Estudiante estudiantes[], int numEstudiantes,
                         const Curso cursos[], int numCursos) {
    if (numEstudiantes == 0) {
        mensajeAviso("No hay estudiantes registrados.");
        return;
    }

    // Primera pasada: promedio de cada estudiante, aprobados/reprobados
    // globales y el mayor promedio.
    double sumaPromedios = 0;
    double mayorPromedio = -1;
    int conPromedio = 0, aprobados = 0, reprobados = 0;

    for (int i = 0; i < numEstudiantes; i++) {
        int conNota = 0;
        double promedio = promedioEstudiante(estudiantes[i], numCursos, conNota);
        if (conNota == 0) {
            continue;   // sin notas: no entra en las estadisticas de promedio
        }

        conPromedio++;
        sumaPromedios += promedio;
        if (promedio >= NOTA_MINIMA) {
            aprobados++;
        } else {
            reprobados++;
        }
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
        }
    }

    subtitulo("General");
    cout << pintar("  Estudiantes registrados:  ", GRIS) << numEstudiantes << "\n";
    cout << pintar("  Con al menos una nota:    ", GRIS) << conPromedio << "\n";

    if (conPromedio == 0) {
        mensajeAviso("Ningun estudiante tiene cursos con nota completa todavia.");
        return;
    }

    double promedioGeneral = sumaPromedios / conPromedio;
    cout << pintar("  Promedio general:         ", GRIS) << promedioConColor(promedioGeneral, conPromedio, 0) << "\n";
    cout << pintar("  Aprobados (promedio >= " + textoDecimal(NOTA_MINIMA, 0) + "): ", GRIS)
         << pintar(to_string(aprobados), VERDE) << "\n";
    cout << pintar("  Reprobados:               ", GRIS) << pintar(to_string(reprobados), ROJO) << "\n";

    // Segunda pasada: puede haber empate en el mayor promedio, asi que se
    // muestran todos los que lo tengan.
    subtitulo("Mayor promedio: " + textoDecimal(mayorPromedio, 2));
    for (int i = 0; i < numEstudiantes; i++) {
        int conNota = 0;
        double promedio = promedioEstudiante(estudiantes[i], numCursos, conNota);
        if (conNota > 0 && promedio == mayorPromedio) {
            cout << "  " << columna(estudiantes[i].carne, 16) << estudiantes[i].nombre << "\n";
        }
    }

    // Resumen de cada curso que tenga al menos un estudiante
    subtitulo("Por curso");
    cout << pintar("  " + columna("Codigo", 8) + columna("Curso", 46) + columna("Est.", 6)
                   + columna("Aprob.", 8) + columna("Reprob.", 9) + columna("En cur.", 9)
                   + "Promedio", GRIS) << "\n";

    for (int c = 0; c < numCursos; c++) {
        ResumenCurso r = calcularResumenCurso(estudiantes, numEstudiantes, c);
        if (r.llevan == 0) {
            continue;
        }
        cout << "  " << columna(cursos[c].codigo, 8) << columna(cursos[c].nombre, 46)
             << columna(to_string(r.llevan), 6)
             << pintar(columna(to_string(r.aprobados), 8), VERDE)
             << pintar(columna(to_string(r.reprobados), 9), ROJO)
             << pintar(columna(to_string(r.enCurso), 9), AMARILLO)
             << promedioConColor(r.promedio, r.conNota, 0) << "\n";
    }
    cout << pintar("  Est. = estudiantes que lo llevan o lo llevaron.", GRIS) << "\n";
    cout << pintar("  El promedio solo incluye notas completas (los aprobados sin notas no entran).", GRIS) << "\n";
}
