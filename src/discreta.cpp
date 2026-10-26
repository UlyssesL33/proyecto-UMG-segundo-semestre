// discreta.cpp - Submenu de Matematica Discreta (opcion 9)
#include "discreta.h"
#include "pantalla.h"
#include "validaciones.h"
#include "estudiantes.h"
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
        opcionMenu(1, NOMBRES[1]);
        opcionMenu(2, NOMBRES[2]);
        grupoMenu("LOGICA PROPOSICIONAL");
        opcionMenu(3, NOMBRES[3]);
        grupoMenu("ARBOLES BINARIOS");
        for (int i = 4; i <= 6; i++) {
            opcionMenu(i, NOMBRES[i]);
        }
        opcionMenu(0, "Regresar al menu principal");
        cout << "\n";
        separador();
        opcion = leerEntero("  Seleccione: ", 0, 6);

        if (opcion == 0) {
            break;
        }

        encabezado(RUTA + " > " + NOMBRES[opcion]);

        int pos = -1;
        if (opcion >= 1 && opcion <= 4) {
            if (opcion == 2) {
                subtitulo("Primer estudiante (A1)");
            }
            pos = elegirEstudiante(estudiantes, numEstudiantes);
            if (pos == -1) {
                pausar();
                continue;
            }
            encabezado(RUTA + " > " + NOMBRES[opcion] + " > " + estudiantes[pos].nombre);
        }

        switch (opcion) {
            case 1:
                pantallaConjuntosEstudiante(estudiantes[pos], cursos, numCursos);
                break;
            case 2: {
                subtitulo("Segundo estudiante (A2)");
                int segundo = elegirEstudiante(estudiantes, numEstudiantes);
                if (segundo == -1) {
                    break;
                }
                if (segundo == pos) {
                    mensajeError("Elija dos estudiantes distintos.");
                    break;
                }
                encabezado(RUTA + " > " + NOMBRES[2]);
                pantallaCompararEstudiantes(estudiantes[pos], estudiantes[segundo], cursos, numCursos);
                break;
            }
            case 3:
                pantallaHabilitados(estudiantes[pos], cursos, numCursos);
                break;
            case 4:
                pantallaArbolEstudiante(estudiantes[pos], cursos, numCursos);
                break;
            case 5:
                pantallaBloqueos(cursos, numCursos);
                break;
            case 6:
                pantallaProfundidad(cursos, numCursos);
                break;
        }

        pausar();
    } while (opcion != 0);
}
