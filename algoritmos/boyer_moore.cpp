/* Implementacion de algoritmo Boyer-Moore para la busqueda de patrones en cadenas de texto
   El algoritmo se basa en dos heurísticas: Bad Character y Good Suffix.
   El algoritmo tiene una complejidad temporal promedio de O(n/m) y una complejidad espacial de O(m), donde n es el tamaño del texto y m es el tamaño del patrón.
   El algoritmo es eficiente para buscar patrones en textos grandes y es ampliamente utilizado en aplicaciones de procesamiento de texto.

   Implementacion obtenida de: https://www.tutorialspoint.com/data_structures_algorithms/boyer_moore_algorithm.htm
*/

#include <bits/stdc++.h>
using namespace std;

// Funcion para calcular el desplazamiento completo del patrón
void computeFullShift(int shiftArr[], int longSuffArr[], string patrn) {
    
    int n = patrn.size(); 
    int i = n;
    int j = n+1;
    longSuffArr[i] = j;  // Inicializar el sufijo largo

    while(i > 0) {
        // Searching right if (i-1)th and (j-1)th item are not the same
        while(j <= n && patrn[i-1] != patrn[j-1] ) {
            // to shift pattern from i to j
            if(shiftArr[j] == 0) {
                shiftArr[j] = j-i; 
            }
            // Updating long suffix value
            j = longSuffArr[j]; 
        }
        i--;
        j--;
        longSuffArr[i] = j;
    }  
}
// Function for good suffix match
void computeGoodSuffix(int shiftArr[], int longSuffArr[], string patrn) {
    // length of the pattern
    int n = patrn.size(); 
    int j;
    j = longSuffArr[0];
    // Looping through the pattern
    for(int i = 0; i<n; i++) {
        // setting shift to long suffix value
        if(shiftArr[i] == 0) {
            shiftArr[i] = j; 
            if(i == j) {
                // Updating long suffix value
                j = longSuffArr[j]; 
            }
        }
    }
}

// Funcion para buscar el patrón en la cadena original y almacenar las posiciones en un vector
void searchPattern(string orgnStr, string patrn, vector<int> *array) {
    
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
    * desplazamientos de buen sufijo 
    */
    computeFullShift(shiftArr, longerSuffArray, patrn); 
    computeGoodSuffix(shiftArr, longerSuffArray, patrn); 
    
    int shift = 0;
    while(shift <= (strLen - patLen)) {
        int j = patLen - 1;
        // decrement j when pattern and main string character is matching
        while(j >= 0 && patrn[j] == orgnStr[shift+j]) {
            j--; 
        }
        if(j < 0) {
            // to store the position where pattern is found
            array->push_back(shift); 
            shift += shiftArr[0];
        }else {
            shift += shiftArr[j+1];
        }
    }
}

void boyer_moore(string text, string patrn) {

    // Vector para guardar las posiciones donde se encuentra el patrón
    vector<int> locArray;

    // Llamar a la función de búsqueda del patrón
    searchPattern(text, patrn, &locArray); 

    // Imprimir las posiciones donde se encuentra el patrón
    for(int i = 0; i < locArray.size(); i++) cout << "Patron encontrado en la posicion: " << locArray[i] << endl;
    
    cout << "Total de ocurrencias: " << locArray.size() << endl; // Imprimir total de ocurrencias
}