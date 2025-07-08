# Estructura-Datos-Deteccion-Patrones
Proyecto semestral para el ramo Estructuras de Datos.

---

## Compilar programa:
### Automáticamente:
1. **Para compilar el programa**, abre una terminal en la raíz del proyecto y ejecuta `make`.
2. **Para limpiar los archivos generados** usa:
`make clean`.
##### Requiere tener make y g++ instalados
### Manualmente:
`g++ main.cpp utils.cpp -Ialgoritmos -o detectar_patrones.exe`

---

## Usar programa:
`./detectar_patrones.exe <algoritmo> <Patron> -<flag> <NombreArchivoOCarpeta> <FlagsAdicionales>`

Para analizar todos los archivos de una carpeta usar la flag `-f` y para archivos individuales usar `-a`.
Por default se procesan todos los archivos dentro de la carpeta a menos que se especifique una cantidad.

Flags adicionales:
`-r` : Si se usa el programa tambien imprimirá la cantidad de encuentros por archivo (En caso de usar multiples patrones solo imprimirá los del ultimo patron por ahora).
`-cantidad=<numero>` : Para especificar una cantidad limitada de archivos que queremos concatenar de la carpeta especificada.
`--patrones` : Para especificar que el patron puesto es el nombre de un archivo con patrones en vez de un patrón en sí.
`-test`: Para comprobar que las posiciones calculadas coinciden con la ocurrencia del patrón.
`-repeticiones=<numero>`: Para repetir la consulta del patrón una cierta cantidad de veces y calcular el promedio y varianza.