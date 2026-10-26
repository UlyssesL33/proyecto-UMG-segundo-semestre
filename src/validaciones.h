// validaciones.h - Lectura segura de datos desde el teclado
// Todas las funciones leen una LINEA completa con getline y la revisan.
// Si el dato no es valido, muestran un mensaje y vuelven a preguntar,
// asi el programa nunca se rompe por un dato mal escrito.
#pragma once

#include <string>

// Quita espacios al inicio y al final: "  Ana  " -> "Ana"
std::string recortar(const std::string& texto);

// Pide un numero entero entre min y max (inclusive).
int leerEntero(const std::string& mensaje, int min, int max);

// Pide un numero con decimales entre min y max (inclusive).
// Acepta punto o coma decimal: 8.5 o 8,5
double leerDecimal(const std::string& mensaje, double min, double max);

// Pide un texto que no este vacio y que no contenga '|'
// ('|' es el separador de los archivos de datos).
std::string leerTexto(const std::string& mensaje);

// true si el carne tiene la forma DDDD-DD-D... (ej. 0900-26-9368).
// El correlativo final puede tener de 1 a 6 digitos.
bool formatoCarneValido(const std::string& carne);

// Pide un carne con formato valido y que sea de la carrera 0900.
std::string leerCarne(const std::string& mensaje);

// ---------- Utilidades para leer archivos ----------

// Parte una linea por el caracter '|' y guarda cada pedazo en campos[].
// Devuelve cuantos campos encontro, o maxCampos + 1 si sobran campos.
// "090008|Algoritmos|5" -> campos = {"090008", "Algoritmos", "5"} -> 3
int partirLinea(const std::string& linea, std::string campos[], int maxCampos);

// Convierten texto a numero. Devuelven false si el texto no es un numero.
bool textoAEntero(const std::string& texto, int& resultado);
bool textoADecimal(const std::string& texto, double& resultado);
