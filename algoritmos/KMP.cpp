#include <string>
#include <vector>

using namespace std;

/* Funcion para crear la tabla de fallos de KMP */
vector<int> tableKMP(const string& pattern){
	int large = pattern.size();
	int index = 0;
	vector<int> tablaKMP(large,0);
	tablaKMP[0] = -1;
	for (int i = 1; i < large; ++i){
		tablaKMP[i] = index;
		if(pattern[i] == pattern[index]){
			index++;
		}
		else{
			index = 0;
		}
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
			}
			//Si no, reiniciar indice y saltar posiciones segun la tabla de fallos.
			else{
				indexText = indexText + indexPattern - f[indexPattern];
				if(indexPattern>0 ) indexPattern = f[indexPattern]; 
			}
			//Cuando hay coincidencia guardar la posición
			if(indexPattern == n){
				positionArray.push_back(indexText);
				indexPattern = 0;
				indexText++;
			}
		}
	}
	return positionArray;

}