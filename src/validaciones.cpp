// =====================================================================
// validaciones.cpp - Implementacion de la lectura segura de datos
// =====================================================================
#include "validaciones.h"
#include "config.h"

#include <iostream>
#include <sstream>   // stringstream: convertir texto a numero

using namespace std;

// ---------------------------------------------------------------------
string recortar(const string& texto) {
    int inicio = 0;
    int fin = (int)texto.length() - 1;

    while (inicio <= fin && (texto[inicio] == ' ' || texto[inicio] == '\t' || texto[inicio] == '\r')) {
        inicio++;
    }
    while (fin >= inicio && (texto[fin] == ' ' || texto[fin] == '\t' || texto[fin] == '\r')) {
        fin--;
    }
    return texto.substr(inicio, fin - inicio + 1);
}

// ---------------------------------------------------------------------
// Idea: se lee la linea completa como texto y se intenta convertir.
// "12"    -> valido
// "abc"   -> no se puede convertir
// "12abc" -> se convierte el 12 pero sobra "abc" -> invalido
// ---------------------------------------------------------------------
int leerEntero(const string& mensaje, int min, int max) {
    string linea;
    while (true) {
        cout << mensaje;
        getline(cin, linea);

        stringstream ss(linea);
        int valor;
        char sobrante;

        bool esNumero = (ss >> valor) && !(ss >> sobrante);

        if (!esNumero) {
            cout << "  Error: debe ingresar un numero entero.\n";
        } else if (valor < min || valor > max) {
            cout << "  Error: el valor debe estar entre " << min << " y " << max << ".\n";
        } else {
            return valor;
        }
    }
}

// ---------------------------------------------------------------------
double leerDecimal(const string& mensaje, double min, double max) {
    string linea;
    while (true) {
        cout << mensaje;
        getline(cin, linea);

        // Permitir coma decimal: "8,5" se trata como "8.5"
        for (int i = 0; i < (int)linea.length(); i++) {
            if (linea[i] == ',') {
                linea[i] = '.';
            }
        }

        stringstream ss(linea);
        double valor;
        char sobrante;

        bool esNumero = (ss >> valor) && !(ss >> sobrante);

        if (!esNumero) {
            cout << "  Error: debe ingresar un numero (puede llevar decimales).\n";
        } else if (valor < min || valor > max) {
            cout << "  Error: el valor debe estar entre " << min << " y " << max << ".\n";
        } else {
            return valor;
        }
    }
}

// ---------------------------------------------------------------------
string leerTexto(const string& mensaje) {
    string linea;
    while (true) {
        cout << mensaje;
        getline(cin, linea);
        linea = recortar(linea);

        if (linea.empty()) {
            cout << "  Error: el dato no puede quedar vacio.\n";
        } else if (linea.find('|') != string::npos) {
            cout << "  Error: el caracter '|' no esta permitido.\n";
        } else {
            return linea;
        }
    }
}

// ---------------------------------------------------------------------
// Revisa el carne caracter por caracter:
//   posiciones 0-3 : digitos (carrera)
//   posicion   4   : '-'
//   posiciones 5-6 : digitos (anio)
//   posicion   7   : '-'
//   posicion   8.. : de 1 a 6 digitos (correlativo)
// ---------------------------------------------------------------------
bool formatoCarneValido(const string& carne) {
    int largo = (int)carne.length();

    // 8 caracteres fijos + correlativo de 1 a 6 digitos
    if (largo < 9 || largo > 14) {
        return false;
    }

    for (int i = 0; i < largo; i++) {
        char c = carne[i];
        if (i == 4 || i == 7) {
            if (c != '-') {
                return false;
            }
        } else {
            if (c < '0' || c > '9') {
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------
string leerCarne(const string& mensaje) {
    while (true) {
        string carne = leerTexto(mensaje);

        if (!formatoCarneValido(carne)) {
            cout << "  Error: formato invalido. Ejemplo: " << CODIGO_CARRERA << "-26-9368\n";
        } else if (carne.substr(0, 4) != CODIGO_CARRERA) {
            cout << "  Error: el carne debe ser de " << NOMBRE_CARRERA
                 << " (empieza con " << CODIGO_CARRERA << ").\n";
        } else {
            return carne;
        }
    }
}
