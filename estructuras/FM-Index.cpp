#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

class FMIndex {
private:
    // Atributos necesarios de FMIndex

    string bwt;                         // Burrows-Wheeler Transform
    vector<int> ctable;                 // Tabla C (conteo de caracteres)
    vector<vector<int>> occ_table;      // Tabla OCC (conteo de ocurrencias)
    vector<int> suffix_array;           // Tabla de sufijos

    // Métodos privados necesarios para la implementación de FMIndex

    /* 
    @brief Método necesario para asegurar que el texto tenga un centinela al final,
    lo cual es necesario para que la BWT funcione.
    */
    string prepareTextForBWT(const string& text) {
        if (text.empty()) return "$"; // Si el texto está vacío, devuelve solo el centinela
        else if (text.back() == '$') return text; // Si ya tiene un centinela, no lo añade de nuevo
        return text + '$'; // El centinela '$' indica el final del texto
    }
    
    /*
    @brief Método que construye la Burrows-Wheeler Transform (BWT) de un texto dado.
    */
    void buildBWT(const string& input) {
        string text = prepareTextForBWT(input); // Prepara el texto añadiendo un centinela
        int n = text.size();
    
        // Paso 1: Crear un array de sufijos que contenga los índices [0, 1, 2, ..., n-1]
        vector<int> suffix_array(n);
        iota(suffix_array.begin(), suffix_array.end(), 0);
    
        // Paso 2: Ordenar los índices basándose en la rotación cíclica que representan. 
        sort(suffix_array.begin(), suffix_array.end(),
             [&](int a, int b) {
                 // Compara las rotaciones que empiezan en 'a' y 'b'
                 for (int i = 0; i < n; ++i) {
                     char char_a = text[(a + i) % n];
                     char char_b = text[(b + i) % n];
                     if (char_a != char_b) {
                         return char_a < char_b;
                     }
                 }
                 return false; // Son idénticas
             });
    
        // Paso 3: Construir la BWT. El carácter de la BWT es el que precede al inicio de cada rotación en el texto original.
        string bwt;
        bwt.reserve(n); // Pre-reservar memoria para eficiencia
        for (int i = 0; i < n; ++i) {
            // El carácter es el que está justo antes del sufijo (con ajuste cíclico)
            int char_index = (suffix_array[i] - 1 + n) % n;
            bwt += text[char_index];
        }
    }

    /*
    @brief Método que construye la tabla C (conteo de caracteres) a partir de la BWT.
    */
    void buildCTable() {
        ctable.assign(256, 0); // Carácteres ASCII
    
        // 1. Contar la frecuencia de cada carácter en la BWT
        for (char c : bwt) {
            ctable[static_cast<unsigned char>(c)]++;
        }
    
        // 2. Calcular las sumas acumuladas para obtener la tabla C correcta 
        // C[c] debe ser el número de caracteres < c
        int total_count = 0;
        for (int i = 0; i < 256; ++i) {
            int current_char_count = ctable[i];
            ctable[i] = total_count;
            total_count += current_char_count;
        }
    }

    /*
    @brief Método que construye la tabla OCC (conteo de ocurrencias) a partir de la BWT.
    */
    void buildOccTable() {
        // Construye la tabla OCC (conteo de ocurrencias)
        int n = bwt.size();
        occ_table.assign(256, vector<int>(n + 1, 0)); // Carácteres ASCII
    
        for (int i = 0; i < n; ++i) {
            char c = bwt[i];
            for (int j = 0; j < 256; ++j) {
                occ_table[j][i + 1] = occ_table[j][i]; // Copiar el valor anterior
            }
            occ_table[c][i + 1]++; // Incrementar el conteo para el carácter actual
        }
    }

    /*
    @brief Método que realiza la búsqueda hacia atrás en la BWT para encontrar el intervalo
    */
    pair<int, int> backwardSearch(const string& pattern) {
        int top = 0; // Inicio del intervalo
        int bottom = bwt.size() - 1; // Fin del intervalo
    
        for (int i = pattern.size() - 1; i >= 0; --i) {
            char c = pattern[i];
            if (ctable[static_cast<unsigned char>(c)] == 0) {
                return {-1, -1}; // El carácter no está en la BWT
            }
    
            // Actualizar el intervalo [top, bottom]
            top = ctable[static_cast<unsigned char>(c)] + occ_table[static_cast<unsigned char>(c)][top];
            bottom = ctable[static_cast<unsigned char>(c)] + occ_table[static_cast<unsigned char>(c)][bottom + 1] - 1;
    
            if (top > bottom) {
                return {-1, -1}; // No se encontró el patrón
            }
        }
    
        return {top, bottom}; // Retorna el intervalo donde se encuentra el patrón
    }

    public:
    
    /*
    Constructor de la clase FMIndex que toma un texto y construye la BWT, la tabla C y la tabla OCC.
    @param text El texto sobre el cual se construirá el FM-Index.
    */
    FMIndex(const string& text) {
        buildBWT(text); // Construir la BWT del texto
        buildCTable(); // Construir la tabla C
        buildOccTable(); // Construir la tabla OCC
    }
    
    /*
    @brief Método que busca un patrón en la BWT utilizando la tabla C y la tabla OCC.
    @param pattern El patrón a buscar en la BWT.
    @return Un vector de enteros que contiene los índices de las ocurrencias del patrón en la BWT.
    Si el patrón no se encuentra, devuelve un vector vacío.
    */
    vector<int> search(const string& pattern) {
        auto [top, bottom] = backwardSearch(pattern);
        if (top == -1) return {}; // Patrón no encontrado
        
        // Si se encontró el patrón, construir el resultado con los índices de las ocurrencias
        vector<int> result;
        for (int i = top; i <= bottom; ++i) {
            result.push_back(suffix_array[i]); // Agregar el índice de la ocurrencia
        }
        // Ordenar los resultados para que estén en orden ascendente
        sort(result.begin(), result.end());
        return result;
    }
};