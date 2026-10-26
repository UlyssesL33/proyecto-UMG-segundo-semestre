// pantalla.h - Aspecto visual de la consola (estilo TUI sencillo)
// Todo lo visual pasa por aqui: limpiar pantalla, encabezados, colores
// y mensajes. Asi el resto del programa no repite codigos de color, y si
// una consola no muestra bien los colores se apagan con USAR_COLORES
// en config.h, en un solo lugar.
//
// Los colores usan secuencias ANSI ("\033[32m" = verde). Funcionan en la
// terminal de VS Code, Windows Terminal y la consola de Windows 10/11.
#pragma once

#include <string>

enum Color {
    VERDE,      // exito, aprobado
    ROJO,       // error, reprobado
    AMARILLO,   // aviso, en curso
    CIAN,       // acento: titulos y numeros de opcion
    GRIS        // texto secundario
};

// Prepara la consola para mostrar colores (necesario en Windows).
// Se llama una sola vez, al inicio de main.
void iniciarConsola();

void limpiarPantalla();

// Espera Enter para que el usuario alcance a leer antes de continuar.
void pausar();

// Devuelve el texto envuelto en el codigo del color (o sin color si
// USAR_COLORES es false). Ojo: no usar con setw, porque los codigos de
// color cuentan como caracteres y desalinean las columnas.
std::string pintar(const std::string& texto, Color color);

// Rellena el texto con espacios a la derecha hasta "ancho" caracteres.
// Hace lo mismo que setw, pero devuelve un string que despues se puede
// pintar sin desalinear la tabla:  pintar(columna("Aprobado", 10), VERDE)
std::string columna(const std::string& texto, int ancho);

// Numero con una cantidad fija de decimales: textoDecimal(61, 2) -> "61.00"
std::string textoDecimal(double valor, int decimales);

// Limpia la pantalla y dibuja el encabezado del programa con la ruta
// de la seccion actual. Ej: encabezado("Cursos y notas > Ulises Lopez")
void encabezado(const std::string& seccion);

// Subtitulo dentro de una pantalla, en color de acento.
void subtitulo(const std::string& texto);

// Una opcion de menu: "  [1] Registrar estudiante" con el numero en color.
void opcionMenu(int numero, const std::string& texto);

// Nombre de un grupo de opciones dentro de un menu (en gris).
void grupoMenu(const std::string& texto);

// Linea separadora.
void separador();

// Recuadro breve que explica el concepto matematico que se esta usando:
//   concepto("Diferencia de conjuntos");
//   lineaConcepto("P = S - A");
//   lineaConcepto("Cursos del pensum que NO estan en Aprobados.");
void concepto(const std::string& titulo);
void lineaConcepto(const std::string& texto);

// Mensajes con su simbolo y color.
void mensajeExito(const std::string& texto);   // [OK] verde
void mensajeError(const std::string& texto);   // [X]  rojo
void mensajeAviso(const std::string& texto);   // [!]  amarillo
