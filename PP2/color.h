#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3; // Segue a estrutura base para manipulação das cores

// Realiza a correção de gama, ajustando a linearidade da luz emitida em relação à sua representação em formato digital
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component); // Correção para o espaço perceptual com fator 2

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    // Recuperação das das informações para x, y e z
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Normalização para intervalo de intensidade
    static const interval intensity(0.000, 0.999);

    // Conversão para Byte
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Escrita dos valores nos pixels
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif