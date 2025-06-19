#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>

using namespace std;

namespace FMIndex {
    string prepareTextForBWT(const string& text) {
        // Prepara el texto para la BWT, agregando un centinela al final
        if (text.empty()) return "$"; // Si el texto está vacío, devuelve solo el centinela
        else if (text.back() == '$') return text; // Si ya tiene un centinela, no lo añade de nuevo
        return text + '$'; // El centinela '$' indica el final del texto
    }

    // Construye la Burrows-Wheeler Transform (BWT) de un texto
    string buildBWT(const string& input) {
        string text = prepareTextForBWT(input); // Prepara el texto añadiendo un centinela
        int n = text.size();
        if (n == 0) return "";

        // Paso 1: Crear un array de sufijos que contenga los índices [0, 1, 2, ..., n-1]
        vector<int> suffix_array(n);
        iota(suffix_array.begin(), suffix_array.end(), 0);

        // Paso 2: Ordenar los índices basándose en la rotación cíclica que representan. La comparación ahora se hace sobre el texto con el centinela.
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
        string bwt_result;
        bwt_result.reserve(n); // Pre-reservar memoria para eficiencia
        for (int i = 0; i < n; ++i) {
            // El carácter es el que está justo antes del sufijo (con ajuste cíclico)
            int char_index = (suffix_array[i] - 1 + n) % n;
            bwt_result += text[char_index];
        }

        return bwt_result;
    }

    void buildCTable(const string& bwt, vector<int>& ctable) {
        // Construye la tabla C (conteo de caracteres)
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

    void buildOccTable(const string& bwt, vector<vector<int>>& occ_table) {
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

    // Realiza una búsqueda hacia atrás en la BWT para encontrar el intervalo de ocurrencias del patrón
    pair<int, int> backwardSearch(const string& pattern, const string& bwt, const vector<int>& ctable, const vector<vector<int>>& occ_table) {
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
    
    // Realiza una búsqueda de un patrón en la BWT y devuelve los índices de las ocurrencias
    vector<int> search(const string& pattern, const string& bwt, const vector<int>& ctable, const vector<vector<int>>& occ_table) {
        auto [top, bottom] = backwardSearch(pattern, bwt, ctable, occ_table);
        if (top == -1) return {}; // Patrón no encontrado

        // Si se encontró el patrón, construir el resultado con los índices de las ocurrencias
        vector<int> result;
        for (int i = top; i <= bottom; ++i) {
            result.push_back(i); // Agregar el índice de la ocurrencia
        }
        return result;
    }
}

int main() {
    string text = "banana de ana$";
    string bwt = FMIndex::buildBWT(text);
    cout << "BWT: " << bwt << endl;

    vector<int> ctable;
    FMIndex::buildCTable(bwt, ctable);
    
    vector<vector<int>> occ_table;
    FMIndex::buildOccTable(bwt, occ_table);

    string pattern = "ana";
    vector<int> occurrences = FMIndex::search(pattern, bwt, ctable, occ_table);
    
    cout << "Veces encontrado: " << occurrences.size() << endl;

    return 0;
}