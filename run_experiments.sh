#!/bin/bash

EJECUTABLE=./detectar_patrones.exe
ARCHIVOS=("DNA" "English")
PATRONES=("patronesV1" "patronesV2")
ALGORITMOS=("KMP" "Robin-Karp" "Boyer-Moore")
REPETICIONES=1
CSV="resultados.csv"

echo "Algoritmo;Archivo;Patrones;Tiempo(ns)" > "$CSV"

for patron in "${PATRONES[@]}"; do
	for archivo in "${ARCHIVOS[@]}"; do
	    for algoritmo in "${ALGORITMOS[@]}"; do
	        for ((i = 1; i <= REPETICIONES; i++)); do
	            tiempo=$($EJECUTABLE "$algoritmo" "$patron" "-f" "archivos_de_prueba/$archivo" "--patrones")
	            echo "$algoritmo;$archivo;$patron;$tiempo" >> "$CSV"
	        done
	    done
	done
done