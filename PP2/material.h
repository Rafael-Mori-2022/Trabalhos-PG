#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h" 
// Inclui a definição da classe "hittable", que representa objetos que podem ser atingidos por raios.

// Declaração da classe abstrata material
class material {
  public:
    virtual ~material() = default;
    // Destrutor virtual para garantir que as subclasses sejam destruídas corretamente.

    // Função virtual pura que define como o material dispersa (scatter) um raio.
    // Ela será implementada nas subclasses para definir comportamentos específicos.
    virtual bool scatter(
        const ray& r_in, // Raio incidente
        const hit_record& rec, // Informações sobre o ponto de impacto
        color& attenuation, // A quantidade de luz absorvida pelo material
        ray& scattered // O raio disperso
    ) const {
        return false; // Por padrão, o material não dispersa raios.
    }
};

// Implementação do material Lambertiano (superfície difusa)
class lambertian : public material {
  public:
    // Construtor que inicializa o albedo (cor difusa do material).
    lambertian(const color& albedo) : albedo(albedo) {}

    // Sobrescreve a função scatter para definir o comportamento difuso.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        // Calcula a direção do espalhamento aleatoriamente em torno da normal da superfície.
        auto scatter_direction = rec.normal + random_unit_vector();

        // Verifica se a direção é degenerada (próxima de zero), o que pode ocorrer por problemas numéricos.
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal; // Corrige a direção degenerada.

        scattered = ray(rec.p, scatter_direction); // Define o raio disperso.
        attenuation = albedo; // Define a cor atenuada como a cor do material.
        return true; // Indica que o raio foi disperso.
    }

  private:
    color albedo; // Cor difusa do material.
};

// Implementação do material metálico
class metal : public material {
  public:
    // Construtor que inicializa a cor do metal e o fator de desfoque (fuzz).
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    // Sobrescreve a função scatter para definir o comportamento metálico (reflexão).
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        // Calcula o vetor refletido baseado na direção do raio incidente e a normal da superfície.
        vec3 reflected = reflect(r_in.direction(), rec.normal);

        // Aplica o desfoque (fuzz), que adiciona aleatoriedade à reflexão.
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

        scattered = ray(rec.p, reflected); // Define o raio refletido.
        attenuation = albedo; // Define a cor atenuada como a cor do metal.
        
        // O raio é disperso somente se estiver na mesma direção da normal da superfície.
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    color albedo; // Cor do metal.
    double fuzz; // Fator de desfoque (borrão) da reflexão.
};

// Implementação do material dielétrico (materiais como vidro ou água)
class dielectric : public material {
  public:
    // Construtor que inicializa o índice de refração do material.
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    // Sobrescreve a função scatter para definir o comportamento de refração e reflexão.
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0); // A luz não é absorvida (transparente).

        // Determina o índice de refração dependendo de qual lado da superfície o raio está.
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        // Normaliza a direção do raio incidente.
        vec3 unit_direction = unit_vector(r_in.direction());

        // Calcula o ângulo de incidência (coseno) e o ângulo de refração.
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        // Verifica se a refração é impossível devido à reflexão total interna.
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        // Se não for possível refratar ou se a refletância for alta, reflete o raio.
        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri); // Caso contrário, refrata.

        scattered = ray(rec.p, direction); // Define o raio refratado ou refletido.
        return true; // O raio foi disperso.
    }

  private:
    double refraction_index; // Índice de refração do material.

    // Função que calcula a refletância usando a aproximação de Schlick.
    static double reflectance(double cosine, double refraction_index) {
        // Aproximação de Schlick para calcular a probabilidade de reflexão.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine), 5);
    }
};

#endif // Fecha a proteção contra inclusões múltiplas

