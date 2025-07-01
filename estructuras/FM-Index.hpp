#ifndef FM_INDEX_HPP
#define FM_INDEX_HPP

#include <string>
#include <vector>
#include <utility>

/**
 * @class FMIndex
 * @brief Implementa el índice FM para búsqueda eficiente de patrones en texto.
 *
 * Esta clase encapsula la lógica para construir y utilizar un FM-Index.
 * Su objetivo es proporcionar una interfaz sencilla para realizar búsquedas
 * rápidas de patrones en un texto largo previamente indexado.
 */
class FMIndex {
public:
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
    FMIndex(const std::string& text);

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
    std::vector<int> search(const std::string& pattern);

private:
    std::string bwt;                                        // La Burrows-Wheeler Transform (BWT) del texto original.
    std::vector<int> ctable;                                // Tabla C: Almacena el conteo acumulado de cada carácter.
    std::vector<std::vector<int>> occ_checkpoint_table;     // Tabla OCC con checkpoints para consultas rápidas.
    std::vector<int> suffix_array;                          // El Suffix Array (SA) del texto original.
    static const int CHECKPOINT_INTERVAL = 16;              // Intervalo de frecuencia para los checkpoints de la tabla OCC.

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
    std::string prepareTextForBWT(const std::string& text);

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
    void buildBWT(const std::string& input);

    /**
     * @brief Construye la tabla C (conteo acumulado de caracteres).
     *
     * La tabla C almacena el conteo acumulado de cada carácter en el alfabeto,
     * lo cual es esencial para la función LF-mapping en el FM-Index.
     *
     * @note Complejidad Temporal: O(A), donde A es el tamaño del alfabeto (256 para ASCII).
     * @note Complejidad Espacial: O(A).
     */
    void buildCTable();

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
    void buildOccTable();

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
    int getOcc(unsigned char c, int k) const;

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
    std::pair<int, int> backwardSearch(const std::string& pattern);
};

#endif // FM_INDEX_HPP