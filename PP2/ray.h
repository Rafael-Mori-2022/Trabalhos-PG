#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  public:

    // Construtores
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {} // Construtor que inicializa a origem e a direção do raio.

    // Função que retorna a origem do raio.
    const point3& origin() const { 
        return orig; 
    }

    // Função que retorna a direção do raio.
    const vec3& direction() const { 
        return dir; 
    }

    // Função que retorna a posição do ponto em que o raio está após percorrer uma distância t.
    // A equação usada aqui é: P(t) = origem + t * direção, onde t é um escalar.
    point3 at(double t) const {
        return orig + t * dir;
    }

  private:
    point3 orig; // A origem do raio, representada como um ponto 3D.
    vec3 dir; // A direção do raio, representada como um vetor 3D.
};


#endif