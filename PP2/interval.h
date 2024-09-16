#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
  public:
    double min, max;  // Os limites inferior (min) e superior (max) do intervalo.

    // Construtor padrão que cria um intervalo vazio, onde min é +infinito e max é -infinito.
    interval() : min(+infinity), max(-infinity) {}

    // Construtor que inicializa o intervalo com valores mínimos e máximos específicos.
    interval(double min, double max) : min(min), max(max) {}

    // Retorna o tamanho do intervalo, ou seja, a diferença entre max e min.
    double size() const {
        return max - min;
    }

    // Verifica se um valor x está dentro do intervalo, incluindo os limites.
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Verifica se um valor x está estritamente dentro do intervalo, excluindo os limites.
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Função que "trava" um valor x dentro dos limites do intervalo. Se x for menor que min, retorna min;
    // se for maior que max, retorna max; caso contrário, retorna o próprio valor x.
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    // Definição de dois intervalos estáticos comuns: empty (vazio) e universe (todo o espaço).
    static const interval empty, universe;
};

// Inicializa o intervalo vazio como tendo +infinito para min e -infinito para max.
// Isso significa que ele não contém nenhum número real.
const interval interval::empty    = interval(+infinity, -infinity);

// Inicializa o intervalo universo como abrangendo todos os números reais, com -infinito para min e +infinito para max.
const interval interval::universe = interval(-infinity, +infinity);

#endif
