// notas.cpp - Cursos de un estudiante y sus notas
#include "notas.h"
#include "config.h"
#include "validaciones.h"
#include "cursos.h"
#include "estudiantes.h"
#include "pantalla.h"
#include "logica.h"
#include "arboles.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Calculos

void reiniciarCurso(Estudiante& e, int curso, int estado) {
    e.estado[curso] = estado;
    for (int k = 0; k < NUM_COMPONENTES; k++) {
        e.puntos[curso][k] = SIN_CALIFICAR;
    }
}

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

// Ejemplo: Algoritmos 85 y Matematica Discreta 37 (completos), Algebra
// Lineal en curso con 8 puntos.  ->  (85 + 37) / 2 = 61, cursosConNota = 2
// Algebra no entra porque todavia le faltan componentes.
double promedioEstudiante(const Estudiante& e, int numCursos, int& cursosConNota) {
    double suma = 0;
    cursosConNota = 0;

    for (int c = 0; c < numCursos; c++) {
        if (notasCompletas(e, c)) {
            suma += notaCurso(e, c);
            cursosConNota++;
        }
    }

    if (cursosConNota == 0) {
        return 0;   // no hay cursos con nota: no se puede dividir entre 0
    }
    return suma / cursosConNota;
}

string promedioConColor(double promedio, int cursosConNota, int ancho) {
    if (cursosConNota == 0) {
        return pintar(columna("-", ancho), GRIS);
    }
    string texto = columna(textoDecimal(promedio, 2), ancho);
    return pintar(texto, promedio >= NOTA_MINIMA ? VERDE : ROJO);
}

int contarCursosEnEstado(const Estudiante& e, int numCursos, int estado) {
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] == estado) {
            cantidad++;
        }
    }
    return cantidad;
}

const char* textoEstado(int estado) {
    switch (estado) {
        case EN_CURSO:  return "En curso";
        case APROBADO:  return "Aprobado";
        case REPROBADO: return "Reprobado";
        default:        return "No cursado";
    }
}

// Funciones auxiliares de este archivo

// Convierte unos puntos a texto: -1 -> "-", 12.5 -> "12.5", 10 -> "10"
string textoPuntos(double valor) {
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

// Llena opciones[] con los cursos HABILITADOS de un ciclo.
// "ocultos" devuelve cuantos cursos pendientes del ciclo no se muestran
// porque todavia no cumplen la regla (les falta prerrequisito o creditos).
static int cursosHabilitadosDelCiclo(const Estudiante& e, const Curso cursos[], int numCursos,
                                     int ciclo, int opciones[], int& ocultos) {
    int cantidad = 0;
    ocultos = 0;
    for (int c = 0; c < numCursos; c++) {
        if (cursos[c].ciclo != ciclo) {
            continue;
        }
        bool pendiente = (e.estado[c] == NO_CURSADO || e.estado[c] == REPROBADO);
        if (!pendiente) {
            continue;
        }
        if (estaHabilitado(e, cursos, numCursos, c)) {
            opciones[cantidad] = c;
            cantidad++;
        } else {
            ocultos++;
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

// marcar cursos aprobados de ciclos anteriores (sin notas)

static void opcionMarcarCurso(Estudiante& e, const Curso cursos[], int numCursos) {
    int ciclo = elegirCiclo(cursos, numCursos);
    if (ciclo == 0) {
        return;
    }

    int opciones[MAX_CURSOS];
    int ocultos = 0;
    int cantidad = cursosHabilitadosDelCiclo(e, cursos, numCursos, ciclo, opciones, ocultos);
    if (ocultos > 0) {
        mensajeAviso(to_string(ocultos) + " curso(s) del ciclo no aparecen: aun no estan habilitados.");
        cout << pintar("  Marque los ciclos en orden (1, 2, 3...) para cumplir los prerrequisitos.", GRIS) << "\n";
    }
    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos habilitados del ciclo");
    if (curso == -1) {
        return;
    }

    reiniciarCurso(e, curso, APROBADO);
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
        if (cursos[c].ciclo != ciclo) {
            continue;
        }
        bool pendiente = (e.estado[c] == NO_CURSADO || e.estado[c] == REPROBADO);
        if (!pendiente) {
            continue;   // aprobados o en curso no se tocan
        }
        // solo se aprueba lo que esta habilitado
        if (!estaHabilitado(e, cursos, numCursos, c)) {
            mensajeAviso(cursos[c].codigo + "  " + cursos[c].nombre + "  (no habilitado: falta prerrequisito o creditos)");
            continue;
        }
        reiniciarCurso(e, c, APROBADO);
        mensajeExito(cursos[c].codigo + "  " + cursos[c].nombre);
        marcados++;
    }
    cout << "\n" << pintar("  Cursos marcados: ", GRIS) << marcados
         << pintar("  (los aprobados o en curso no se tocan)", GRIS) << "\n";
}

// inscribir un curso

// La misma regla de logica determina que cursos se pueden inscribir.
static void opcionInscribir(Estudiante& e, const Curso cursos[], int numCursos) {
    int opciones[MAX_CURSOS];
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (estaHabilitado(e, cursos, numCursos, c)) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }

    mensajeAviso("Se muestran los cursos que cumplen los requisitos de inscripcion.");

    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos que puede inscribir");
    if (curso == -1) {
        return;
    }

    if (e.estado[curso] == REPROBADO) {
        mensajeAviso("El curso estaba reprobado: se inscribe de nuevo y sus notas anteriores se borran.");
    }

    // Se guarda solo el intento mas reciente: notas en blanco
    reiniciarCurso(e, curso, EN_CURSO);
    cout << "\n";
    mensajeExito("Inscrito en " + cursos[curso].nombre + ".");
}

// Quitar un curso: retirar uno en curso, o deshacer un aprobado o
// reprobado registrado por error. El curso vuelve a "no cursado".
//
// No se permite si dejaria datos incoherentes:
//   - otro curso que depende de este (un hijo en el arbol) ya fue llevado
//   - otro curso llevado pide un minimo de creditos que ya no se cumpliria

static void opcionQuitarCurso(Estudiante& e, const Curso cursos[], int numCursos) {
    int opciones[MAX_CURSOS];
    int cantidad = 0;
    for (int c = 0; c < numCursos; c++) {
        if (e.estado[c] != NO_CURSADO) {
            opciones[cantidad] = c;
            cantidad++;
        }
    }

    int curso = elegirCurso(cursos, opciones, cantidad, "Cursos del estudiante");
    if (curso == -1) {
        return;
    }

    // 1) Cursos que dependen directamente de este y ya fueron llevados
    int hijos[MAX_HIJOS_BINARIOS];
    int n = hijosDe(cursos, numCursos, curso, hijos);
    for (int i = 0; i < n; i++) {
        if (e.estado[hijos[i]] != NO_CURSADO) {
            mensajeError("No se puede quitar: " + cursos[hijos[i]].nombre + " ("
                         + textoEstado(e.estado[hijos[i]]) + ") depende de este curso.");
            mensajeAviso("Quite primero ese curso.");
            return;
        }
    }

    // 2) Si era aprobado, se pierden sus creditos: revisar minimos
    if (e.estado[curso] == APROBADO) {
        int creditosDespues = creditosAprobados(e, cursos, numCursos) - cursos[curso].creditos;
        for (int c = 0; c < numCursos; c++) {
            if (c != curso && e.estado[c] != NO_CURSADO && cursos[c].creditosMinimos > creditosDespues) {
                mensajeError("No se puede quitar: " + cursos[c].nombre + " pide "
                             + to_string(cursos[c].creditosMinimos) + " creditos y quedarian "
                             + to_string(creditosDespues) + ".");
                return;
            }
        }
    }

    mensajeAviso(cursos[curso].nombre + " (" + textoEstado(e.estado[curso])
                 + ") pasara a no cursado y se borraran sus notas.");
    if (leerEntero("  Confirma? (1 = Si, 0 = No): ", 0, 1) == 0) {
        mensajeAviso("Cancelado.");
        return;
    }

    reiniciarCurso(e, curso, NO_CURSADO);
    mensajeExito(cursos[curso].nombre + " quitado.");
}

// ingresar los puntos de un curso

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

// Pantalla con los cursos del estudiante

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

}

// Submenu de la opcion 2

void menuCursosYNotas(Estudiante estudiantes[], int numEstudiantes,
                      const Curso cursos[], int numCursos) {
    int pos = elegirEstudiante(estudiantes, numEstudiantes);
    if (pos == -1) {
        return;
    }

    // Referencia: los cambios se hacen directamente sobre estudiantes[pos]
    Estudiante& e = estudiantes[pos];
    const string ruta = "Cursos y notas > " + e.nombre;
    const string nombres[7] = {
        "Regresar al menu principal", "Marcar curso aprobado (sin notas)",
        "Marcar aprobado un ciclo completo", "Inscribir curso",
        "Ingresar / actualizar puntos de un curso", "Quitar curso (retirar o deshacer)",
        "Ver cursos del estudiante"
    };
    int opcion;

    do {
        // Cada vuelta del submenu empieza en una pantalla limpia
        encabezado(ruta);

        int enCurso = contarCursosEnEstado(e, numCursos, EN_CURSO);
        cout << pintar("  Carne: ", GRIS) << e.carne
             << pintar("    Creditos aprobados: ", GRIS) << creditosAprobados(e, cursos, numCursos)
             << pintar("    Cursos en curso: ", GRIS) << enCurso << "\n";

        grupoMenu("CICLOS ANTERIORES");
        opcionMenu(1, nombres[1]);
        opcionMenu(2, nombres[2]);
        grupoMenu("SEMESTRE ACTUAL");
        for (int i = 3; i <= 5; i++) {
            opcionMenu(i, nombres[i]);
        }
        grupoMenu("CONSULTA");
        opcionMenu(6, nombres[6]);
        opcionMenu(0, nombres[0]);
        cout << "\n";
        separador();
        opcion = leerEntero("  Seleccione: ", 0, 6);

        // La opcion elegida se abre en su propia pantalla limpia
        if (opcion == 0) {
            break;
        }
        encabezado(ruta + " > " + nombres[opcion]);

        switch (opcion) {
            case 1: opcionMarcarCurso(e, cursos, numCursos);     break;
            case 2: opcionMarcarCiclo(e, cursos, numCursos);     break;
            case 3: opcionInscribir(e, cursos, numCursos);       break;
            case 4: opcionIngresarPuntos(e, cursos, numCursos);  break;
            case 5: opcionQuitarCurso(e, cursos, numCursos);     break;
            case 6: mostrarCursosEstudiante(e, cursos, numCursos); break;
            default: break;
        }

        pausar();
    } while (opcion != 0);
}
