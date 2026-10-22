// =====================================================================
// Control Academico - Ingenieria en Sistemas UMG
// Proyecto integrador: Algoritmos, Algebra Lineal y Matematica Discreta
// ---------------------------------------------------------------------
// Menu principal. Las opciones que dicen "En construccion" se iran
// llenando paso a paso segun el roadmap del equipo.
// =====================================================================

#include <iostream>
#include <string>
#include <cstdlib>   // system()

#include "config.h"        // constantes (NOTA_MINIMA, MAX_CURSOS, ...)
#include "modelos.h"       // struct Curso y struct Estudiante
#include "validaciones.h"  // leerEntero, leerDecimal, leerTexto, leerCarne

using namespace std;

// Limpia la consola. Se usa solo desde aqui para que, si algun dia
// hay que cambiar el comando, se cambie en un unico lugar.
void limpiarPantalla() {
    system("cls");
}

// Detiene el programa hasta que el usuario presione Enter,
// para que alcance a leer el resultado antes de volver al menu.
void pausar() {
    cout << "\nPresione Enter para continuar...";
    string basura;
    getline(cin, basura);
}

// ---------------------------------------------------------------------
// TEMPORAL (paso 3): prueba de las validaciones desde la opcion 1.
// En el paso 5 se reemplaza por el registro real de estudiantes.
// ---------------------------------------------------------------------
void probarValidaciones() {
    cout << "\n--- Prueba de validaciones ---\n";
    string carne  = leerCarne("Carne (ej. 0900-26-9368): ");
    string nombre = leerTexto("Nombre completo: ");
    double p2     = leerDecimal("Puntos del Parcial 2 (0 a 20): ", 0, PUNTOS_MAXIMOS[PARCIAL_2]);

    cout << "\nDatos validos recibidos:\n";
    cout << "  Carne:     " << carne << "\n";
    cout << "  Carrera:   " << carne.substr(0, 4) << " - " << NOMBRE_CARRERA << "\n";
    cout << "  Nombre:    " << nombre << "\n";
    cout << "  Parcial 2: " << p2 << " / " << PUNTOS_MAXIMOS[PARCIAL_2] << "\n";
}

void mostrarMenu() {
    cout << "=============================================\n";
    cout << "   CONTROL ACADEMICO - ING. EN SISTEMAS UMG\n";
    cout << "=============================================\n";
    cout << " 1. Registrar estudiante\n";
    cout << " 2. Inscribir curso / ingresar notas\n";
    cout << " 3. Consultar estudiante (por carne)\n";
    cout << " 4. Listar estudiantes (global o por curso)\n";
    cout << " 5. Modificar estudiante\n";
    cout << " 6. Eliminar estudiante\n";
    cout << " 7. Estadisticas\n";
    cout << " 8. Analisis matricial        (Algebra)\n";
    cout << " 9. Arbol y cursos habilitados (Discreta)\n";
    cout << "10. Guardar informacion\n";
    cout << " 0. Salir\n";
    cout << "---------------------------------------------\n";
}

int main() {
    int opcion;

    do {
        limpiarPantalla();
        mostrarMenu();

        // leerEntero vuelve a preguntar si se escriben letras o un numero
        // fuera de 0-10, asi que aqui opcion siempre es valida.
        opcion = leerEntero("Seleccione una opcion: ", 0, 10);

        switch (opcion) {
            case 1:  probarValidaciones();                                       break;
            case 2:  cout << "\n[Inscribir curso / notas] En construccion.\n";    break;
            case 3:  cout << "\n[Consultar estudiante] En construccion.\n";       break;
            case 4:  cout << "\n[Listar estudiantes] En construccion.\n";         break;
            case 5:  cout << "\n[Modificar estudiante] En construccion.\n";       break;
            case 6:  cout << "\n[Eliminar estudiante] En construccion.\n";        break;
            case 7:  cout << "\n[Estadisticas] En construccion.\n";               break;
            case 8:  cout << "\n[Analisis matricial] En construccion.\n";         break;
            case 9:  cout << "\n[Arbol y cursos habilitados] En construccion.\n"; break;
            case 10: cout << "\n[Guardar informacion] En construccion.\n";        break;
            case 0:  cout << "\nSaliendo del programa. Hasta pronto.\n";          break;
            default: break;  // no deberia pasar: leerEntero ya valido 0-10
        }

        if (opcion != 0) {
            pausar();
        }

    } while (opcion != 0);

    return 0;
}
