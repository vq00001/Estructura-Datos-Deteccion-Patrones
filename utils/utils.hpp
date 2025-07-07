#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

string readFolder(const string &carpeta, vector<int> *posiciones, bool expresivo=true ,int cantidadArchivos = -1);
string readFile(const string &archivo, bool expresivo=true);
void encuentros_por_archivo(const string &carpeta, const vector<int> &posiciones, const vector<int> &pos_final_archivos);
void startTimer();
void stopTimer();
long long getAndStopTime();
std::vector<string> readPatterns(const string &file);
bool verifyPattern(vector<int> posiciones, const string &text, const string &pattern);
pair<long long, long long> calcularPromedioYVarianza(const vector<long long> &tiempos);

void escribirCSVTiempoConstruccion(long long tiempo_creacion, const string &algoritmo,const string &carpeta, int cant_archivos, const string &archivo="tiempo_de_creacion.csv");
#endif