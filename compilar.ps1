param([switch]$Ejecutar)

$ErrorActionPreference = 'Stop'

# Compila todos los modulos, incluso al invocarse desde src/.
$fuentes = @(Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot 'src') -Filter '*.cpp' -File |
    Sort-Object Name | Select-Object -ExpandProperty FullName)
$ejecutable = Join-Path $PSScriptRoot 'control.exe'

& g++ -std=c++11 -Wall -Wextra @fuentes -o $ejecutable
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Compilacion completa: $ejecutable"

if ($Ejecutar) {
    # Las rutas de datos del programa son relativas a la raiz.
    Push-Location -LiteralPath $PSScriptRoot
    try {
        & $ejecutable
        $resultado = $LASTEXITCODE
    } finally {
        Pop-Location
    }
    exit $resultado
}
