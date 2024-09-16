#ifndef HITTABLE_H
#define HITTABLE_H

class material;  

// Classe que armazena as informações sobre a colisão de um raio com um objeto (hit record).
class hit_record {
  public:
    point3 p;  // Ponto no espaço onde o raio atinge o objeto.
    vec3 normal;  // Vetor normal à superfície no ponto de colisão, utilizado para calcular a iluminação.
    shared_ptr<material> mat;  // Ponteiro para o material do objeto que foi atingido, utilizado para calcular a interação luz/material.
    double t;  // Parâmetro que representa a "distância" ao longo do raio onde a colisão ocorreu (valor de t na equação do raio).
    bool front_face;  // Indica se o raio atingiu a parte frontal ou traseira da superfície (para controlar a direção da normal).

    // Função que ajusta a normal do objeto atingido dependendo da direção do raio.
    // Isso garante que a normal sempre aponte para fora da superfície (para frente).
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Verifica se o raio atinge a parte frontal ou traseira da superfície.
        front_face = dot(r.direction(), outward_normal) < 0;
        
        // Define a normal com base na face atingida. Se o raio atingir a parte frontal, a normal será a original.
        // Caso contrário, inverte-se a direção da normal.
        normal = front_face ? outward_normal : -outward_normal;
    }
};


// Classe abstrata que representa qualquer objeto que pode ser atingido por um raio.
class hittable {
  public:
    virtual ~hittable() = default;  // Destrutor virtual padrão. Garante que destrutores de classes derivadas sejam chamados corretamente.

    // Função virtual pura que deve ser implementada nas classes derivadas.
    // Determina se o raio atinge o objeto e, em caso afirmativo, preenche os detalhes da colisão no hit_record.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
