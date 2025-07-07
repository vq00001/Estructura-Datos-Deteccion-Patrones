#include <bits/stdc++.h>
#include "algoritmos/boyer_moore.hpp"
#include "algoritmos/KMP.hpp"
#include "algoritmos/robin_karp.hpp"
#include "algoritmos/KMP_bad.hpp"
#include "estructuras/suffix_tree.hpp"
#include "estructuras/FM-Index.hpp"
#include "utils/utils.hpp"
#include <filesystem>
#include <iterator>
using namespace std;

int main(int argc, char *argv[]){

    if (argc < 5) {
        cerr << "Uso: " << argv[0] << " <algoritmo> <Patron> -<flag> <CarpetaConArchivosEntrada> <FlagsAdicionales>" << endl;
        return 1;
    }

    // Obtener el nombre del algoritmo y el archivo de entrada
    // si el flag es -f, se busca en carpetas
    // si el flag es -a, se busca en archivos 
    string algoritmo = argv[1];
    vector<string> patrones = {argv[2]};
    string flag = argv[3];

    if (flag != "-f" && flag != "-a") {
        cerr << "Flag no reconocido. Ingresar -f para buscar en carpetas o -a para buscar en archivos." << endl;
        return 1;
    }
    string nombre_archivo_carpeta = argv[4];
    string arg;
    [[maybe_unused]] bool reporte = false, detallado = false, promedio = true, test = false ;
    [[maybe_unused]] int repeticiones = 1, tiempo=0, numArchivos = -1;
    long long tiempoTotal=0;

    //Flags adicionales que puede colocar el usuario
    for (int i = 5; i < argc; ++i){
        arg = argv[i];
        if(arg == "-r"){
            reporte = true;
        } else if(arg.find("-repeticiones=")==0){
            repeticiones = stoi(arg.substr(14));
        } else if(arg == "-d"){
            detallado = true;
        } else if(arg == "-p"){
            promedio = false;
        } else if(arg.find("-cantidad=")==0){
            numArchivos = stoi(arg.substr(10));
        } else if(arg == "--patrones"){
            patrones = readPatterns(patrones[0]); //Interpreta el argumento del patron como el nombre de un archivo en vez de un patrón
        } else if(arg == "-test"){
            test = true;
        }
    }
    string texto;
    
    // Leer el archivo de entrada
    // Si se especifica una cantidad de archivos, se lee esa cantidad

    vector<int> pos_final_archivos; // Vector para almacenar las posiciones del final de los archivos leídos
    if (flag == "-f") {
        texto = readFolder(nombre_archivo_carpeta, &pos_final_archivos, reporte ,numArchivos);
    } else if (flag == "-a") {
        texto = readFile(nombre_archivo_carpeta);
    } else {
        cerr << "Error al leer el archivo o carpeta." << endl;
        return 1;
    }
    long long tiempo_creacion = 0;
    vector<int> posiciones;
    vector<int> v;
    SuffixTree* tree;
    FMIndex* fmIndex = nullptr;
    if(algoritmo == "Suffix-Tree"){
        startTimer();
        tree = new SuffixTree(texto);
        //tiempoTotal+= getAndStopTime();
        tiempo_creacion = getAndStopTime();
        escribirCSVTiempoConstruccion(tiempo_creacion, algoritmo,nombre_archivo_carpeta, numArchivos, "tiempo_de_creacion.csv");
    } else if (algoritmo == "FM-Index"){
        startTimer();
        fmIndex = new FMIndex(texto);
        tiempo_creacion = getAndStopTime();
        escribirCSVTiempoConstruccion(tiempo_creacion, algoritmo, nombre_archivo_carpeta, numArchivos, "tiempo_de_creacion.csv");
    } 
    

    vector<long long>  tiemposAlgoritmo; // Vector para almacenar los tiempos de ejecución de cada algoritmo

    for(int i = 0; i < repeticiones; i++) {
        for (string patron : patrones){
            if (algoritmo == "Boyer-Moore") {
                startTimer();

                posiciones = boyer_moore(texto, patron);

            } else if (algoritmo == "KMP") {
                startTimer();

                posiciones = KMP(patron,texto);

            } else if (algoritmo == "Robin-Karp") {
                startTimer();

                posiciones = robinKarp(texto, patron);
            
            } else if(algoritmo == "KMP_bad"){
                startTimer();

                posiciones = KMP_bad(patron,texto);
            } else if(algoritmo == "Suffix-Tree"){
                startTimer();

                posiciones = tree->search(patron);
            } else if (algoritmo == "FM-Index") {
                startTimer();

                posiciones = fmIndex->search(patron);            
            } else {
                cerr << "Algoritmo no reconocido. Ingresar alguno de los siguientes: Boyer-Moore, KMP, Robin-Karp" << endl;
                return 1;
            }
            tiempoTotal += getAndStopTime();
            
            if(test){
                if(verifyPattern(posiciones,texto,patron)) cout << "El algoritmo funciona" << endl;
                else cout << "Aparentemente no..." << endl;
            }
        }

        tiemposAlgoritmo.push_back(tiempoTotal);
        tiempoTotal = 0;
    }


    pair<long long, long long> promedioVarianza = calcularPromedioYVarianza(tiemposAlgoritmo);
    cout << promedioVarianza.first << ";" << promedioVarianza.second << endl;   

    if(algoritmo == "Suffix-Tree") delete tree;
    if(algoritmo == "FM-Index") delete fmIndex;
    // Ordenar los datos según el algoritmo especificado
    //if(reporte) cout << "\nTiempo de ejecución: " ;
    //cout << tiempoTotal << endl;

    
    // Mostrar las posiciones encontradas por archivo
    if(reporte){
        if (flag == "-f") {
            cout << "Archivo: " << nombre_archivo_carpeta << endl;
            encuentros_por_archivo(nombre_archivo_carpeta, posiciones, pos_final_archivos);
        } else {
            cout << "Cantidad de coincidencias encontradas: " << posiciones.size() << endl;
        } 
    }   

    return 0;
}