#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h" 

#include <vector>  

// Representação de lista de objetos que podem ser atingidos por um raio.
class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;  // Vetor de ponteiros compartilhados para objetos hittable. Armazena a lista de objetos na cena.

    // Construtores
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); } // Construtor que recebe um objeto e já adiciona à lista.

    // Função para limpar a lista de objetos.
    void clear() { objects.clear(); }

    // Função para adicionar um novo objeto à lista.
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);  // Adiciona o objeto ao vetor de objetos.
    }

    // Função override que verifica se um raio atinge algum objeto da lista.
    // A função herda de hittable e precisa ser implementada. Ela retorna verdadeiro se algum objeto é atingido.
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;  // Variável temporária para armazenar os dados de hit de cada objeto.
        bool hit_anything = false;  // Variável booleana que armazena se algum objeto foi atingido.
        auto closest_so_far = ray_t.max;  // Mantém a distância mínima de colisão para garantir que pegamos o objeto mais próximo.

        // Loop que percorre todos os objetos na lista.
        for (const auto& object : objects) {
            // Verifica se o raio atinge o objeto atual e se está mais perto do que o objeto anterior.
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;  // Define que houve uma colisão.
                closest_so_far = temp_rec.t;  // Atualiza a distância mais próxima para a colisão mais recente.
                rec = temp_rec;  // Atualiza o registro de hit com os dados do objeto atingido.
            }
        }

        return hit_anything;  // Retorna verdadeiro se o raio atingiu qualquer objeto da lista.
    }
};

#endif
