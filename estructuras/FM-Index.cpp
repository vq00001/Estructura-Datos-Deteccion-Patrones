#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "FM-Index.hpp"
#include "../utils/sais/sais.h"

using namespace std;

/**
 * @brief Prepara el texto de entrada para la construcción de la BWT.
 *
 * Asegura que el texto termine con un carácter centinela único (en este caso, '\x00')
 * que no aparece en el resto del texto, lo cual es fundamental para la BWT.
 *
 * @param text El texto original de entrada.
 * @return El texto modificado con el carácter centinela.
 * @note Complejidad Temporal: O(N), donde N es la longitud del texto.
 */
string FMIndex::prepareTextForBWT(const string& text) {
    if (text.empty()) return "\x00";                // Si el texto está vacío, devuelve solo el centinela
    else if (text.back() == '\x00') return text;    // Si ya tiene un centinela, no lo añade de nuevo
    return text + '\x00';                           // El centinela '\x00' indica el final del texto
}

/**
 * @brief Construye la Burrows-Wheeler Transform (BWT) y el Suffix Array (SA).
 *
 * Este es un paso fundamental en la construcción del FM-Index. A partir
 * del texto preparado, genera la BWT y el Suffix Array.
 *
 * @param input El texto ya preparado con el carácter centinela.
 * @note Complejidad Temporal: O(N), donde N es la longitud del texto.
 * @note Complejidad Espacial: O(N).
 * @sa buildCTable()
 * @sa buildOccTable()
 */
void FMIndex::buildBWT(const string& input) {
    // Prepara el texto con centinela
    string text = prepareTextForBWT(input);
    int n = text.size();
    
    // Suffix array local
    vector<int> local_suffix_array(n);
    iota(local_suffix_array.begin(), local_suffix_array.end(), 0);

    // Construir el suffix array utilizando SAIS (O(n))
    if (sais(reinterpret_cast<const unsigned char*>(text.c_str()), local_suffix_array.data(), n) != 0) {
        throw runtime_error("Error al construir el suffix array");
    }
    // Guardar el suffix array como atributo de la clase
    this->suffix_array = local_suffix_array;
    
    // Construir BWT (O(n))
    bwt.clear();
    bwt.reserve(n);
    for (int i = 0; i < n; ++i) {
        int char_index = (local_suffix_array[i] - 1 + n) % n;
        bwt += text[char_index];
    }
}

/**
 * @brief Construye la tabla C (conteo acumulado de caracteres).
 *
 * La tabla C almacena el conteo acumulado de cada carácter en el alfabeto,
 * lo cual es esencial para la función LF-mapping en el FM-Index.
 *
 * @note Complejidad Temporal: O(A), donde A es el tamaño del alfabeto (256 para ASCII).
 * @note Complejidad Espacial: O(A).
 */
void FMIndex::buildCTable() {
    ctable.assign(256, 0); // Carácteres ASCII

    // 1. Contar la frecuencia de cada carácter en la BWT (O(n))
    for (char c : bwt) {
        ctable[static_cast<unsigned char>(c)]++;
    }

    // 2. Calcular las sumas acumuladas para obtener la tabla C correcta (O(σ))
    // C[c] debe ser el número de caracteres < c
    int total_count = 0;
    for (int i = 0; i < 256; ++i) {
        int current_char_count = ctable[i];
        ctable[i] = total_count;
        total_count += current_char_count;
    }
}

/**
 * @brief Construye la tabla de ocurrencias (OCC) con checkpoints.
 *
 * La tabla OCC permite determinar el número de veces que un carácter
 * específico aparece hasta una posición dada en la BWT. Los checkpoints
 * se utilizan para optimizar la consulta de ocurrencias, balanceando
 * el espacio y el tiempo de consulta.
 *
 * @note Complejidad Temporal: O(N * A), donde N es la longitud de la BWT y A el tamaño del alfabeto.
 * @note Complejidad Espacial: O(N * A).
 * @sa getOcc()
 */
void FMIndex::buildOccTable() {
    int n = bwt.size();
    int num_checkpoints = (n + CHECKPOINT_INTERVAL - 1) / CHECKPOINT_INTERVAL + 1;
    occ_checkpoint_table.assign(256, vector<int>(num_checkpoints, 0));

    vector<int> current_counts(256, 0); // Conteo actual de cada carácter
    for (int i = 0; i < n; ++i) {
        current_counts[static_cast<unsigned char>(bwt[i])]++;
        int checkpoint_idx = (i + 1) / CHECKPOINT_INTERVAL;
        // Solo guarda el checkpoint si corresponde
        if ((i + 1) % CHECKPOINT_INTERVAL == 0 || i == n - 1) {
            for (int j = 0; j < 256; ++j) {
                occ_checkpoint_table[j][checkpoint_idx] = current_counts[j];
            }
        }
    }
}

/**
 * @brief Obtiene el número de ocurrencias de un carácter hasta una posición dada.
 *
 * Consulta la tabla OCC para determinar cuántas veces ha aparecido el carácter `c`
 * hasta la posición `k` (inclusive) en la BWT. Utiliza los checkpoints para acelerar.
 *
 * @param c El carácter del que se desea contar las ocurrencias.
 * @param k La posición (índice 0-basado) en la BWT hasta la cual contar.
 * @return El número de ocurrencias del carácter `c` hasta la posición `k`.
 * @note Complejidad Temporal: O(1).
 */
int FMIndex::getOcc(unsigned char c, int k) const {
    if (k <= 0) return 0;
    int checkpoint_idx = k / CHECKPOINT_INTERVAL;
    int count = occ_checkpoint_table[c][checkpoint_idx];
    int start_pos = checkpoint_idx * CHECKPOINT_INTERVAL;
    // Escaneo lineal desde el último checkpoint hasta k-1
    for (int i = start_pos; i < k; ++i) {
        if (static_cast<unsigned char>(bwt[i]) == c) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Realiza la búsqueda hacia atrás en la BWT para encontrar el intervalo.
 *
 * Es el corazón del algoritmo de búsqueda del FM-Index. Dado un patrón, esta función
 * encuentra el rango [L, R] en la BWT que corresponde a todas las ocurrencias
 * del patrón.
 *
 * @param pattern El patrón a buscar.
 * @return Un `std::pair<int, int>` que representa el rango [L, R] en la BWT
 * donde se encuentran las ocurrencias del patrón. Si el patrón no
 * se encuentra, L será mayor que R.
 * @note Complejidad Temporal: O(M * Intervalo), donde M es la longitud del patrón.
 * @note Complejidad Espacial: O(OCC), con OCC siendo el número de ocurrencias del patrón.
 * @sa search()
 */
pair<int, int> FMIndex::backwardSearch(const string& pattern) {
    int top = 0; // Inicio del intervalo
    int bottom = bwt.size() - 1; // Fin del intervalo
    // Procesa el patrón de derecha a izquierda
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

/**
 * @brief Constructor de la clase FMIndex.
 *
 * Construye el índice FM a partir del texto proporcionado. Este proceso implica
 * la creación de la Burrows-Wheeler Transform (BWT), el Suffix Array (SA),
 * la tabla C y la tabla de ocurrencias (OCC) con checkpoints.
 *
 * @param text El texto original sobre el que se construirá el índice.
 * @note Complejidad Temporal: O(N), donde N es la longitud del texto.
 * @note Complejidad Espacial: O(N), donde N es la longitud del texto.
 */
FMIndex::FMIndex(const string& text) {
    buildBWT(text);     // Construir la BWT del texto
    buildCTable();      // Construir la tabla C
    buildOccTable();    // Construir la tabla OCC
}

/**
 * @brief Busca todas las ocurrencias de un patrón en el texto original.
 *
 * Utiliza el algoritmo de búsqueda hacia atrás (backward search) sobre la BWT
 * para encontrar todas las posiciones de inicio del patrón en el texto original.
 *
 * @param pattern El patrón (subcadena) a buscar dentro del texto indexado.
 * @return Un `std::vector<int>` que contiene los índices de inicio de todas
 * las ocurrencias del patrón en el texto original. Si el patrón no
 * se encuentra, el vector estará vacío.
 * @note Complejidad Temporal: O(M * intervalo + occ),
 * donde $M$ es la longitud del patrón, `intervalo` es el tamaño del salto
 * para los checkpoints de la tabla OCC, y `occ` es el número de ocurrencias
 * del patrón en el texto.
 * @note Complejidad Espacial: O(1) de espacio adicional durante la búsqueda,
 * más el espacio para almacenar los resultados (depende de `occ`).
 * @sa backwardSearch()
 */
vector<int> FMIndex::search(const string& pattern) {
    auto [top, bottom] = backwardSearch(pattern);
    if (top == -1) return {}; // Patrón no encontrado
    
    // Si se encontró el patrón, construir el resultado con los índices de las ocurrencias
    vector<int> result;
    for (int i = top; i <= bottom; ++i) {
        result.push_back(suffix_array[i]); // Agregar el índice de la ocurrencia
    }
    return result;
}