#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
  public:

    // Parâmetros padrão da câmera
    double aspect_ratio      = 1.0;  
    int    image_width       = 100;  
    int    samples_per_pixel = 10;   // Número de amostras aleatórias para cada pixel
    int    max_depth         = 10;   // Número máximo de vezes que um raio pode refratar ou refletir

    double vfov     = 90;              // Campo de visão vertical

    // Pontos de definição de visão da câmera
    point3 lookfrom = point3(0,0,0);
    point3 lookat   = point3(0,0,-1);  
    vec3   vup      = vec3(0,1,0);

    // Manipuladores de foco
    double defocus_angle = 0;  
    double focus_dist = 10;    

    // Essa função percorre cada pixel da imagem disparando um ou mais raios para determinar a cor do pixel
    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rLinhas restantes para escanear: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j); // Construção dos raios da câmera em i,j
                    pixel_color += ray_color(r, max_depth, world); // Gera as cores para os raios até max_depth
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rTerminoou :)                 \n";
    }

  private:
    int    image_height;         
    double pixel_samples_scale;  // Fator de escala de cor

    //  Localização de câmera
    point3 center;               
    point3 pixel00_loc;    

    vec3   pixel_delta_u;       
    vec3   pixel_delta_v;        
    vec3   u, v, w;              
    vec3   defocus_disk_u;      
    vec3   defocus_disk_v;       

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Cálculos para dimensões de viewport
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        w = unit_vector(lookfrom - lookat); // Vetor para trás
        u = unit_vector(cross(vup, w)); // Vetor para direita 
        v = cross(w, u); // Vetor para cima

        
        vec3 viewport_u = viewport_width * u;    
        vec3 viewport_v = viewport_height * -v;  

        // Cálculo de deslocamento dos pixels
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Cálculo de pixel na top-left
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Retorna ponto aleatório no pixel (amostragem de anti-aliasing)
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    vec3 sample_disk(double radius) const {
        // Retorna ponto aleatório baseado no radius
        return radius * random_in_unit_disk();
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        
        // Condição de parada da recursão -> Alinhada ao conceito de vec3
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};


#endif
