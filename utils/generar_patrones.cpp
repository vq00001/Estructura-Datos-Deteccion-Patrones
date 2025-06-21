#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

#define NOMBRE_ARCHIVO  "patrones"
#define CARPETA_BASE "../archivos_de_prueba/Custom"  // carpeta para generar los patrones


void guardar_(const vector<int>& vec, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        exit(1);
    }

    for (const auto& val : vec) {
        file << val << " "; // Escribe cada valor en el archivo
    }

    file.close();
}


int generar_numero_random(long long minValue, long long maxValue){

    random_device rd;                                   // Obtiene un numero aleatorio del sistema
    mt19937 gen(rd());                                  // Inicializa el generador de numeros aleatorios
    uniform_int_distribution<> dis(minValue, maxValue); // Distribucion uniforme
    
    int numero = dis(gen); // Genera un n├║mero aleatorio en el rango especificado
    return numero;
}

void enlistar_archivos(string dir_base, vector<string> *lista_archivos){
    
    // Recorrer todos los archivos de la carpeta base, y sus subdirectorios

    if (!fs::exists(dir_base) || !fs::is_directory(dir_base)) {
        cerr << "La ruta especificada no es una carpeta o no existe: " << dir_base << endl;
        return;
    }
    for(const auto & entry : fs::directory_iterator(dir_base)){
        
        if (fs::is_directory(entry.path())) {
            enlistar_archivos(entry.path().string(), lista_archivos);
            continue;
        };

        if (!fs::exists(entry.path()) || !fs::is_regular_file(entry.path())) {
            continue;
        }

        lista_archivos->push_back(entry.path().string());
    }
}

void patron_existente(string nombre_archivo, int cant_patrones){

    // Hacer una lista con todos los archivos en archivos_de_prueba y sus subdirectorios
    vector<string> lista_archivos;
    enlistar_archivos(CARPETA_BASE, &lista_archivos);
    
    // Verificar que archivo de destino 
    ofstream file_resultados(nombre_archivo);
    if (!file_resultados.is_open()) {
        exit(1);
    }
    
    for(int i = 0 ; i < cant_patrones; i++){

        // Abrir archivo aleatorio
        int archivo = generar_numero_random(0, lista_archivos.size() - 1);

        ifstream file(lista_archivos[archivo], ios::ate);
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            std::cerr << "Archivo: " << lista_archivos[archivo] << std::endl;
            // Si no se pudo abrir el archivo, continuar con el siguiente
            i++;
            continue;
        }

        long long tamaño_file = file.tellg();                               
        int posicion = generar_numero_random(0, tamaño_file);
        int largo_patron = generar_numero_random(1, tamaño_file - posicion);

        // ir a posicion en archivo y leer la cantidad de caracteres de largo_patron
        file.seekg(posicion); 
        vector<char> pat(largo_patron);
        file.read(pat.data(), largo_patron);
        file.close();

        // Leer los caracteres e ingresarlos a un string
        string patron = "";
        for (char i:pat) {
                patron += i;
        }

        file_resultados << patron << "\n" << static_cast<char>(28)  << "\n"; // Escribe el patron en el archivo
    }

    file_resultados.close();
}

void patron_random(string nombre_archivo, int cant_patrones){
}

int main(int argc, char* argv[]){

    if(argc < 3){
        // NombreArchivo es opcional
        printf("Uso: %s <CantidadPatrones> <TipoPatrones> <NombreArchivo>", argv[0]);
        return 1;
    }

    int cantidad_patrones = stoi(argv[1]);
    string tipo_patron = argv[2];

    if(tipo_patron == "Random"){
        patron_random(NOMBRE_ARCHIVO, cantidad_patrones);
    } else if (tipo_patron == "Existente"){
        patron_existente(NOMBRE_ARCHIVO, cantidad_patrones);
    }
}