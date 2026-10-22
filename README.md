# Control Academico - Ingenieria en Sistemas UMG

Programa en C++ presentado en Algoritmos, Algebra Lineal y Matematica Discreta.

## Estructura

```
src/      codigo fuente (.h y .cpp)
datos/    archivos de datos (pensum.txt, estudiantes.txt)
parametros/  lineamientos de cada clase (referencia)
```

## Compilar y ejecutar (MSYS2 / VS Code)

Desde la carpeta raiz del repositorio:

```
g++ src/*.cpp -o control
./control
```

Se ejecuta desde la raiz para que el programa encuentre `datos/pensum.txt`.

## Visual Studio Community

Crear un proyecto vacio de C++ y usar "Agregar elemento existente" con los
archivos de `src/`. No mover los archivos fuera de `src/`.
