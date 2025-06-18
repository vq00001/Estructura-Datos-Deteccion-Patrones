#include <bits/stdc++.h>
#include "algoritmos/boyer_moore.hpp"
#include "algoritmos/robin_karp.hpp"
#include "utils.hpp"
#include <filesystem>
#include <iterator>
using namespace std;

int main(int argc, char *argv[]){

    if (argc < 5) {
        cerr << "Uso: " << argv[0] << " <algoritmo> <Patron> -<flag> <CarpetaConArchivosEntrada> <CantidadArchivos>" << endl;
        return 1;
    }

    // Obtener el nombre del algoritmo y el archivo de entrada
    // si el flag es -f, se busca en carpetas
    // si el flag es -a, se busca en archivos 
    string algoritmo = argv[1];
    string patron = argv[2];
    string flag = argv[3];
    if (flag != "-f" && flag != "-a") {
        cerr << "Flag no reconocido. Ingresar -f para buscar en carpetas o -a para buscar en archivos." << endl;
        return 1;
    }
    string nombre_archivo_carpeta = argv[4];
    string texto;
    
    // Leer el archivo de entrada
    // Si se especifica una cantidad de archivos, se lee esa cantidad

    vector<int> pos_final_archivos; // Vector para almacenar las posiciones del final de los archivos leídos
    if (argc > 5 && flag == "-f") {
        texto = readFolder(nombre_archivo_carpeta, &pos_final_archivos, stoi(argv[5]));
    } else if (flag == "-f") {
        texto = readFolder(nombre_archivo_carpeta, &pos_final_archivos);
    } else if (flag == "-a") {
        texto = readFile(nombre_archivo_carpeta);
    } else {
        cerr << "Error al leer el archivo o carpeta." << endl;
        return 1;
    }

    vector<int> posiciones;
    // Ordenar los datos según el algoritmo especificado
    if (algoritmo == "Boyer-Moore") {
        startTimer();

        posiciones = boyer_moore(texto, patron);

    } else if (algoritmo == "KMP") {
        startTimer();
        // Implementar KMP aquí
    } else if (algoritmo == "Robin-Karp") {
        startTimer();

        posiciones = robinKarp(texto, patron);
        
    } else {
        cerr << "Algoritmo no reconocido. Ingresar alguno de los siguientes: Boyer-Moore, KMP, Robin-Karp" << endl;
        return 1;
    }

    cout << "\nTiempo de ejecución: ";
    stopTimer();

    // Mostrar las posiciones encontradas por archivo

    if (flag == "-f") {
        cout << "Archivo: " << nombre_archivo_carpeta << endl;
        encuentros_por_archivo(nombre_archivo_carpeta, posiciones, pos_final_archivos);
    } else {
        cout << "Cantidad de coincidencias encontradas: " << posiciones.size() << endl;
    }


    return 0;
}