main.cpp

do { ... } while (opcion != 0) es el menú repetitivo que pide Algoritmos. 
Se usa do-while y no while porque el menú debe mostrarse al menos una vez.

switch (opcion): cada case es una opción del menú. 
Por ahora todas dicen “En construcción”. En los siguientes pasos cada una llamará a una función de otro archivo.

limpiarPantalla() y pausar() son funciones pequeñas para no repetir el mismo código en cada opción. 
pausar() evita que el menú tape el resultado antes de que alcancen a leerlo.
El bloque if (cin.fail()) Si alguien escribe “abc” donde va un número, cin queda en estado de error y, sin este bloque, el menú se repetiría para siempre. 

cin.clear() quita el error y cin.ignore(...) descarta lo que se escribió. En el paso 3 este bloque se reemplaza por una función leerEntero() reutilizable.


config.h: todos los números fijos en un solo lugar
NOTA_MINIMA = 61. Si en la defensa piden 65, se cambia solo esta línea.
MAX_ESTUDIANTES = 100 y MAX_CURSOS = 50: el tamaño de los arreglos.
Componentes de la nota. PARCIAL_1 = 0, PARCIAL_2 = 1, ZONA = 2, FINAL = 3 son posiciones dentro de un arreglo. Así se escribe puntos[c][FINAL] en vez de puntos[c][3], que se lee mejor y es más difícil equivocarse. PUNTOS_MAXIMOS = {10, 20, 35, 35} va en el mismo orden, para validar cada componente.
SIN_CALIFICAR = -1. Un 0 es una nota real (sacó cero), así que hace falta otro valor para decir “todavía no hay nota”.
Estados por curso: NO_CURSADO, EN_CURSO, APROBADO, REPROBADO. Un curso de ciclos anteriores queda APROBADO con sus puntos en -1; así sabemos que no entra al promedio.
#pragma once evita errores si dos archivos incluyen el mismo .h.

modelos.h: los dos struct
Curso tiene un campo por cada columna de pensum.txt. El código es string y no int porque 090007 como número se convierte en 90007.
Estudiante: la idea clave son los arreglos paralelos. La posición c de estado[] y de puntos[][] se refiere al curso cursos[c] del pensum. Por ejemplo, si cursos[7] es Algoritmos, estado[7] dice cómo le fue al alumno en Algoritmos y puntos[7][FINAL] es su final. puntos es una matriz de 50 cursos × 4 componentes, y esa misma idea de matriz es la que luego usamos en Álgebra.
El promedio no es un campo. Se calcula cada vez, así nunca queda desactualizado al modificar una nota.

