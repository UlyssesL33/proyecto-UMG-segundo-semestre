// validaciones.cpp - Implementacion de la lectura segura de datos
#include "validaciones.h"
#include "config.h"
#include "pantalla.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

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

// Convierte un numero a texto sin ceros de sobra: 20 -> "20", 12.5 -> "12.5"
static string numeroATexto(double valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

// Lee una linea del teclado. Si la entrada se termino (Ctrl+Z en Windows,
// Ctrl+D en Linux) ya no hay nada que leer: sin esta revision las
// funciones de abajo volverian a preguntar para siempre.
static void leerLinea(string& linea) {
    if (!getline(cin, linea)) {
        cout << "\n\n";
        mensajeAviso("Fin de la entrada. El programa se cierra.");
        exit(0);
    }
}

// Idea: se lee la linea completa como texto y se intenta convertir.
// "12"    -> valido
// "abc"   -> no se puede convertir
// "12abc" -> se convierte el 12 pero sobra "abc" -> invalido
int leerEntero(const string& mensaje, int min, int max) {
    string linea;
    while (true) {
        cout << mensaje;
        leerLinea(linea);

        int valor;
        if (!textoAEntero(linea, valor)) {
            mensajeError("Debe ingresar un numero entero.");
        } else if (valor < min || valor > max) {
            mensajeError("El valor debe estar entre " + numeroATexto(min) + " y " + numeroATexto(max) + ".");
        } else {
            return valor;
        }
    }
}

double leerDecimal(const string& mensaje, double min, double max) {
    string linea;
    while (true) {
        cout << mensaje;
        leerLinea(linea);

        // Permitir coma decimal: "8,5" se trata como "8.5"
        for (int i = 0; i < (int)linea.length(); i++) {
            if (linea[i] == ',') {
                linea[i] = '.';
            }
        }

        double valor;
        if (!textoADecimal(linea, valor)) {
            mensajeError("Debe ingresar un numero (puede llevar decimales).");
        } else if (valor < min || valor > max) {
            mensajeError("El valor debe estar entre " + numeroATexto(min) + " y " + numeroATexto(max) + ".");
        } else {
            return valor;
        }
    }
}

string leerTexto(const string& mensaje) {
    string linea;
    while (true) {
        cout << mensaje;
        leerLinea(linea);
        linea = recortar(linea);

        if (linea.empty()) {
            mensajeError("El dato no puede quedar vacio.");
        } else if (linea.find('|') != string::npos) {
            mensajeError("El caracter '|' no esta permitido.");
        } else {
            return linea;
        }
    }
}

// Revisa el carne caracter por caracter:
//   posiciones 0-3 : digitos (carrera)
//   posicion   4   : '-'
//   posiciones 5-6 : digitos (anio)
//   posicion   7   : '-'
//   posicion   8.. : de 1 a 6 digitos (correlativo)
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

string leerCarne(const string& mensaje) {
    while (true) {
        string carne = leerTexto(mensaje);

        if (!formatoCarneValido(carne)) {
            mensajeError("Formato invalido. Ejemplo: " + CODIGO_CARRERA + "-26-9368");
        } else if (carne.substr(0, 4) != CODIGO_CARRERA) {
            mensajeError("El carne debe ser de " + NOMBRE_CARRERA + " (empieza con " + CODIGO_CARRERA + ").");
        } else {
            return carne;
        }
    }
}

// Utilidades para leer archivos

// Separa los campos sin escribir fuera del arreglo.
int partirLinea(const string& linea, string campos[], int maxCampos) {
    int cantidad = 0;
    string campo;
    for (int i = 0; i <= (int)linea.length(); i++) {
        if (i == (int)linea.length() || linea[i] == '|') {
            if (cantidad == maxCampos) {
                return maxCampos + 1; // hay mas campos de los permitidos
            }
            campos[cantidad] = recortar(campo);
            cantidad++;
            campo = "";
        } else {
            campo += linea[i];
        }
    }
    return cantidad;
}

bool textoAEntero(const string& texto, int& resultado) {
    stringstream ss(texto);
    if (!(ss >> resultado)) {
        return false;
    }
    char sobrante;
    if (ss >> sobrante) {
        return false;
    }
    return true;
}

bool textoADecimal(const string& texto, double& resultado) {
    stringstream ss(texto);
    if (!(ss >> resultado)) {
        return false;
    }
    char sobrante;
    if (ss >> sobrante) {
        return false;
    }
    return true;
}
