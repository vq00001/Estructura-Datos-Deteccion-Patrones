#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "utils.hpp"
#include <filesystem>
#include "animation.hpp"
namespace fs = std::filesystem;

#define BUFFER_SIZE 1024 // Tamaño del buffer para lectura de archivos

using namespace std;

// Variable estática para almacenar el tiempo de inicio del temporizador
static chrono::high_resolution_clock::time_point startTime;


// Función para leer archivos de una carpeta y devolver su contenido como una cadena separada por "$"
// Si cantidadArchivos es -1, se leen todos los archivos de la carpeta
string readFolder(const string &carpeta, vector<int> *posiciones, int cantidadArchivos) {
    
    SimpleAnimator animador;
    animador.start("\033[34mLeyendo archivos");

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

        txt = txt + "$" + string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        
        // Si se pasan posiciones, agregar la posición del archivo al vector
        posiciones->push_back(txt.size() - 1); // Agregar la posición del final del archivo
        

        cantidadArchivos--; // Decrementar la cantidad de archivos restantes   
    }

    animador.end("\033[32mArchivos leídos con éxito.\033[0m"); // Terminar la animación

    return txt; // Devolver el contenido del archivo como una cadena
}

string readFile(const string &archivo) {
    
    if (!fs::is_regular_file(archivo)) return ""; // Solo archivos
    
    cout << "Leyendo archivo: " << archivo << endl; // Mostrar el nombre del archivo que se está leyendo
    
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
    file.close();
    
    return txt; // Devolver el contenido del archivo como una cadena
}

void encuentros_por_archivo(const string &carpeta, const vector<int> &posiciones_patrones, const vector<int> &pos_final_archivos) {

    // Verificar si la carpeta existe
    if (!fs::exists(carpeta)) {
        cerr << "La carpeta no existe: " << carpeta << endl;
        return;
    }

    if (!fs::is_directory(carpeta)) {
        cerr << "La ruta especificada no es una carpeta: " << carpeta << endl;
        return;
    }

    // Almacenar los nombres de los archivos 
    vector<pair<string, int>> nombres_archivos_vec;
 
    for (const auto &entry : fs::directory_iterator(carpeta)) {
        if (fs::is_regular_file(entry.path())) {
            nombres_archivos_vec.push_back({entry.path().filename().string(), 0});
        }
    }

    
    size_t j = 0;
    for (size_t i = 0; i < nombres_archivos_vec.size(); ++i) {
        
        if (j >= posiciones_patrones.size()) break;

        // Si el archivo actual no tiene ocurrencias del patrón, continuar al siguiente
        if(pos_final_archivos[i] < posiciones_patrones[j]) {
            continue;
        }

        // Contar las ocurrencias del patrón en el archivo actual
        while (j < posiciones_patrones.size() && posiciones_patrones[j] <= pos_final_archivos[i]) {
            nombres_archivos_vec[i].second++;
            j++;
        }
    }
    
    cout << "\nArchivo, Ocurrencias" << endl;
    // Imprimir los resultados
    for (auto n : nombres_archivos_vec){
        // if (n.second == 0) continue;
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