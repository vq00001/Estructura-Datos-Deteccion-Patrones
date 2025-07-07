#!/bin/bash

EJECUTABLE=./detectar_patrones.exe
CARPETAS=("Proteins")
CANT_ARCHIVOS=(10 ) # 20 30 40
PATRONES=("patrones100Existente" "patrones200Existente" "patrones3_300_Existente" "patrones4_1000_Existente" "patrones5_2000_Existente")
ALGORITMOS=("KMP" "Robin-Karp" "Boyer-Moore" "FM-Index")
REPETICIONES=2


CSV_ARCHIVOS="resultados_archivos_var.csv" # para archivos variables
CSV_PATRONES="resultados_patrones_var.csv" # para patrones variables

echo "Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)" > "$CSV_ARCHIVOS"
echo "Algoritmo;Archivos;Patrones;Tiempo_Promedio(ns);Varianza(ns)" > "$CSV_PATRONES"


## Ejecutar para 20 archivos con patrones variables
for patron in "${PATRONES[@]}"; do
	for carpeta in "${CARPETAS[@]}"; do
		# Seleccionar 20 archivos de la carpeta que contenga el patrón
		for algoritmo in "${ALGORITMOS[@]}"; do
			# Ejecutar el algoritmo con el patrón y la carpeta especificada
			promedio_varianza=$($EJECUTABLE "$algoritmo" "$patron" "-f" "./archivos_de_prueba/$carpeta" "--patrones" "-repeticiones=$REPETICIONES" "--cantidad=20")

			echo "$algoritmo;${carpeta};$patron;$promedio_varianza" >> "$CSV_PATRONES"
	    done
	done
done

## Ejecutar para cantidad de archivos variable 
PATRON_FIJO="patrones1_1000_Existente" # patrón fijo para los archivos

for rep in "${REPETICIONES[@]}"; do
	for cant in "${CANT_ARCHIVOS[@]}"; do
		for algoritmo in "${ALGORITMOS[@]}"; do
			promedio_varianza=$($EJECUTABLE "$algoritmo" "$PATRON_FIJO" -f "./archivos_de_prueba/$carpeta" --patrones -cantidad=$cant)
			echo "$algoritmo;$archivo;$PATRON_FIJO;$promedio_varianza" >> "$CSV_ARCHIVOS"
		done
	done
done
