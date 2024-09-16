#ifndef VEC3_H
#define VEC3_H

// Definição da classe base para manipulação de cores, localização, direção, etc.
class vec3 {
  public:
    double e[3]; // Definição de Array das componentes (x, y, z) ou (r, g, b)

    // Construtores
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // Métodos de acesso a informação
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // Inversão do vetor

    // Operadores de indexação
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; } // Viabiliza escrita

    // Adição de vetores
    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    // Multiplicação por escalar t
    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Divisão por escalar t
    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    // Comprimento do vetor
    double length() const {
        return std::sqrt(length_squared());
    }

    // Quadrado do comprimento do vetor 
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // Função voltada a evitar erros de precisão
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    // Randomização entre 0 e 1
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    // Randomização entre min e max
    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// Aliasing para pontos
using point3 = vec3;

// Adição de vetores
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Subtração de vetores
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Multiplicação de vetores 
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Multiplicação de vetor por um escalar
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

// Divisão por escalar
inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

// Produto vetorial 
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// Cross product
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Normalização do vetor
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

// Retorno de vetor aleatório de um disco unitário
inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

// Retorno de vetor aleatório
inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1.0)
            return p / sqrt(lensq);
    }
}

// Gera um vetor aleatório no hemisfério definido pelo vetor normal
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

// Reflete um vetor v em relação à reta normal n
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

// Função de refração entre materiais
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) { // etai_over_etat = razão entre índices de refração
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n); // Cálculo de perpendicular
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n; // Cálculo de paralelo 
    return r_out_perp + r_out_parallel; // Combinação das componentes
}


#endif