#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

std::string readFile(const std::string &carpeta, int cantidadArchivos = -1);
void startTimer();
void stopTimer();

#endif