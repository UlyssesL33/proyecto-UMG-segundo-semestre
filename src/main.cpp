// =====================================================================
// Control Academico - Ingenieria en Sistemas UMG
// Proyecto integrador: Algoritmos, Algebra Lineal y Matematica Discreta
// ---------------------------------------------------------------------
// Paso 1: esqueleto del menu principal.
// Cada opcion todavia muestra "En construccion"; se iran llenando
// paso a paso segun el roadmap del equipo.
// =====================================================================

#include <iostream>
#include <cstdlib>   // system()
#include <limits>    // numeric_limits, para limpiar el buffer de entrada

#include "config.h"   // constantes (NOTA_MINIMA, MAX_CURSOS, ...)
#include "modelos.h"  // struct Curso y struct Estudiante

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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    cout << "Seleccione una opcion: ";
}

int main() {
    int opcion;

    do {
        limpiarPantalla();
        mostrarMenu();

        cin >> opcion;

        // Si el usuario escribe letras, cin queda en estado de error y el
        // menu se repetiria para siempre. Esto lo evita por ahora; en el
        // paso 3 se reemplaza por la funcion leerEntero().
        if (cin.fail()) {
            cin.clear();
            opcion = -1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1:  cout << "\n[Registrar estudiante] En construccion.\n";       break;
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
            default: cout << "\nOpcion invalida. Intente de nuevo.\n";           break;
        }

        if (opcion != 0) {
            pausar();
        }

    } while (opcion != 0);

    return 0;
}
