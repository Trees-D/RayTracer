#include <core/camera/camera.h>
#include <core/shapes/shapes.h>
#include <core/scene/scene.h>
#include <core/tracer/raytracer.h>
#include <core/materials/materials.h>
#include <core/textures/solidcolor.h>
#include <core/textures/picture.h>
#include <core/lights/diffuselight.h>
#include <utils/utils.h>
#include <math/math.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        Utils::Logger::error("[ERROR] function `main(argc, argv)`:\n\t./main [output path] [sample number]\n");
    }
    Utils::Image img(1080, 540);
    // Utils::Image img(1920, 1080);
    // Utils::Image img(4096, 2160);
    float aspect_radio = static_cast<float>(img.width()) / img.height();
    Math::vec3 position = {0, 30, 35};
    Math::vec3 look_at = {0, 20, 0};
    Math::vec3 vup = {0, 1, 0};

    Core::Camera cam(position, look_at, vup, 60, aspect_radio, 0.1, glm::length(position - look_at));
    Utils::pointer<Core::Scene> scene_ptr = Utils::new_ptr<Core::Scene>();
    scene_ptr->set_camera(cam);

    Math::rgb color1 = Math::cast_to_rgb("#B5A289");
    Math::rgb color2 = Math::cast_to_rgb("#8F1E33");
    Math::rgb color3 = Math::cast_to_rgb("#3F568B");
    Math::rgb color4 = Math::cast_to_rgb("#103A25");

    scene_ptr->add(Utils::new_ptr<Core::Rectangle>(
        Core::Vertex(Math::vec3(-32, 0, -32), Math::vec2(0, 1)),
        Core::Vertex(Math::vec3(-32, 0, 32), Math::vec2(0, 0)),
        Core::Vertex(Math::vec3(32, 0, -32), Math::vec2(1, 1)),
        Core::Vertex(Math::vec3(32, 0, 32), Math::vec2(1, 0)),
        Utils::new_ptr<Core::Metal>(Utils::new_ptr<Core::PictureTexture>("./sources/1.jpg"), 0.8)
    ));

    scene_ptr->add(Utils::new_ptr<Core::Rectangle>(
        Core::Vertex(Math::vec3(-32, 64, -32), Math::vec2(0, 1)),
        Core::Vertex(Math::vec3(-32, 0, -32), Math::vec2(0, 0)),
        Core::Vertex(Math::vec3(32, 64, -32), Math::vec2(1, 1)),
        Core::Vertex(Math::vec3(32, 0, -32), Math::vec2(1, 0)),
        Utils::new_ptr<Core::Lambertian>(Utils::new_ptr<Core::PictureTexture>("./sources/2.png"))
    ));

    scene_ptr->add(Utils::new_ptr<Core::Rectangle>(
        Core::Vertex(Math::vec3(-32, 64, 32), Math::vec2(0, 1)),
        Core::Vertex(Math::vec3(-32, 0, 32), Math::vec2(0, 0)),
        Core::Vertex(Math::vec3(-32, 64, -32), Math::vec2(1, 1)),
        Core::Vertex(Math::vec3(-32, 0, -32), Math::vec2(1, 0)),
        Utils::new_ptr<Core::Lambertian>(Utils::new_ptr<Core::PictureTexture>("./sources/3.jpg"))
    ));

    scene_ptr->add(Utils::new_ptr<Core::Rectangle>(
        Core::Vertex(Math::vec3(32, 64, -32), Math::vec2(0, 1)),
        Core::Vertex(Math::vec3(32, 0, -32), Math::vec2(0, 0)),
        Core::Vertex(Math::vec3(32, 64, 32), Math::vec2(1, 1)),
        Core::Vertex(Math::vec3(32, 0, 32), Math::vec2(1, 0)),
        Utils::new_ptr<Core::Lambertian>(Utils::new_ptr<Core::PictureTexture>("./sources/4.png"))
    ));

    scene_ptr->add(Utils::new_ptr<Core::Rectangle>(
        Core::Vertex(Math::vec3(32, 64, 32), Math::vec2(0, 1)),
        Core::Vertex(Math::vec3(32, 0, 32), Math::vec2(0, 0)),
        Core::Vertex(Math::vec3(-32, 64, 32), Math::vec2(1, 1)),
        Core::Vertex(Math::vec3(-32, 0, 32), Math::vec2(1, 0)),
        Utils::new_ptr<Core::Lambertian>(Utils::new_ptr<Core::PictureTexture>("./sources/flower.png"))
    ));

    scene_ptr->add(Utils::new_ptr<Core::Sphere>(
        Math::vec3(0, 8, 0), 8,
        Utils::new_ptr<Core::Dielectric>(2.4)
    ));

    scene_ptr->add(Utils::new_ptr<Core::Sphere>(
        Math::vec3(0, 200, 0), 100,
        Utils::new_ptr<Core::DiffuseLight>(Math::rgb(4, 4, 4))
    ));

    Math::rgb rainbow_colors[8] = {
        Math::cast_to_rgb("#D0104C"),
        Math::cast_to_rgb("#E83015"),
        Math::cast_to_rgb("#FFB11B"),
        Math::cast_to_rgb("#90B44B"),
        Math::cast_to_rgb("#66BAB7"),
        Math::cast_to_rgb("#58B2DC"),
        Math::cast_to_rgb("#6F3381"),
        Math::cast_to_rgb("#1C1C1C")
    };
    for (int i = 0; i < 8; ++ i) {
        scene_ptr->add(Utils::new_ptr<Core::Sphere>(
            Math::vec3(-28 + i * 8, 4, -28), 4,
            Utils::new_ptr<Core::Lambertian>(Utils::new_ptr<Core::SolidColorTexture>(rainbow_colors[i]))
        ));
    }


    Utils::pointer<Core::RayTracer> tracer = Utils::new_ptr<Core::RayTracer>(scene_ptr);
    tracer->rendering(img, std::atoi(argv[2]));
    img.save(argv[1]);
    return 0;
}