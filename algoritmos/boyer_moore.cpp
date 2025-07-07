/* Implementacion de algoritmo Boyer-Moore para la busqueda de patrones en cadenas de texto
   El algoritmo se basa en dos heurísticas: Bad Character y Good Suffix.
   El algoritmo tiene una complejidad temporal promedio de O(n/m) y una complejidad espacial de O(m), donde n es el tamaño del texto y m es el tamaño del patrón.
   El algoritmo es eficiente para buscar patrones en textos grandes y es ampliamente utilizado en aplicaciones de procesamiento de texto.

   Implementacion obtenida de: https://www.tutorialspoint.com/data_structures_algorithms/boyer_moore_algorithm.htm
*/

#include <bits/stdc++.h>
#include "boyer_moore.hpp"

using namespace std;

// Funcion para calcular el desplazamiento completo del patrón
void computeFullShift(int shiftArr[], int longSuffArr[], string patrn) {

    
    int n = patrn.size(); 
    int i = n;
    int j = n+1;
    longSuffArr[i] = j;  // Inicializar el sufijo largo del patrón
    
    // Mientras quedan caracteres por procesar en el patrón
    while(i > 0) {
        // Buscar a la derecha si el elemento (i-1) y (j-1) no son iguales
        while(j <= n && patrn[i-1] != patrn[j-1] ) {
            // Desplazar el patrón de i a j
            if(shiftArr[j] == 0) {
                shiftArr[j] = j-i; 
            }
            // Actualizar el valor del sufijo largo
            j = longSuffArr[j]; 
        }
        i--;
        j--;
        longSuffArr[i] = j;
    }  
}

// Funcion para computar Good Suffix 
void computeGoodSuffix(int shiftArr[], int longSuffArr[], string patrn) {
    
    int n = patrn.size();  // Largo del patrón
    int j;
    j = longSuffArr[0];
    
    // Recorrer el patrón
    for(int i = 0; i<n; i++) {
        // setting shift to long suffix value 
        if(shiftArr[i] == 0) {
            shiftArr[i] = j; 
            if(i == j) {
                // Actualizar el valor del sufijo largo
                j = longSuffArr[j]; 
            }
        }
    }
}

// Funcion para buscar el patrón en la cadena original y almacenar las posiciones en un vector
void searchPattern(string orgnStr, string patrn, vector<int> *position_array) {
    
    int patLen = patrn.size();          // Largo del patron
    int strLen = orgnStr.size();        // Largo de la cadena original
    int longerSuffArray[patLen + 1];    // Array para almacenar los sufijos largos
    int shiftArr[patLen + 1];           // Array para almacenar los desplazamientos

    // Inicializando los elementos del array de desplazamientos a 0
    for(int i = 0; i <= patLen; i++) {
        shiftArr[i] = 0;
    }

    /* Llamando a la función computeFullShift y computeGoodSuffix para calcular los desplazamientos
    * La primera calcula los desplazamientos completos para el patrón y la segunda calcula los  
    * desplazamientos de Good Suffix.
    * Estas funciones son parte de la heurística del algoritmo Boyer-Moore.
    */
    computeFullShift(shiftArr, longerSuffArray, patrn); 
    computeGoodSuffix(shiftArr, longerSuffArray, patrn); 
    
    int shift = 0;
    while(shift <= (strLen - patLen)) {
        int j = patLen - 1;
        // disminuir j cuando el patron y el caracter de la cadena principal coinciden
        while(j >= 0 && patrn[j] == orgnStr[shift+j]) {
            j--; 
        }

        // Si j es menor que 0, significa que se encontró el patrón
        if(j < 0) {
            // almacenar la posición donde se encuentra el patrón
            // cout << shift << endl; // Imprimir la posición donde se encontró el patrón
            position_array->push_back(shift); 
            shift += max(shiftArr[0], 1);   // Desplazar el patrón 
        } else {
            // Si j es mayor o igual a 0, significa que no se encontró el patrón
            shift += max(shiftArr[j+1], 1); // Desplazar el patrón
        }
    }
}

vector<int> boyer_moore(const std::string &txt, const std::string &pat) {

    // Vector para guardar las posiciones donde se encuentra el patrón
    vector<int> locArray;

    // Llamar a la función de búsqueda del patrón
    searchPattern(txt, pat, &locArray); 

    // Imprimir las posiciones donde se encuentra el patrón
    return locArray;
}
