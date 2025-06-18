#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
string readFolder(const string &carpeta, vector<int> *posiciones, int cantidadArchivos = -1);
string readFile(const string &archivo);
void encuentros_por_archivo(const string &carpeta, const vector<int> &posiciones, const vector<int> &pos_final_archivos);
void startTimer();
void stopTimer();

#endif