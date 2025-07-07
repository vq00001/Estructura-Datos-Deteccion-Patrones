@echo off
setlocal enabledelayedexpansion

:: -----------------------------    
:: CONFIGURACIÓN
:: -----------------------------
set "EJECUTABLE=detectar_patrones.exe"
set "REPETICIONES=1"


:: Archivos CSV
set "CSV_ARCHIVOS=resultados_archivos_var.csv"
set "CSV_PATRONES=resultados_patrones_var.csv"

:: Listas manuales
set "CARPETA=Proteins"
set "PATRONES=patrones1_100_Existente patrones2_200_Existente patrones3_300_Existente patrones4_1000_Existente" 
:: patrones5_2000_Existente
set "ALGORITMOS=FM-Index KMP Robin-Karp Boyer-Moore"
set "CANTIDADES=1" 
:: 10 20 30 40
set "PATRON_FIJO=patrones1_100_Existente"


:: -----------------------------
:: ENCABEZADOS CSV
:: -----------------------------
echo Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_ARCHIVOS%"
echo Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)> "%CSV_PATRONES%"

:: -----------------------------
:: BLOQUE 1 - Patrones variables (20 archivos)
:: -----------------------------

for %%P in (%PATRONES%) do (
    for %%A in (%ALGORITMOS%) do (
        echo Ejecutando %%A con patrón %%P en carpeta %CARPETA%
        for /f "delims=" %%R in ('%EJECUTABLE% %%A %%P -f ".\archivos_de_prueba\%CARPETA%" --patrones -repeticiones=%REPETICIONES% -cantidad=1') do (
            set "RESULTADO=%%R"
            echo !RESULTADO!
            echo %%A;%CARPETA%;%%P;!RESULTADO!>> "%CSV_PATRONES%"
        )
    )
)

:: -----------------------------
:: BLOQUE 2 - Archivos variables con patrón fijo
:: -----------------------------

for /l %%I in (1,1,%REPETICIONES%) do (
    for %%C in (%CANTIDADES%) do (
        for %%A in (%ALGORITMOS%) do (
            echo Ejecutando %%A con patrón fijo %PATRON_FIJO%, cantidad %%C, repetición %%I
            for /f "delims=" %%R in ('%EJECUTABLE% %%A %PATRON_FIJO% -f "archivos_de_prueba\%CARPETA%" --patrones -cantidad=%%C') do (
                set "RESULTADO=%%R"
                echo !RESULTADO!
                echo %%A;%CARPETA%;%PATRON_FIJO%;!RESULTADO!>> "%CSV_ARCHIVOS%"
            )
        )
    )
)


pause
