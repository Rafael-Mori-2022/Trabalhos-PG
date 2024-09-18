// Essa implementação conta com códigos do repositório: https://github.com/RayTracing/raytracing.github.io
// As biblotecas foram implementadas por Peter Shirley em 2016!

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

    auto wood = make_shared<lambertian>(color(0.5, 0.25, 0.1));   
    auto leaves = make_shared<lambertian>(color(0.1, 0.8, 0.1));  

    // Criar as texturas das imagens e os materiais
    auto side_texture = make_shared<image_texture>("img/blocomine.jpg");  // Textura para as laterais dos cubos
    auto top_texture = make_shared<image_texture>("img/grama.jpg");       // Textura para o topo dos cubos

    auto side_material = make_shared<lambertian>(side_texture);
    auto top_material = make_shared<lambertian>(top_texture);

    // Texturas para o cubo adicional (baú)
    auto side_texture_new = make_shared<image_texture>("img/ladobau.png");  // Lado do cubo adicional
    auto front_texture_new = make_shared<image_texture>("img/frentebau.png"); // Frente do cubo adicional
    auto top_texture_new = make_shared<image_texture>("img/topobau.png");  // Topo do cubo adicional

    auto side_material_new = make_shared<lambertian>(side_texture_new);
    auto front_material_new = make_shared<lambertian>(front_texture_new);
    auto top_material_new = make_shared<lambertian>(top_texture_new);

    // Parâmetros dos cubos
    float cubo_size = 1.0;
    float cubo_spacing = 0.01; // Espaçamento entre cubos

    // Adicionando blocos de terra na formação 3x3
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            float x = col * (cubo_size + cubo_spacing);
            float z = row * (cubo_size + cubo_spacing);

            // Adicionar blocos de terra em todas as posições
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

    // Adicionar a Enderpearl no canto superior esquerdo
    auto sphere_texture = make_shared<image_texture>("img/enderpearl.png");
    auto sphere_material = make_shared<lambertian>(sphere_texture);
    point3 sphere_center(0.5, cubo_size + 0.5, 0.5); // Ajustado para o canto superior esquerdo, acima do bloco de terra
    float sphere_radius = 0.3;
    world.add(make_shared<sphere>(sphere_center, sphere_radius, sphere_material));

    // Adicionar a Fireball no canto inferior direito
    auto fireball_texture = make_shared<image_texture>("img/fireball.png");
    auto fireball_material = make_shared<lambertian>(fireball_texture);
    point3 fireball_center(2.5, cubo_size + 0.5, 2.5); // Posição ajustada para o canto inferior direito, acima do bloco de terra
    float fireball_radius = 0.3;
    world.add(make_shared<sphere>(fireball_center, fireball_radius, fireball_material));

    // Adicionar o baú no canto superior direito
    float x_above = 2 * (cubo_size + cubo_spacing); // X do canto superior direito
    float z_above = 0 * (cubo_size + cubo_spacing); // Z do canto superior direito
    float y_above = cubo_size;                      // Altura do baú (em cima do bloco de terra)

    // Adicionar os lados do cubo do baú
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material_new)); // Frente
    world.add(make_shared<quad>(point3(x_above, y_above, z_above + cubo_size), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), front_material_new)); // Traseira
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Esquerda
    world.add(make_shared<quad>(point3(x_above + cubo_size, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Direita
    world.add(make_shared<quad>(point3(x_above, y_above + cubo_size, z_above), vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), top_material_new)); // Topo

    // Adicionar a árvore no canto inferior esquerdo e centralizar no bloco
    point3 tree_base(0.5 * cubo_size, cubo_size, 2.5 * (cubo_size + cubo_spacing)); // Posição mais centralizada no bloco de terra
    world.add(box(tree_base + vec3(-0.1, 0, -0.1), tree_base + vec3(0.1, 1.2, 0.1), wood)); // Tronco da árvore, agora centralizado
    world.add(box(tree_base + vec3(-0.4, 1.2, -0.4), tree_base + vec3(0.4, 1.6, 0.4), leaves));  // Folhas inferior
    world.add(box(tree_base + vec3(-0.3, 1.6, -0.3), tree_base + vec3(0.3, 1.9, 0.3), leaves));  // Folhas intermediária
    world.add(box(tree_base + vec3(-0.2, 1.9, -0.2), tree_base + vec3(0.2, 2.1, 0.2), leaves));  // Folhas superior

    // Adicionar uma fonte de luz emissiva (simulação de luz)
    auto light_material = make_shared<lambertian>(color(4, 4, 4)); // Luz mais brilhante
    world.add(make_shared<sphere>(point3(5, 5, -5), 1.0, light_material)); // Luz vinda da direita

    // Configuração da câmera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);
    cam.vfov = 20;
    cam.lookfrom = point3(6, 6, 8);
    cam.lookat = point3(1.5, 1, 1.5); // Centralizado na formação
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    // Renderizar a cena
    cam.render(world);
}

// Função para configurar e renderizar a cena
void scene_with_inverted_spheres() {
    hittable_list world;

    auto wood = make_shared<lambertian>(color(0.5, 0.25, 0.1));   
    auto leaves = make_shared<lambertian>(color(0.1, 0.8, 0.1));  

    // Criar as texturas das imagens e os materiais
    auto side_texture = make_shared<image_texture>("img/blocomine.jpg");  // Textura para as laterais dos cubos
    auto top_texture = make_shared<image_texture>("img/grama.jpg");       // Textura para o topo dos cubos

    auto side_material = make_shared<lambertian>(side_texture);
    auto top_material = make_shared<lambertian>(top_texture);

    // Texturas para o cubo adicional (baú)
    auto side_texture_new = make_shared<image_texture>("img/ladobau.png");  // Lado do cubo adicional
    auto front_texture_new = make_shared<image_texture>("img/frentebau.png"); // Frente do cubo adicional
    auto top_texture_new = make_shared<image_texture>("img/topobau.png");  // Topo do cubo adicional

    auto side_material_new = make_shared<lambertian>(side_texture_new);
    auto front_material_new = make_shared<lambertian>(front_texture_new);
    auto top_material_new = make_shared<lambertian>(top_texture_new);

    // Parâmetros dos cubos
    float cubo_size = 1.0;
    float cubo_spacing = 0.01; // Espaçamento entre cubos

    // Adicionando blocos de terra na formação 3x3
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            float x = col * (cubo_size + cubo_spacing);
            float z = row * (cubo_size + cubo_spacing);

            // Adicionar blocos de terra em todas as posições
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

    // Adicionar a Enderpearl no canto superior esquerdo
    auto sphere_texture = make_shared<image_texture>("img/enderpearl.png");
    auto sphere_material = make_shared<lambertian>(sphere_texture);
    point3 sphere_center(2.5, cubo_size + 0.5, 2.5); // Ajustado para o canto superior esquerdo, acima do bloco de terra
    float sphere_radius = 0.3;
    world.add(make_shared<sphere>(sphere_center, sphere_radius, sphere_material));

    // Adicionar a Fireball no canto inferior direito
    auto fireball_texture = make_shared<image_texture>("img/fireball.png");
    auto fireball_material = make_shared<lambertian>(fireball_texture);
    point3 fireball_center(0.5, cubo_size + 0.5, 0.5); // Posição ajustada para o canto inferior direito, acima do bloco de terra
    float fireball_radius = 0.3;
    world.add(make_shared<sphere>(fireball_center, fireball_radius, fireball_material));

    // Adicionar o baú no canto superior direito
    float x_above = 2 * (cubo_size + cubo_spacing); // X do canto superior direito
    float z_above = 0 * (cubo_size + cubo_spacing); // Z do canto superior direito
    float y_above = cubo_size;                      // Altura do baú (em cima do bloco de terra)

    // Adicionar os lados do cubo do baú
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material_new)); // Frente
    world.add(make_shared<quad>(point3(x_above, y_above, z_above + cubo_size), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), front_material_new)); // Traseira
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Esquerda
    world.add(make_shared<quad>(point3(x_above + cubo_size, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Direita
    world.add(make_shared<quad>(point3(x_above, y_above + cubo_size, z_above), vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), top_material_new)); // Topo

    // Adicionar a árvore no canto inferior esquerdo e centralizar no bloco
    point3 tree_base(0.5 * cubo_size, cubo_size, 2.5 * (cubo_size + cubo_spacing)); // Posição mais centralizada no bloco de terra
    world.add(box(tree_base + vec3(-0.1, 0, -0.1), tree_base + vec3(0.1, 1.2, 0.1), wood)); // Tronco da árvore, agora centralizado
    world.add(box(tree_base + vec3(-0.4, 1.2, -0.4), tree_base + vec3(0.4, 1.6, 0.4), leaves));  // Folhas inferior
    world.add(box(tree_base + vec3(-0.3, 1.6, -0.3), tree_base + vec3(0.3, 1.9, 0.3), leaves));  // Folhas intermediária
    world.add(box(tree_base + vec3(-0.2, 1.9, -0.2), tree_base + vec3(0.2, 2.1, 0.2), leaves));  // Folhas superior

    // Adicionar uma fonte de luz emissiva (simulação de luz)
    auto light_material = make_shared<lambertian>(color(4, 4, 4)); // Luz mais brilhante
    world.add(make_shared<sphere>(point3(5, 5, -5), 1.0, light_material)); // Luz vinda da direita

    // Configuração da câmera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);
    cam.vfov = 20;
    cam.lookfrom = point3(6, 6, 8);
    cam.lookat = point3(1.5, 1, 1.5); // Centralizado na formação
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    // Renderizar a cena
    cam.render(world);
}

// Função para configurar e renderizar a cena
void scene_with_different_cam() {
    hittable_list world;

    auto wood = make_shared<lambertian>(color(0.5, 0.25, 0.1));   
    auto leaves = make_shared<lambertian>(color(0.1, 0.8, 0.1));  

    // Criar as texturas das imagens e os materiais
    auto side_texture = make_shared<image_texture>("img/blocomine.jpg");  // Textura para as laterais dos cubos
    auto top_texture = make_shared<image_texture>("img/grama.jpg");       // Textura para o topo dos cubos

    auto side_material = make_shared<lambertian>(side_texture);
    auto top_material = make_shared<lambertian>(top_texture);

    // Texturas para o cubo adicional (baú)
    auto side_texture_new = make_shared<image_texture>("img/ladobau.png");  // Lado do cubo adicional
    auto front_texture_new = make_shared<image_texture>("img/frentebau.png"); // Frente do cubo adicional
    auto top_texture_new = make_shared<image_texture>("img/topobau.png");  // Topo do cubo adicional

    auto side_material_new = make_shared<lambertian>(side_texture_new);
    auto front_material_new = make_shared<lambertian>(front_texture_new);
    auto top_material_new = make_shared<lambertian>(top_texture_new);

    // Parâmetros dos cubos
    float cubo_size = 1.0;
    float cubo_spacing = 0.01; // Espaçamento entre cubos

    // Adicionando blocos de terra na formação 3x3
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            float x = col * (cubo_size + cubo_spacing);
            float z = row * (cubo_size + cubo_spacing);

            // Adicionar blocos de terra em todas as posições
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

    // Adicionar a Enderpearl no canto superior esquerdo
    auto sphere_texture = make_shared<image_texture>("img/enderpearl.png");
    auto sphere_material = make_shared<lambertian>(sphere_texture);
    point3 sphere_center(2.5, cubo_size + 0.5, 2.5); // Ajustado para o canto superior esquerdo, acima do bloco de terra
    float sphere_radius = 0.3;
    world.add(make_shared<sphere>(sphere_center, sphere_radius, sphere_material));

    // Adicionar a Fireball no canto inferior direito
    auto fireball_texture = make_shared<image_texture>("img/fireball.png");
    auto fireball_material = make_shared<lambertian>(fireball_texture);
    point3 fireball_center(0.5, cubo_size + 0.5, 0.5); // Posição ajustada para o canto inferior direito, acima do bloco de terra
    float fireball_radius = 0.3;
    world.add(make_shared<sphere>(fireball_center, fireball_radius, fireball_material));

    // Adicionar o baú no canto superior direito
    float x_above = 2 * (cubo_size + cubo_spacing); // X do canto superior direito
    float z_above = 0 * (cubo_size + cubo_spacing); // Z do canto superior direito
    float y_above = cubo_size;                      // Altura do baú (em cima do bloco de terra)

    // Adicionar os lados do cubo do baú
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), side_material_new)); // Frente
    world.add(make_shared<quad>(point3(x_above, y_above, z_above + cubo_size), vec3(cubo_size, 0, 0), vec3(0, cubo_size, 0), front_material_new)); // Traseira
    world.add(make_shared<quad>(point3(x_above, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Esquerda
    world.add(make_shared<quad>(point3(x_above + cubo_size, y_above, z_above), vec3(0, 0, cubo_size), vec3(0, cubo_size, 0), side_material_new)); // Direita
    world.add(make_shared<quad>(point3(x_above, y_above + cubo_size, z_above), vec3(cubo_size, 0, 0), vec3(0, 0, cubo_size), top_material_new)); // Topo

    // Adicionar a árvore no canto inferior esquerdo e centralizar no bloco
    point3 tree_base(0.5 * cubo_size, cubo_size, 2.5 * (cubo_size + cubo_spacing)); // Posição mais centralizada no bloco de terra
    world.add(box(tree_base + vec3(-0.1, 0, -0.1), tree_base + vec3(0.1, 1.2, 0.1), wood)); // Tronco da árvore, agora centralizado
    world.add(box(tree_base + vec3(-0.4, 1.2, -0.4), tree_base + vec3(0.4, 1.6, 0.4), leaves));  // Folhas inferior
    world.add(box(tree_base + vec3(-0.3, 1.6, -0.3), tree_base + vec3(0.3, 1.9, 0.3), leaves));  // Folhas intermediária
    world.add(box(tree_base + vec3(-0.2, 1.9, -0.2), tree_base + vec3(0.2, 2.1, 0.2), leaves));  // Folhas superior

    // Adicionar uma fonte de luz emissiva (simulação de luz)
    auto light_material = make_shared<lambertian>(color(4, 4, 4)); // Luz mais brilhante
    world.add(make_shared<sphere>(point3(5, 5, -5), 1.0, light_material)); // Luz vinda da direita

    // Configuração da câmera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);
    cam.vfov = 20;
    cam.lookfrom = point3(8, 5, 2);  
    cam.lookat = point3(1, 1, 1.5);  
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    // Renderizar a cena
    cam.render(world);
}

int main() {
    
    // Por favor, rode separadamente cada uma das cenas para verificar o funcionamento adequado!
    // scene_with_textured_cubes();
    // scene_with_inverted_spheres();
    scene_with_different_cam();
           
    return 0;
}
