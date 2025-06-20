# --- Variables ---
# Compilador de C++
CXX = g++

# Flags del compilador: -std=c++17 para C++17, -Wall para mostrar todas las advertencias, -Wextra  para advertencias adicionales, -I. para incluir el directorio actual
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

# Flags del enlazador (linker): -lstdc++fs para la librería filesystem
LDFLAGS = -lstdc++fs

# Nombre del archivo ejecutable de salida
TARGET = detectar_patrones.exe

# --- Búsqueda de archivos ---
# Encuentra automáticamente todos los archivos .cpp en el directorio actual y en las carpetas 'algoritmos' y 'estructuras'
SOURCES = $(wildcard *.cpp algoritmos/*.cpp estructuras/*.cpp utils/*.cpp)

# Genera los nombres de los archivos objeto (.o) a partir de los archivos fuente (.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


# --- Reglas ---

# La regla por defecto que se ejecuta al escribir 'make'
all: $(TARGET)

# Regla para enlazar todos los archivos objeto (.o) y crear el ejecutable final
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Regla genérica para compilar un archivo .cpp en un archivo .o
# Compila cualquier .cpp encontrado en su correspondiente .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados (ejecutable y archivos objeto)
clean:
	rm -f $(TARGET) $(OBJECTS)

# Declara las reglas que no corresponden a nombres de archivos
.PHONY: all clean