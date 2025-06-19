#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>

using namespace std;

namespace FMIndex {
    string prepareTextForBWT(const string& text) {
        // Prepara el texto para la BWT, agregando un centinela al final
        if (text.empty()) {
            return "$"; // Si el texto está vacío, devuelve solo el centinela
        } else if (text.back() == '$') {
            return text; // Si ya tiene un centinela, no lo añade de nuevo
        } 
        return text + '$'; // El centinela '$' indica el final del texto
    }

    // Construye la Burrows-Wheeler Transform (BWT) de un texto
    string build_BWT(const string& input) {
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
}