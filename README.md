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

En PowerShell tambien se puede compilar y ejecutar todo con:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\compilar.ps1 -Ejecutar
```

Si la terminal esta dentro de `src/`, usar `..\compilar.ps1`.

En VS Code, abrir la carpeta raiz `proyecto-UMG-segundo-semestre`.
La configuracion local de Code Runner permite usar **Run Code** (`Ctrl+Alt+N`)
para compilar todos los `.cpp` y ejecutar desde la raiz. `Ctrl+Shift+B` solo
compila; la tarea **Compilar y ejecutar proyecto** tambien inicia el programa.
No compilar unicamente `main.cpp`: necesita enlazar los demas modulos.

## Visual Studio Community

Crear un proyecto vacio de C++ y usar "Agregar elemento existente" con los
archivos de `src/`. No mover los archivos fuera de `src/`.
