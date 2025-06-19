#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>

class SimpleAnimator {
private:
    std::atomic<bool> activo{false};
    std::thread hilo;

public:
    // Iniciar animación
    void start(const std::string& mensaje = "Leyendo archivo") {
        activo = true;
        hilo = std::thread([this, mensaje]() {
            int estado = 0;
            while (activo) {
                std::string puntos = std::string(estado % 4, '.');
                std::cout << "\r" << mensaje << " " << puntos << "   " << std::flush;
                estado++;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });
    }

    // Terminar animación y limpiar línea
    void end(const std::string& mensajeFinal = "Archivo leído con éxito.") {
        activo = false;
        if (hilo.joinable()) hilo.join();

        // Sobrescribir línea anterior con mensaje final
        std::cout << "\r" << mensajeFinal << std::string(30, ' ') << std::endl;
    }
};

#endif // ANIMATION_HPP
