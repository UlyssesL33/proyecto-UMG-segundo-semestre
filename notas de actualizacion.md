do { ... } while (opcion != 0) es el menú repetitivo que pide Algoritmos. 
Se usa do-while y no while porque el menú debe mostrarse al menos una vez.

switch (opcion): cada case es una opción del menú. 
Por ahora todas dicen “En construcción”. En los siguientes pasos cada una llamará a una función de otro archivo.

limpiarPantalla() y pausar() son funciones pequeñas para no repetir el mismo código en cada opción. 
pausar() evita que el menú tape el resultado antes de que alcancen a leerlo.
El bloque if (cin.fail()) Si alguien escribe “abc” donde va un número, cin queda en estado de error y, sin este bloque, el menú se repetiría para siempre. 

cin.clear() quita el error y cin.ignore(...) descarta lo que se escribió. En el paso 3 este bloque se reemplaza por una función leerEntero() reutilizable.
