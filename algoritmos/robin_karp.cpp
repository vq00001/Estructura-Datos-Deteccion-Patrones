#include <vector>
#include <string>
#include "robin_karp.hpp"

using namespace std;

namespace robin_karp {
    constexpr int prime = 101;    // Número primo para el cálculo del hash
    constexpr int d = 256;        // Número de caracteres en el alfabeto

    // Función para calcular el hash de un patrón
    long long hash(const string &str, int length) {
        long long h = 0;
        for (int i = 0; i < length; i++) {
            h = (static_cast<long long>(d) * h + str[i]) % prime;
        }
        return h;
    }
}

// Función para buscar un patrón en un texto usando el algoritmo de Robin-Karp
vector<int> robinKarp(const string &text, const string &pattern) {
    vector<int> result;
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m) return result;

    long long patternHash = robin_karp::hash(pattern, m);
    long long textHash = robin_karp::hash(text, m);

    // Calcular el valor de h para el último carácter del patrón
    long long h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * robin_karp::d) % robin_karp::prime;
    }

    for (int i = 0; i <= n - m; i++) {
        if (patternHash == textHash) {
            // Comparar caracteres.
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                result.push_back(i);
            }
        }
        // Calcular el hash del siguiente substring
        if (i < n - m) {
            textHash = (robin_karp::d * (textHash - text[i] * h) + text[i + m]) % robin_karp::prime;
            if (textHash < 0) {
                textHash += robin_karp::prime; // Asegurar que el hash sea positivo
            }
        }
    }
    return result;
}