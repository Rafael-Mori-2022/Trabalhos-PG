#ifndef MAIN_LIB_H
#define MAIN_LIB_H

#include <cmath>       // Biblioteca para funções matemáticas (ex: sqrt, sin, cos).
#include <cstdlib>     // Biblioteca padrão para funções de manipulação de memória e geração de números aleatórios.
#include <iostream>    // Biblioteca para entrada e saída padrão.
#include <limits>      // Fornece os limites numéricos dos tipos de dados.
#include <memory>      // Biblioteca para gerenciamento de memória dinâmica (ex: smart pointers).


// Usings para facilitar o uso de tipos da biblioteca padrão
using std::make_shared;    // Facilita a criação de smart pointers (shared_ptr).
using std::shared_ptr;     // Define smart pointers que podem compartilhar a posse do recurso.

// Constantes
const double infinity = std::numeric_limits<double>::infinity(); // Define o valor do infinito.
const double pi = 3.1415926535897932385; // Define o valor de pi.

// Funções utilitárias

// Converte graus em radianos.
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Retorna um número aleatório entre [0, 1).
inline double random_double() {
    // Utiliza a função rand() para gerar um valor aleatório.
    return std::rand() / (RAND_MAX + 1.0);
}

// Retorna um número aleatório no intervalo [min, max).
inline double random_double(double min, double max) {
    // Gera um número aleatório no intervalo desejado usando a função random_double().
    return min + (max - min) * random_double();
}

// Headers comuns
#include "color.h"     // Inclui funções e definições relacionadas a cores.
#include "interval.h"  // Inclui funções para trabalhar com intervalos numéricos.
#include "ray.h"       // Inclui a definição da classe de raios.
#include "vec3.h"      // Inclui a classe para vetores em 3 dimensões.

#endif