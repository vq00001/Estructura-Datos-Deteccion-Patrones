#include <string>
#include <vector>
#include <iostream>
#include "KMP.hpp"

using namespace std;

//Implementación basada en el concepto obtenido de su definición en wikipedia y luego optimizada 
//en base a otras implementaciones vistas
//https://es.wikipedia.org/wiki/Algoritmo_Knuth-Morris-Pratt

/* Funcion para crear la tabla de fallos de KMP */
vector<int> tableKMP(const string& pattern){
	int large = pattern.size();
	int index = 0;
	vector<int> tablaKMP(large,0);
	for (int i = 1; i < large; ++i){
		while(index > 0 && pattern[index] != pattern[i]){
			index = tablaKMP[index-1];
		}
		if(pattern[i] == pattern[index]){
			index++;
		}
		tablaKMP[i] = index;
	}
	return tablaKMP;
}

/*KMP Crea una tabla de fallos basandose en cuantas veces se repite el prefijo del patrón dentro de sí,
de esta forma puede recorrer el texto de forma linear y saltar rapidamente coincidencias que terminaron
fallando */
vector<int> KMP(const string& pattern,const string& text){
	vector<int> f= tableKMP(pattern);
	//Arreglo para guardar las posiciones donde efectivamente se encuentra el patrón
	std::vector<int> positionArray;
	//indexPattern e indexText para recorrer el texto e ir comparandolo con el patrón
	int m = text.size(), n = pattern.size(), indexPattern =0, indexText = 0;
	if (m >= n){
		//Mientras quede texto por analizar
		while(indexText + indexPattern < m ){
			//Si hay coincidencia ir moviendo los indices
			if(pattern[indexPattern] == text[indexPattern+indexText]){
				indexPattern++;
				if(indexPattern == n){
					positionArray.push_back(indexText);
					indexText = indexText + indexPattern - f[indexPattern-1];
					indexPattern = f[indexPattern-1]; 
				}
			}
			//Si no, reiniciar indice y saltar posiciones segun la tabla de fallos.
			else{
				if(indexPattern == 0) indexText++;
				else{
					indexText = indexText + indexPattern - f[indexPattern-1];
					indexPattern = f[indexPattern-1];
				}
			}
			//Cuando hay coincidencia guardar la posición
			
		}
	}
	return positionArray;

}