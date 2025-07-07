@echo off
setlocal enabledelayedexpansion

:: Configuración de variables
set "EJECUTABLE=detectar_patrones.exe"
set "CSV_ARCHIVOS=resultados_archivos_var.csv"
set "CSV_PATRONES=resultados_patrones_pat.csv"
set "REPETICIONES=2"

:: Listas simuladas (solo 1 valor por simplicidad, puedes expandirlas manualmente)
set "ALGORITMO=KMP"
set "PATRON=patronesV1"
set "CARPETA=Proteins"

:: Crear encabezados en los CSV
echo Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_ARCHIVOS%"
echo Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_PATRONES%"

:: Ejecutar el algoritmo
echo Ejecutando %ALGORITMO% con patrón %PATRON% en carpeta %CARPETA%
for /f "delims=" %%O in ('%EJECUTABLE% %ALGORITMO% %PATRON% -f "archivos_de_prueba\%CARPETA%" --patrones -repeticiones=%REPETICIONES% --cantidad=20') do (
    set "RESULTADO=%%O"
    echo !RESULTADO!
    echo %ALGORITMO%;%CARPETA%;%PATRON%;!RESULTADO!>> "%CSV_PATRONES%"
)

pause
