#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
// Inclui a definição da classe `hittable`, que é a base para objetos que podem ser "atingidos" por raios.

class sphere : public hittable {
    // A classe `sphere` herda de `hittable`, o que significa que representa um objeto que pode
    // ser atingido por um raio, como uma esfera em um sistema de ray tracing.
  
  public:
    // Construtor da classe `sphere`, que inicializa o centro, raio e o material da esfera.
    sphere(const point3& center, double radius, shared_ptr<material> mat)
      : center(center), radius(std::fmax(0, radius)), mat(mat) {}
      // O construtor usa `std::fmax(0, radius)` para garantir que o raio nunca seja negativo.

    // Função `hit` que verifica se um raio atinge a esfera. Esta função sobrescreve o método da
    // classe base `hittable`.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        // Calcula o vetor da origem do raio até o centro da esfera (vetor OC).

        auto a = r.direction().length_squared();
        // Calcula o valor de `a`, que é o quadrado da magnitude da direção do raio.

        auto h = dot(r.direction(), oc);
        // `h` é o produto escalar entre a direção do raio e o vetor OC.

        auto c = oc.length_squared() - radius * radius;
        // `c` é a diferença entre o quadrado da magnitude de OC e o quadrado do raio da esfera.

        auto discriminant = h * h - a * c;
        // Calcula o discriminante, que determina se o raio atinge a esfera ou não.

        if (discriminant < 0)
            return false;
        // Se o discriminante for negativo, não há interseção entre o raio e a esfera.

        auto sqrtd = std::sqrt(discriminant);
        // Calcula a raiz quadrada do discriminante.

        // Encontra a raiz mais próxima que está no intervalo aceitável.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            // Se a primeira raiz não estiver no intervalo válido, tenta a segunda.
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Se um ponto de interseção for encontrado, atualiza o registro da interseção.
        rec.t = root;
        rec.p = r.at(rec.t); // Calcula o ponto de interseção.
        
        vec3 outward_normal = (rec.p - center) / radius;
        // Calcula a normal para o ponto de interseção (outward normal).

        rec.set_face_normal(r, outward_normal);
        // Ajusta a normal da face para garantir que ela aponte na direção correta.

        rec.mat = mat;
        // Associa o material da esfera ao registro de interseção.

        return true;
    }

  private:
    // Atributos da esfera.
    point3 center;               // O centro da esfera.
    double radius;               // O raio da esfera.
    shared_ptr<material> mat;    // Ponteiro compartilhado para o material da esfera.
};

#endif

