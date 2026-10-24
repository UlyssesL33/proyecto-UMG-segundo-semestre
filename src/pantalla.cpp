// =====================================================================
// pantalla.cpp - Aspecto visual de la consola
// =====================================================================
#include "pantalla.h"
#include "config.h"

#include <iostream>
#include <cstdlib>   // system()
#include <sstream>   // stringstream
#include <iomanip>   // fixed, setprecision

#ifdef _WIN32
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <windows.h>   // solo para activar los colores en Windows
#endif

using namespace std;

// Ancho de las lineas del encabezado y separadores
const int ANCHO = 64;

// ---------------------------------------------------------------------
// Codigos ANSI de cada color. "\033" es el caracter ESC; la consola lo
// interpreta como "lo que sigue es una instruccion, no texto".
// ---------------------------------------------------------------------
static const char* codigoColor(Color color) {
    switch (color) {
        case VERDE:    return "\033[32m";
        case ROJO:     return "\033[31m";
        case AMARILLO: return "\033[33m";
        case CIAN:     return "\033[36m";
        case GRIS:     return "\033[90m";
    }
    return "";
}

static const char* const RESTABLECER = "\033[0m";   // vuelve al color normal

// ---------------------------------------------------------------------
// En Windows la consola clasica no interpreta los codigos ANSI hasta
// que se activa el modo "virtual terminal". En otros sistemas no hace
// falta nada.
// ---------------------------------------------------------------------
void iniciarConsola() {
#ifdef _WIN32
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(salida, &modo)) {
        SetConsoleMode(salida, modo | 0x0004);   // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    }
#endif
}

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\n" << pintar("  Presione Enter para continuar...", GRIS);
    string basura;
    getline(cin, basura);
}

string pintar(const string& texto, Color color) {
    if (!USAR_COLORES) {
        return texto;
    }
    return codigoColor(color) + texto + RESTABLECER;
}

string columna(const string& texto, int ancho) {
    if (ancho <= 0) {
        return texto;         // sin relleno
    }
    if ((int)texto.length() >= ancho) {
        return texto + " ";   // siempre deja al menos un espacio de separacion
    }
    return texto + string(ancho - texto.length(), ' ');
}

string textoDecimal(double valor, int decimales) {
    stringstream ss;
    ss << fixed << setprecision(decimales) << valor;
    return ss.str();
}

// ---------------------------------------------------------------------
// Ejemplo de lo que dibuja:
//
// ================================================================
//   CONTROL ACADEMICO  |  Ingenieria en Sistemas UMG
// ================================================================
//   > Cursos y notas > Ulises Lopez
// ----------------------------------------------------------------
// ---------------------------------------------------------------------
void encabezado(const string& seccion) {
    limpiarPantalla();
    string doble(ANCHO, '=');

    cout << pintar(doble, CIAN) << "\n";
    cout << "  CONTROL ACADEMICO  " << pintar("|", GRIS) << "  " << NOMBRE_CARRERA << " UMG\n";
    cout << pintar(doble, CIAN) << "\n";
    cout << pintar("  > ", CIAN) << seccion << "\n";
    separador();
}

void subtitulo(const string& texto) {
    cout << "\n" << pintar("  " + texto, CIAN) << "\n";
}

void opcionMenu(int numero, const string& texto) {
    string num = to_string(numero);
    if (numero < 10) {
        num = " " + num;   // alinea [ 1] con [10]
    }
    cout << "  " << pintar("[" + num + "]", CIAN) << " " << texto << "\n";
}

void grupoMenu(const string& texto) {
    cout << "\n" << pintar("  " + texto, GRIS) << "\n";
}

void separador() {
    cout << pintar(string(ANCHO, '-'), GRIS) << "\n";
}

void concepto(const string& titulo) {
    cout << "\n  " << pintar("+-- CONCEPTO: ", GRIS) << pintar(titulo, CIAN) << "\n";
}

void lineaConcepto(const string& texto) {
    cout << "  " << pintar("|  ", GRIS) << texto << "\n";
}

void mensajeExito(const string& texto) {
    cout << "  " << pintar("[OK] " + texto, VERDE) << "\n";
}

void mensajeError(const string& texto) {
    cout << "  " << pintar("[X] " + texto, ROJO) << "\n";
}

void mensajeAviso(const string& texto) {
    cout << "  " << pintar("[!] " + texto, AMARILLO) << "\n";
}
