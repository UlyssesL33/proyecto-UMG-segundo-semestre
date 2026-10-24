// =====================================================================
// discreta.cpp - Submenu de Matematica Discreta (opcion 9)
// =====================================================================
#include "discreta.h"
#include "pantalla.h"
#include "validaciones.h"   // leerEntero
#include "estudiantes.h"    // elegirEstudiante
#include "conjuntos.h"
#include "logica.h"
#include "arboles.h"

#include <iostream>
#include <string>

using namespace std;

void menuDiscreta(const Estudiante estudiantes[], int numEstudiantes,
                  const Curso cursos[], int numCursos) {
    const string RUTA = "Menu principal > Matematica Discreta";
    const string NOMBRES[7] = {
        "", "Conjuntos de un estudiante", "Comparar dos estudiantes",
        "Cursos habilitados (logica)", "Arbol binario de prerrequisitos",
        "Que bloquea reprobar un curso", "Cadena mas larga (altura)"
    };
    int opcion;

    do {
        encabezado(RUTA);

        grupoMenu("CONJUNTOS");
        opcionMenu(1, NOMBRES[1] + pintar("   (pendientes = pensum - aprobados)", GRIS));
        opcionMenu(2, NOMBRES[2] + pintar("     (interseccion, diferencia, union)", GRIS));
        grupoMenu("LOGICA PROPOSICIONAL");
        opcionMenu(3, NOMBRES[3] + pintar("  (p AND q AND r AND s)", GRIS));
        grupoMenu("ARBOLES BINARIOS");
        opcionMenu(4, NOMBRES[4] + pintar("      (recorrido recursivo)", GRIS));
        opcionMenu(5, NOMBRES[5] + pintar(" (subarbol)", GRIS));
        opcionMenu(6, NOMBRES[6] + pintar("     (altura del arbol)", GRIS));
        opcionMenu(0, "Regresar al menu principal");
        cout << "\n";
        separador();
        opcion = leerEntero("  Seleccione: ", 0, 6);

        if (opcion == 0) {
            break;
        }

        encabezado(RUTA + " > " + NOMBRES[opcion]);

        if (opcion == 1 || opcion == 3 || opcion == 4) {
            // Estas pantallas son de UN estudiante
            int pos = elegirEstudiante(estudiantes, numEstudiantes);
            if (pos != -1) {
                encabezado(RUTA + " > " + NOMBRES[opcion] + " > " + estudiantes[pos].nombre);
                if (opcion == 1) pantallaConjuntosEstudiante(estudiantes[pos], cursos, numCursos);
                if (opcion == 3) pantallaHabilitados(estudiantes[pos], cursos, numCursos);
                if (opcion == 4) pantallaArbolEstudiante(estudiantes[pos], cursos, numCursos);
            }
        } else if (opcion == 2) {
            subtitulo("Primer estudiante (A1)");
            int p1 = elegirEstudiante(estudiantes, numEstudiantes);
            if (p1 != -1) {
                subtitulo("Segundo estudiante (A2)");
                int p2 = elegirEstudiante(estudiantes, numEstudiantes);
                if (p2 == p1) {
                    mensajeError("Elija dos estudiantes distintos.");
                } else if (p2 != -1) {
                    encabezado(RUTA + " > " + NOMBRES[2]);
                    pantallaCompararEstudiantes(estudiantes[p1], estudiantes[p2], cursos, numCursos);
                }
            }
        } else if (opcion == 5) {
            pantallaBloqueos(cursos, numCursos);
        } else if (opcion == 6) {
            pantallaProfundidad(cursos, numCursos);
        }

        pausar();
    } while (opcion != 0);
}
