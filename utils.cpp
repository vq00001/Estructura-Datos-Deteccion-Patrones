#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "utils.hpp"
#include <filesystem>
namespace fs = std::filesystem;
// #include "animation.hpp"

#define BUFFER_SIZE 1024 // Tamaño del buffer para lectura de archivos

using namespace std;

// Variable estática para almacenar el tiempo de inicio del temporizador
static chrono::high_resolution_clock::time_point startTime;


// Función para leer archivos de una carpeta y devolver su contenido como una cadena separada por "$"
// Si cantidadArchivos es -1, se leen todos los archivos de la carpeta
string readFile(const string &carpeta, int cantidadArchivos) {
    
    string txt = ""; // Variable para almacenar el contenido del archivo

    for (const auto & entry : fs::directory_iterator(carpeta)){

        if (cantidadArchivos == 0) break; // Si se ha alcanzado la cantidad de archivos, salir del bucle
        cout << "Leyendo archivo: " << entry.path().filename() << endl; // Mostrar el nombre del archivo que se está leyendo
        
        if (!fs::is_regular_file(entry.path()))     continue; // Solo archivos


        if (!fs::exists(entry.path())) {
            cerr << "El archivo no existe: " << entry.path() << endl;
            return "";
        }
        
        ifstream file(entry.path());
        
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo: " << entry.path() << endl;
            return "";
        }

        txt = txt + "$" + string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        cantidadArchivos--; // Decrementar la cantidad de archivos restantes   
    }
    return txt; // Devolver el contenido del archivo como una cadena
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
    //auto durationSeconds = chrono::duration_cast<chrono::seconds>(endTime - startTime);
    //cout << "Tiempo transcurrido: " << duration.count() << " nanosegundos (" << durationSeconds.count() << " segundos)." << endl;

    if (duration.count() == 0) cerr << "ADVERTENCIA: El tiempo medido es igual a 0 ns." << endl;

    cout << duration.count() << endl; // Imprimir solo el tiempo en nanosegundos para la exportación a CSV
    startTime = endTime; // Reiniciar el temporizador para la próxima vez
}