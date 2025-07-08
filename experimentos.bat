@echo off
setlocal enabledelayedexpansion

:: -----------------------------    
:: CONFIGURACIÓN
:: -----------------------------
set "EJECUTABLE=detectar_patrones.exe"
set "REPETICIONES=20"

:: Archivos CSV
set "CSV_ARCHIVOS=resultados_archivos_var.csv"
set "CSV_PATRONES=resultados_patrones_var.csv"

:: Listas manuales
set "CARPETA=Proteins"
set "PATRONES=patrones1_100_Existente patrones2_200_Existente patrones3_300_Existente patrones4_1000_Existente" 
:: patrones5_2000_Existente
set "ALGORITMOS=FM-Index KMP Robin-Karp Boyer-Moore"
set "CANTIDADES=10 20 30 40" 
:: 10 20 30 40
set "PATRON_FIJO=patrones1_100_Existente"
::  Cantidad
set "CANTIDAD_FIJA=20"
:: -----------------------------
:: ENCABEZADOS CSV
:: -----------------------------
echo Algoritmo;Archivos;Cantidad;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_ARCHIVOS%"
echo Algoritmo;Archivos;Cantidad;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_PATRONES%"

:: -----------------------------
:: BLOQUE 2 - Archivos variables con patrón fijo
:: -----------------------------
echo --------- Cantidad de Archivos variable ------------- > "tiempo_de_creacion.csv"
echo Algoritmo;Archivos;Cantidad;Tiempo_Promedio(ns) >> "tiempo_de_creacion.csv"

for /l %%I in (1,1,%REPETICIONES%) do (
    for %%C in (%CANTIDADES%) do (
        for %%A in (%ALGORITMOS%) do (
            for /f "delims=" %%R in ('cmd /c "%EJECUTABLE% %%A %PATRON_FIJO% -f "archivos_de_prueba\%CARPETA%" --patrones -cantidad=%%C "') do (
                set "RESULTADO=%%R"
                echo %%A, %%C, repeticion: %%I, !RESULTADO!
                echo %%A;%CARPETA%;%%C;%PATRON_FIJO%;!RESULTADO! >> "%CSV_ARCHIVOS%"
            )
        )
    )
)
:: -----------------------------
:: BLOQUE 1 - Patrones variables (20 archivos)
:: -----------------------------
echo --------- Cantidad de Patrones variable ------------- >> "tiempo_de_creacion.csv"
echo Algoritmo;Archivos;Cantidad;Tiempo_Promedio(ns) >> "tiempo_de_creacion.csv"

for %%P in (%PATRONES%) do (
    for %%A in (%ALGORITMOS%) do (
        for /f "delims=" %%R in ('cmd /c "%EJECUTABLE% %%A %%P -f .\archivos_de_prueba\%CARPETA% --patrones -repeticiones=%REPETICIONES% -cantidad=1 "') do (
            set "RESULTADO=%%R"
            echo %%A, 20, !RESULTADO!
            echo %%A;%CARPETA%;20;%%P;!RESULTADO! >> "%CSV_PATRONES%"
        )
    )
)

pause
