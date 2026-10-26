#include "algebra.h"
#include "archivos.h"
#include "config.h"
#include "discreta.h"
#include "estudiantes.h"
#include "notas.h"
#include "validaciones.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static int errores = 0;
static Estudiante estudiantes[MAX_ESTUDIANTES];
static Estudiante cargados[MAX_ESTUDIANTES];
static Curso cursos[] = {
    {"090008", "Algoritmos", 5, 2, SIN_PRERREQUISITO, 0},
    {"090012", "Programacion I", 5, 3, "090008", 0},
    {"090007", "Algebra Lineal", 4, 2, SIN_PRERREQUISITO, 0}
};
static istringstream entrada;
static ostringstream salida;
static streambuf* entradaOriginal;
static streambuf* salidaOriginal;

static void comprobar(bool condicion, const string& caso) {
    if (!condicion) {
        cerr << "FALLO: " << caso << "\n";
        errores++;
    }
}

static void comenzar(const string& texto) {
    entrada.clear();
    entrada.str(texto);
    salida.str("");
    salida.clear();
    cin.clear();
    entradaOriginal = cin.rdbuf(entrada.rdbuf());
    salidaOriginal = cout.rdbuf(salida.rdbuf());
}

static string terminar() {
    cin.rdbuf(entradaOriginal);
    cin.clear();
    cout.rdbuf(salidaOriginal);
    return salida.str();
}

static string cursosYNotas(const string& texto) {
    comenzar(texto);
    menuCursosYNotas(estudiantes, 2, cursos, 3);
    return terminar();
}

static string discreta(const string& texto, int cantidad = 2) {
    comenzar(texto);
    menuDiscreta(estudiantes, cantidad, cursos, 3);
    return terminar();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: flujos_basicos.exe ruta_archivo_temporal\n";
        return 2;
    }

    int entero;
    double decimal;
    comprobar(textoAEntero(" 12 ", entero) && entero == 12, "entero con espacios");
    comprobar(!textoAEntero("12abc", entero), "rechazar texto despues del entero");
    comprobar(!textoAEntero("1.5", entero), "rechazar decimal como entero");
    comprobar(!textoAEntero("999999999999999999999999", entero), "rechazar desbordamiento");
    comprobar(textoADecimal("12.5", decimal) && decimal == 12.5, "convertir decimal");
    comprobar(!textoADecimal("12.5x", decimal), "rechazar sobrantes en decimal");
    comenzar("abc\n12abc\n9\n2\n");
    entero = leerEntero("", 0, 3);
    terminar();
    comprobar(entero == 2, "volver a preguntar hasta recibir un entero valido");
    comenzar("8,5\n");
    decimal = leerDecimal("", 0, 10);
    terminar();
    comprobar(decimal == 8.5, "seguir aceptando coma decimal");

    string campos[4];
    campos[3] = "NO_CAMBIAR";
    comprobar(partirLinea(" A |B|", campos, 3) == 3 && campos[0] == "A" && campos[2].empty(),
              "conservar espacios recortados y ultimo campo vacio");
    comprobar(partirLinea("A|B|C|D", campos, 3) == 4 && campos[3] == "NO_CAMBIAR",
              "avisar exceso de campos sin escribir fuera del limite");
    comprobar(partirLinea("", campos, 3) == 1 && campos[0].empty(), "linea vacia");

    for (int i = 0; i < 2; i++) {
        inicializarEstudiante(estudiantes[i]);
        estudiantes[i].carne = "0900-26-" + to_string(i + 1);
        estudiantes[i].nombre = i == 0 ? "Ana" : "Luis";
        estudiantes[i].carrera = CODIGO_CARRERA;
    }
    comprobar(estudiantes[0].estado[MAX_CURSOS-1] == NO_CURSADO &&
              estudiantes[0].puntos[MAX_CURSOS-1][FINAL] == SIN_CALIFICAR,
              "inicializar tambien el ultimo curso del arreglo");

    cursosYNotas("1\n3\n1\n\n0\n");
    comprobar(estudiantes[0].estado[0] == EN_CURSO && !tieneNotas(estudiantes[0], 0), "inscribir sin notas");
    cursosYNotas("1\n4\n1\n-0.5\n10\n20\n30\n20\n\n0\n");
    comprobar(notaCurso(estudiantes[0], 0) == 80 && estudiantes[0].estado[0] == APROBADO,
              "ingresar notas y rechazar negativos distintos de -1");
    cursosYNotas("1\n1\n2\n1\n\n0\n");
    int conNota;
    comprobar(estudiantes[0].estado[2] == APROBADO && !tieneNotas(estudiantes[0], 2) &&
              promedioEstudiante(estudiantes[0], 3, conNota) == 80 && conNota == 1,
              "marcar aprobacion historica sin alterar el promedio");
    cursosYNotas("1\n5\n2\n1\n\n0\n");
    comprobar(estudiantes[0].estado[2] == NO_CURSADO && !tieneNotas(estudiantes[0], 2), "retirar curso");
    cursosYNotas("1\n3\n1\n\n0\n");
    string mensaje = cursosYNotas("1\n5\n1\n\n0\n");
    comprobar(estudiantes[0].estado[0] == APROBADO && mensaje.find("depende de este curso") != string::npos,
              "mantener la proteccion al retirar un prerrequisito");

    for (int k = 0; k < NUM_COMPONENTES; k++) estudiantes[1].puntos[0][k] = 5;
    actualizarEstado(estudiantes[1], 0);
    cursosYNotas("2\n3\n1\n\n0\n");
    comprobar(estudiantes[1].estado[0] == EN_CURSO && !tieneNotas(estudiantes[1], 0), "reinscribir borra el intento anterior");
    cursosYNotas("2\n2\n2\n\n0\n");
    comprobar(estudiantes[1].estado[0] == EN_CURSO && estudiantes[1].estado[2] == APROBADO,
              "marcar ciclo respeta los cursos ya inscritos");

    comprobar(discreta("1\n\n0\n", 0).find("No hay estudiantes") != string::npos, "discreta sin estudiantes");
    comprobar(discreta("1\n0\n\n0\n").find("Cancelar") != string::npos, "cancelar primer estudiante");
    comprobar(discreta("2\n1\n1\n\n0\n").find("distintos") != string::npos, "rechazar comparar al mismo estudiante");
    comprobar(discreta("2\n1\n0\n\n0\n").find("inclusion-exclusion") == string::npos, "cancelar segundo estudiante");
    comprobar(discreta("2\n1\n2\n\n0\n").find("inclusion-exclusion") != string::npos, "comparar dos estudiantes");
    comprobar(discreta("1\n1\n\n0\n").find("Conjuntos de Ana") != string::npos, "consultar conjuntos");
    comprobar(discreta("3\n1\n\n0\n").find("p AND q AND r AND s") != string::npos, "consultar habilitacion");
    comprobar(discreta("4\n1\n\n0\n").find("I: ") != string::npos, "dibujar arbol binario");
    comprobar(discreta("5\n0\n\n0\n").find("Subarbol de") == string::npos, "cancelar bloqueos");
    comprobar(discreta("5\n1\n\n0\n").find("Subarbol de Algoritmos") != string::npos, "consultar bloqueos");
    comprobar(discreta("6\n\n0\n").find("altura 2") != string::npos, "consultar cadena mas larga");

    comenzar("1\n2\n\n2\n2\n\n3\n1\n1\n\n0\n");
    menuAlgebra(estudiantes, 2, cursos, 3);
    mensaje = terminar();
    comprobar(mensaje.find("P^T") != string::npos && mensaje.find("Ponderado") != string::npos &&
              mensaje.find("Necesita 61") != string::npos, "recorrer las tres pantallas de algebra");

    comprobar(guardarEstudiantes(estudiantes, 2, cursos, 3, argv[1]), "guardar datos de prueba");
    comprobar(cargarEstudiantes(cargados, cursos, 3, argv[1]) == 2 &&
              cargados[0].estado[0] == APROBADO && notaCurso(cargados[0], 0) == 80 &&
              cargados[1].estado[2] == APROBADO && !tieneNotas(cargados[1], 2),
              "conservar notas e historicos al guardar y cargar");

    double matriz[MAX_FILAS][MAX_COLUMNAS] = {{1, 2}, {3, 4}};
    double transpuestaMatriz[MAX_COLUMNAS][MAX_FILAS];
    double vector[MAX_COLUMNAS] = {2, 3};
    double producto[MAX_FILAS];
    transpuesta(matriz, 2, 2, transpuestaMatriz);
    matrizPorVector(matriz, 2, 2, vector, producto);
    comprobar(transpuestaMatriz[0][1] == 3 && producto[0] == 8 && producto[1] == 18, "conservar calculos matriciales");

    cout << (errores == 0 ? "Todas las pruebas de flujos basicos pasaron.\n" : "Hay pruebas fallidas.\n");
    return errores == 0 ? 0 : 1;
}
