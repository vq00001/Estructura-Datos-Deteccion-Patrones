#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "utils.hpp"
#include <filesystem>
#include "animation.hpp"
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

#define BUFFER_SIZE 1024 // Tamaño del buffer para lectura de archivos

using namespace std;

// Variable estática para almacenar el tiempo de inicio del temporizador
static chrono::high_resolution_clock::time_point startTime;


// Función para leer archivos de una carpeta y devolver su contenido como una cadena separada por "$"
// Si cantidadArchivos es -1, se leen todos los archivos de la carpeta
string readFolder(const string &carpeta, vector<int> *posiciones,bool expresivo ,int cantidadArchivos) {
    
    SimpleAnimator animador;
    if(expresivo){
        animador.start("\033[34mLeyendo archivos");
    }

    string txt = ""; // Variable para almacenar el contenido del archivo

    for (const auto & entry : fs::directory_iterator(carpeta)){

        if (cantidadArchivos == 0) break; // Si se ha alcanzado la cantidad de archivos, salir del bucle
        
        if (!fs::is_regular_file(entry.path())) continue; // Solo archivos
        
        if (!fs::exists(entry.path())) {
            animador.end("\033[31mEl archivo no existe: \033[0m" + entry.path().string());
            return "";
        }
        
        ifstream file(entry.path());
        
        if (!file.is_open()) {
            animador.end("\033[31mError al abrir archivo: \033[0m" + entry.path().string());
            return "";
        }

        string file_content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        // Reemplazar caracteres no ASCII
        for (char &c : file_content) {
            if (static_cast<unsigned char>(c) > 127) c = '\x1F';
        }
        txt = txt + "$" + file_content;
        file.close();

        
        // Si se pasan posiciones, agregar la posición del archivo al vector
        posiciones->push_back(txt.size() - 1); // Agregar la posición del final del archivo
        

        cantidadArchivos--; // Decrementar la cantidad de archivos restantes   
    }

    if (expresivo) {
        animador.end("\033[32mArchivos leídos con éxito.\033[0m"); // Terminar la animación
    }
    return txt; // Devolver el contenido del archivo como una cadena
}

string readFile(const string &archivo,bool expresivo) {
    
    if (!fs::is_regular_file(archivo)) return ""; // Solo archivos
    
    if(expresivo) cout << "Leyendo archivo: " << archivo << endl; // Mostrar el nombre del archivo que se está leyendo
    
    if (!fs::exists(archivo)) {
        cerr << "El archivo no existe: " << archivo << endl;
        return "";
    }
    
    // Abrir el archivo y leer su contenido
    ifstream file(archivo);

    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << archivo << endl;
        return "";
    }

    string txt = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    // Reemplazar caracteres no ASCII
    for (char &c : txt) {
        if (static_cast<unsigned char>(c) > 127) c = '\x1F';
    }
    file.close();
    
    return txt; // Devolver el contenido del archivo como una cadena
}

void encuentros_por_archivo(const string &carpeta, const vector<int> &posiciones_patrones, const vector<int> &pos_final_archivos) {
    if (!fs::exists(carpeta) || !fs::is_directory(carpeta)) {
        cerr << "Carpeta inválida: " << carpeta << endl;
        return;
    }

    // Almacenar los nombres de los archivos en orden lexicográfico
    vector<pair<string, int>> nombres_archivos_vec;
    for (const auto &entry : fs::directory_iterator(carpeta)) {
        if (fs::is_regular_file(entry.path())) {
            nombres_archivos_vec.emplace_back(entry.path().filename().string(), 0);
        }
    }

    // Ordenar por nombre para asegurar orden consistente
    sort(nombres_archivos_vec.begin(), nombres_archivos_vec.end());

    // Ordenar las posiciones para hacer conteo eficiente
    vector<int> posiciones_ordenadas = posiciones_patrones;
    sort(posiciones_ordenadas.begin(), posiciones_ordenadas.end());

    size_t j = 0;
    for (size_t i = 0; i < nombres_archivos_vec.size(); ++i) {
        while (j < posiciones_ordenadas.size() && posiciones_ordenadas[j] <= pos_final_archivos[i]) {
            nombres_archivos_vec[i].second++;
            j++;
        }
    }

    cout << "\nArchivo, Ocurrencias" << endl;
    // Imprimir los resultados
    for (auto n : nombres_archivos_vec){
        if (n.second == 0) continue;
        cout << n.first << ",  " << n.second << endl;
    }
}

// Función para iniciar el temporizador
void startTimer() {
    //cout << "Temporizador iniciado." << endl;
    startTime = chrono::high_resolution_clock::now();
}


// Función para detener temporizador y mostrar el tiempo transcurrido
void stopTimer() {
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

    if (duration.count() == 0) cerr << "ADVERTENCIA: El tiempo medido es igual a 0 ns." << endl;

    cout << duration.count() << endl; // Imprimir solo el tiempo en nanosegundos para la exportación a CSV
    startTime = endTime; // Reiniciar el temporizador para la próxima vez
}

long long getAndStopTime(){
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(endTime - startTime);

    if (duration.count() == 0) cerr << "ADVERTENCIA: El tiempo medido es igual a 0 ns." << endl;

    long long time = duration.count() ; 
    startTime = endTime; // Reiniciar el temporizador para la próxima vez
    return time;
}

vector<string> readPatterns(const string &file){
    string txt = readFile(file,false); //lee el archivo y lo guarda

    std::vector<string> patterns;
    string separador = string("\n") + static_cast<char>(28) + "\n";
    size_t inicio = 0;
    size_t fin = txt.find(separador,inicio);

    //Itera en el texto del archivo hasta encontrar el separador establecido, y guarda el string asociado en el vector  de patrones
    while(fin != string::npos){
        string patron = txt.substr(inicio, fin-inicio);
        patterns.push_back(patron);
        inicio = fin + separador.length(); 
        fin = txt.find(separador,inicio);
    }

    return patterns;
}

bool verifyPattern(vector<int> posiciones, const string &text, const string &pattern){
    int n = pattern.size();
    int m = text.size();
    for (int i : posiciones){
        for (int j = 0; j < n; ++j){
            if(j+i >= m || text[j+i] != pattern[j]){
                return false;
            }
        }
    }
    return true;
}


pair<long long, long long> calcularPromedioYVarianza(const vector<long long> &tiempos) {
    if (tiempos.empty()) return {0, 0}; // Si no hay tiempos, retornar 0 para ambos

    long long suma = 0;
    for (long long tiempo : tiempos) {
        suma += tiempo;
    }

    double promedio = static_cast<double>(suma) / tiempos.size();

    double varianza = 0;
    for (long long tiempo : tiempos) {
        varianza += (tiempo - promedio) * (tiempo - promedio);
    }
    varianza /= tiempos.size();

    return {static_cast<long long>(promedio), static_cast<long long>(varianza)};
}

void escribirCSVTiempoConstruccion(long long tiempo_creacion, const string &algoritmo, const string &carpeta, int cant_archivos, const string &archivo) {
    ofstream file(archivo, ios::app);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo CSV: " << archivo << endl;
        return;
    }
    file << carpeta << ";" <<  cant_archivos << ";" << algoritmo << ";" << tiempo_creacion << endl;
    file.close();
}


// void eliminarCharNoAscii() {
//     int cantidadArchivos = -1; // Inicializar la cantidad de archivos a 0
//     string txt = "";
//     for (const auto & entry : fs::directory_iterator('./archivos_de_prueba/sources/')){

//         if (cantidadArchivos == 0) break; // Si se ha alcanzado la cantidad de archivos, salir del bucle
        
//         if (!fs::is_regular_file(entry.path())) continue; // Solo archivos
        
        
//         ifstream file(entry.path());
        
//         if (!file.is_open()) {
//             animador.end("\033[31mError al abrir archivo: \033[0m" + entry.path().string());
//             return "";
//         }

//         txt = txt + "$" + string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
//         file.close();

        
//         // Si se pasan posiciones, agregar la posición del archivo al vector
//         posiciones->push_back(txt.size() - 1); // Agregar la posición del final del archivo
        

//         cantidadArchivos--; // Decrementar la cantidad de archivos restantes   
//     }
// }

