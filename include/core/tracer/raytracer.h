#ifndef DA_CORE_TRACER_RAYTRACER_HEADER
#define DA_CORE_TRACER_RAYTRACER_HEADER

#include "core/scene/scene.h"
#include "core/materials/material.h"
#include "math/math.h"
#include "utils/utils.h"

#include <omp.h>

namespace Core
{

class RayTracer
{
public:
    RayTracer(const Utils::pointer<Scene> &scene, bool accelerated = true) noexcept;

    virtual ~RayTracer() noexcept = default;

    Math::rgb trace(const Math::Ray &ray) const noexcept;

    void rendering(Utils::Image &image, size_t sample_number = 5000) const noexcept;
public:
    Utils::pointer<Scene> scene;
    
private:
    RayTracer(const RayTracer &rhs) noexcept = delete;
    RayTracer &operator=(const RayTracer &rhs) noexcept = delete;
};

inline RayTracer::RayTracer(const Utils::pointer<Scene> &scene, bool accelerated) noexcept
    : scene(scene)
{
    if (accelerated)
        scene->build_tree();
}

inline Math::rgb RayTracer::trace(const Math::Ray &ray) const noexcept
{
    static constexpr float p_rr = 0.8;
    float p = Math::Sampler::uniform01();
    if (p > p_rr)
        return {0, 0, 0};

    Math::rgb background;
    float t = (ray.dir.y + 1) * 0.5;
    // background = Math::lerp(Math::rgb(1, 1, 1), Math::rgb(0.5, 0.8, 1), t);
    background = {0, 0, 0};

    HitRecord record;

    if (!scene->intersect(ray, Math::EPS, Math::INF, record))
        return background / p_rr;

    Math::rgb attenuation;
    Math::Ray new_ray;
    Math::rgb emitted = record.material->emitted(record.uv, record.point);

    if (!record.material->scatter(ray, record, attenuation, new_ray))
        return emitted / p_rr;

    return (emitted + attenuation * trace(new_ray)) / p_rr;

}

inline void RayTracer::rendering(Utils::Image &image, size_t sample_number) const noexcept
{
    float inv_sample = 1.f / sample_number;
    Math::rgb pixel;
#pragma omp parallel for schedule(dynamic, 1) private(pixel)
    for (int j = 0; j < image.height(); ++ j) {
        for (int i = 0; i < image.width(); ++ i) {
            pixel = {0, 0, 0};
            for (int k = 0; k < sample_number; ++ k) {
                float s = (i + Math::Sampler::uniform01()) / image.width();
                float t = (j + Math::Sampler::uniform01()) / image.height();
                Math::Ray ray = scene->camera(s, t);
                pixel += trace(ray) * inv_sample;
            }
            image.set(i, j, Math::rgba(pixel, 0));
        }
    }
}

}

#endif