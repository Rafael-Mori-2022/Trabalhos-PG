#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "texture.h"
#include "sphere.h"

// Função para configurar e renderizar a cena
void scene_with_textured_cubes() {
    hittable_list world;

    // Criar as texturas das imagens e os materiais
    auto side_texture = make_shared<image_texture>("blocomine.jpg");  // Textura para as laterais dos cubos 2x2
    auto top_texture = make_shared<image_texture>("grama.jpg");       // Textura para o topo dos cubos 2x2

    auto side_material = make_shared<lambertian>(side_texture);
    auto top_material = make_shared<lambertian>(top_texture);

    // Texturas para o cubo adicional
    auto side_texture_new = make_shared<image_texture>("ladobau.png");  // Lado do cubo adicional
    auto front_texture_new = make_shared<image_texture>("frentebau.png"); // Frente do cubo adicional
    auto top_texture_new = make_shared<image_texture>("topobau.png");  // Topo do cubo adicional

    auto side_material_new = make_shared<lambertian>(side_texture_new);
    auto front_material_new = make_shared<lambertian>(front_texture_new);
    auto top_material_new = make_shared<lambertian>(top_texture_new);

    // Parâmetros dos cubos
    float cubo_size = 1.0;
    float cubo_spacing = 0.01; // Espaçamento entre cubos

    // Adicionando cubos em uma formação 2x2
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 2; ++col) {
            float x = col * (cubo_size + cubo_spacing);
            float z = row * (cubo_size + cubo_spacing);

            // Adicionar os seis lados do cubo com as texturas apropriadas
            // Frente
            world.add(make_shared<quad>(point3(x, 0, z), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material));
            // Traseira
            world.add(make_shared<quad>(point3(x, 0, z + cubo_size), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material));
            // Esquerda
            world.add(make_shared<quad>(point3(x, 0, z), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material));
            // Direita
            world.add(make_shared<quad>(point3(x + cubo_size, 0, z), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material));
            // Topo
            world.add(make_shared<quad>(point3(x, cubo_size, z), vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), top_material));
            // Base
            world.add(make_shared<quad>(point3(x, 0, z), vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), side_material));
        }
    }

    // Adicionando o cubo adicional acima do cubo da direita
    // Adicionando o cubo adicional acima do cubo da direita
// Adicionando o cubo adicional acima do cubo da direita
// Adicionando o cubo adicional acima do cubo da direita
float x_above = 1 * (cubo_size + cubo_spacing); // X do cubo da direita
float z_above = 0 * (cubo_size + cubo_spacing); // Z do cubo da direita
float y_above = cubo_size + cubo_spacing;       // Acima do cubo da direita

// Adicionar os seis lados do cubo adicional com as texturas apropriadas
// Frente (ladobau.png) - Agora a frente será o lado do baú
world.add(make_shared<quad>(point3(x_above, y_above, z_above), 
    vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material_new)); // Face frontal ajustada

// Traseira (frentebau.png) - Adicionar "frentebau.png" na traseira
world.add(make_shared<quad>(point3(x_above, y_above, z_above + cubo_size), 
    vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), front_material_new)); // Face traseira com frentebau

// Esquerda (ladobau.png)
world.add(make_shared<quad>(point3(x_above, y_above, z_above), 
    vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new));

// Direita (ladobau.png)
world.add(make_shared<quad>(point3(x_above + cubo_size, y_above, z_above), 
    vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new));

// Topo (topobau.png)
world.add(make_shared<quad>(point3(x_above, y_above + cubo_size, z_above), 
    vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), top_material_new));

// Remover a face base que estava fora do lugar e não é necessária

 auto sphere_texture = make_shared<image_texture>("enderpearl.png"); // Carregar a textura da esfera
 auto sphere_material = make_shared<lambertian>(sphere_texture); // Aplicar a textura à esfera
// Adicionar uma esfera no centro da cena
point3 sphere_center(0.5, 1.5, 1); // Posição centralizada da esfera
float sphere_radius = 0.3; // Raio da esfera ajustado para um tamanho visível
world.add(make_shared<sphere>(sphere_center, sphere_radius, sphere_material)); // Adicionando a esfera


    // Configuração da câmera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);
    cam.vfov = 20;
    cam.lookfrom = point3(6, 6, 8);  // Ajustar a posição da câmera para ver o cubo adicional
    cam.lookat = point3(1, 1, 1);    // Centro da formação
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    // Renderizar a cena
    cam.render(world);
}

int main() {
    scene_with_textured_cubes();
}
