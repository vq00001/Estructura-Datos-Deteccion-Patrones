#include <bits/stdc++.h>
#include "algoritmos/boyer_moore.cpp"
#include "utils.hpp"
#include <filesystem>
#include <iterator>
using namespace std;

int main(int argc, char *argv[]){

    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <algoritmo> <CarpetaConArchivosEntrada> <Patron> <CantidadArchivos>" << endl;
        return 1;
    }

    // Obtener el nombre del algoritmo y el archivo de entrada
    string algoritmo = argv[1];
    string carpeta = argv[2];
    string patron = argv[3];
    string texto;
    
    // Leer el archivo de entrada
    // Si se especifica una cantidad de archivos, se lee esa cantidad
    if (argc >= 5) {
        texto = readFile(carpeta, stoi(argv[4]));
    } else {
        texto = readFile(carpeta);
    }

    // Ordenar los datos según el algoritmo especificado
    if (algoritmo == "Boyer_Moore") {
        startTimer();

        boyer_moore(texto, patron);

        
    } else if (algoritmo == "KMP") {
        startTimer();
        // Implementar KMP aquí
    } else if (algoritmo == "Robin-Karp") {
        startTimer();
        // Implementar Robin-Karp aquí
    } else {
        cerr << "Algoritmo no reconocido. Ingresar alguno de los siguientes: Boyer_Moore, KMP, Robin-Karp" << endl;
        return 1;
    }
    
    stopTimer();
    return 0;
}