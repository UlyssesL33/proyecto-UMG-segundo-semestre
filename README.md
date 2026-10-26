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

## Arboles binarios de prerrequisitos

El pensum se representa como un bosque de arboles binarios: cada curso
admite hasta dos hijos (izquierdo y derecho, segun el orden del archivo).
La carga rechaza un tercer hijo, ciclos, codigos repetidos y prerrequisitos
inexistentes. Los enlaces representan prerrequisitos, no un orden de busqueda.

## Visual Studio Community

Crear un proyecto vacio de C++ y usar "Agregar elemento existente" con los
archivos de `src/`. No mover los archivos fuera de `src/`.

## Como seguir el codigo

1. Empezar en `main.cpp`: cada opcion del `switch` llama a un modulo.
2. Leer `modelos.h` y `config.h` para entender los datos y sus limites.
3. Seguir `registrarEstudiante`, `menuCursosYNotas` y `guardarEstudiantes`.
4. Revisar conjuntos, logica, arboles y algebra por separado.

Los menus usan el mismo flujo: mostrar opciones, leer, ejecutar y pausar.
`reiniciarCurso` cambia el estado y limpia las notas en un solo lugar.
En los arboles, `nivel` controla la sangria y `I`/`D` indican cada rama.
Las formulas matematicas se explican dentro de sus pantallas.

## Pruebas (PowerShell, desde la raiz)

```powershell
New-Item -ItemType Directory -Force build/pruebas | Out-Null
$modulos = Get-ChildItem src/*.cpp | Where-Object Name -ne 'main.cpp' | Select-Object -ExpandProperty FullName
g++ -std=c++11 -Wall -Wextra -I src tests/arboles_binarios.cpp $modulos -o build/pruebas/arboles.exe
./build/pruebas/arboles.exe build/pruebas/pensum_prueba.txt
g++ -std=c++11 -Wall -Wextra -I src tests/flujos_basicos.cpp $modulos -o build/pruebas/flujos.exe
./build/pruebas/flujos.exe build/pruebas/estudiantes_prueba.txt
```

Los archivos de prueba se escriben en `build/pruebas`, separados de los datos reales.
