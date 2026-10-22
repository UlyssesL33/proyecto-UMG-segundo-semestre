// =====================================================================
// notas.cpp - Cursos de un estudiante y sus notas
// =====================================================================
#include "notas.h"
#include "config.h"
#include "validaciones.h"
#include "cursos.h"        // elegirCurso, ultimoCiclo, totalCreditosPensum
#include "estudiantes.h"   // elegirEstudiante
#include "pantalla.h"

#include <iostream>
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

string estadoConColor(int estado, int ancho) {
    string texto = columna(textoEstado(estado), ancho);
    switch (estado) {
        case APROBADO:  return pintar(texto, VERDE);
        case REPROBADO: return pintar(texto, ROJO);
        case EN_CURSO:  return pintar(texto, AMARILLO);
        default:        return pintar(texto, GRIS);
    }
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
    mensaje << "  Ciclo (1 a " << mayor << ", 0 = cancelar): ";
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
        mensajeError("Use -1 o un valor desde 0.");
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
    cout << "\n";
    mensajeExito(cursos[curso].nombre + " marcado como aprobado (sin notas).");
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
            mensajeExito(cursos[c].codigo + "  " + cursos[c].nombre);
            marcados++;
        }
    }
    cout << "\n" << pintar("  Cursos marcados: ", GRIS) << marcados
         << pintar("  (los aprobados o en curso no se tocan)", GRIS) << "\n";
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
        mensajeAviso("El curso estaba reprobado: se inscribe de nuevo y sus notas anteriores se borran.");
    }

    // Se guarda solo el intento mas reciente: notas en blanco
    e.estado[curso] = EN_CURSO;
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = SIN_CALIFICAR;
    }
    cout << "\n";
    mensajeExito("Inscrito en " + cursos[curso].nombre + ".");
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
            mensajeAviso("Primero inscriba un curso (opcion 3).");
        }
        return;
    }

    subtitulo(cursos[curso].nombre + ": puntos netos obtenidos");
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = leerPuntos(k, e.puntos[curso][k]);
    }

    actualizarEstado(e, curso);

    cout << "\n" << pintar("  Nota del curso: ", GRIS) << textoPuntos(notaCurso(e, curso)) << " / 100\n";
    cout << pintar("  Estado:         ", GRIS) << estadoConColor(e.estado[curso], 0) << "\n";
    if (!notasCompletas(e, curso)) {
        mensajeAviso("Faltan componentes por calificar; el curso sigue en curso.");
    }
}

// =====================================================================
// Pantalla con los cursos del estudiante
// =====================================================================

void mostrarCursosEstudiante(const Estudiante& e, const Curso cursos[], int numCursos) {
    subtitulo("Cursos de " + e.nombre);
    cout << pintar("  " + columna("Codigo", 8) + columna("Curso", 46) + columna("P1", 5)
                   + columna("P2", 5) + columna("Zona", 6) + columna("Final", 7)
                   + columna("Total", 7) + "Estado", GRIS) << "\n";

    int llevados = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == NO_CURSADO) {
            continue;
        }
        llevados++;

        cout << "  " << columna(cursos[c].codigo, 8) << columna(cursos[c].nombre, 46)
             << columna(textoPuntos(e.puntos[c][PARCIAL_1]), 5)
             << columna(textoPuntos(e.puntos[c][PARCIAL_2]), 5)
             << columna(textoPuntos(e.puntos[c][ZONA]), 6)
             << columna(textoPuntos(e.puntos[c][FINAL]), 7)
             << columna(tieneNotas(e, c) ? textoPuntos(notaCurso(e, c)) : string("-"), 7)
             << estadoConColor(e.estado[c], 0) << "\n";
    }

    if (llevados == 0) {
        mensajeAviso("No ha llevado ningun curso.");
    }

    cout << "\n" << pintar("  Creditos aprobados: ", GRIS) << creditosAprobados(e, cursos, numCursos)
         << " de " << totalCreditosPensum(cursos, numCursos) << "\n";
}

// =====================================================================
// Submenu de la opcion 2
// =====================================================================

void menuCursosYNotas(Estudiante estudiantes[], int numEstudiantes,
                      const Curso cursos[], int numCursos) {
    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }

    // Referencia: los cambios se hacen directamente sobre estudiantes[pos]
    Estudiante& e = estudiantes[pos];
    int opcion;

    do {
        // Cada vuelta del submenu empieza en una pantalla limpia
        encabezado("Menu principal > Cursos y notas > " + e.nombre);

        int enCurso = 0;
        for (int c = 0; c < numCursos; c++) {
            if (e.estado[c] == EN_CURSO) {
                enCurso++;
            }
        }
        cout << pintar("  Carne: ", GRIS) << e.carne
             << pintar("    Creditos aprobados: ", GRIS) << creditosAprobados(e, cursos, numCursos)
             << pintar("    Cursos en curso: ", GRIS) << enCurso << "\n";

        grupoMenu("CICLOS ANTERIORES");
        opcionMenu(1, "Marcar curso aprobado (sin notas)");
        opcionMenu(2, "Marcar aprobado un ciclo completo");
        grupoMenu("SEMESTRE ACTUAL");
        opcionMenu(3, "Inscribir curso");
        opcionMenu(4, "Ingresar / actualizar puntos de un curso");
        grupoMenu("CONSULTA");
        opcionMenu(5, "Ver cursos del estudiante");
        opcionMenu(0, "Regresar al menu principal");
        cout << "\n";
        separador();
        opcion = leerEntero("  Seleccione: ", 0, 5);

        // La opcion elegida se abre en su propia pantalla limpia
        const string NOMBRES[6] = {
            "", "Marcar curso aprobado", "Marcar ciclo completo",
            "Inscribir curso", "Ingresar puntos", "Ver cursos"
        };
        if (opcion != 0) {
            encabezado("Menu principal > Cursos y notas > " + e.nombre + " > " + NOMBRES[opcion]);
        }

        switch (opcion) {
            case 1: opcionMarcarCurso(e, cursos, numCursos);     break;
            case 2: opcionMarcarCiclo(e, cursos, numCursos);     break;
            case 3: opcionInscribir(e, cursos, numCursos);       break;
            case 4: opcionIngresarPuntos(e, cursos, numCursos);  break;
            case 5: mostrarCursosEstudiante(e, cursos, numCursos); break;
            default: break;
        }

        if (opcion != 0) {
            pausar();   // deja leer el resultado antes de redibujar el submenu
        }
    } while (opcion != 0);
}
