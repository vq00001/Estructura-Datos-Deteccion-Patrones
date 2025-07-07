EJECUTABLE=./generar_patrones.exe
TIPO=("Existente")
TAMANO=(100 200 300 1000 2000)


for tipo in "${TIPO[@]}"; do
	for tamano in "${TAMANO[@]}"; do
		$EJECUTABLE "$tamano" "$tipo" "patron_${tamano}_${tipo}.txt"
		echo "Generado patron_${tamano}_${tipo}.txt"
	done
done

