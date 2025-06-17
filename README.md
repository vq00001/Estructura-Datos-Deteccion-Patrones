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
`./detectar_patrones.exe <algoritmo> <Patron> -<flag> <NombreArchivoOCarpeta> <CantidadArchivos>`

Para analizar todos los archivos de una carpeta usar la flag `-f` y para archivos individuales usar `-a`.
Por default se procesan todos los archivos dentro de la carpeta a menos que se especifique una cantidad.
