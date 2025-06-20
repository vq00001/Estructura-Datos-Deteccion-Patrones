#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include "../utils/SA-IS.hpp"

using namespace std;

class FMIndex {
private:
    // Atributos necesarios de FMIndex

    string bwt;                                     // Burrows-Wheeler Transform
    vector<int> ctable;                             // Tabla C (conteo de caracteres)
    vector<vector<int>> occ_checkpoint_table;       // Tabla OCC con checkpoints para optimizar memoria
    vector<int> suffix_array;                       // Tabla de sufijos
    static const int CHECKPOINT_INTERVAL = 64;      // Intervalo para los checkpoints en OCC

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
        SAIS::build_suffix_array(text, suffix_array);

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
    @brief Método que construye la tabla OCC (conteo de ocurrencias) con checkpoints para ahorrar espacio.
    */
    void buildOccTable() {
        int n = bwt.size();
        int num_checkpoints = (n / CHECKPOINT_INTERVAL) + 1;
        occ_checkpoint_table.assign(256, vector<int>(num_checkpoints, 0));

        vector<int> current_counts(256, 0);
        for (int i = 0; i < n; ++i) {
            current_counts[static_cast<unsigned char>(bwt[i])]++;
            if ((i + 1) % CHECKPOINT_INTERVAL == 0) {
                int checkpoint_idx = (i + 1) / CHECKPOINT_INTERVAL;
                for (int j = 0; j < 256; ++j) {
                    occ_checkpoint_table[j][checkpoint_idx] = current_counts[j];
                }
            }
        }
    }

    /*
    @brief Calcula el número de ocurrencias del carácter 'c' en el prefijo BWT[0..k-1] usando la tabla de checkpoints.
    */
    int getOcc(unsigned char c, int k) const {
        if (k <= 0) return 0;
        
        int checkpoint_idx = k / CHECKPOINT_INTERVAL;
        int count = occ_checkpoint_table[c][checkpoint_idx];
        
        int start_pos = checkpoint_idx * CHECKPOINT_INTERVAL;
        for (int i = start_pos; i < k; ++i) {
            if (static_cast<unsigned char>(bwt[i]) == c) {
                count++;
            }
        }
        return count;
    }

    /*
    @brief Método que realiza la búsqueda hacia atrás en la BWT para encontrar el intervalo
    */
    pair<int, int> backwardSearch(const string& pattern) {
        int top = 0; // Inicio del intervalo
        int bottom = bwt.size() - 1; // Fin del intervalo
    
        for (int i = pattern.size() - 1; i >= 0; --i) {
            unsigned char c = pattern[i];
            
            // Actualizar el intervalo [top, bottom] usando getOcc
            top = ctable[c] + getOcc(c, top);
            bottom = ctable[c] + getOcc(c, bottom + 1) - 1;
    
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