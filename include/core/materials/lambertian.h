#ifndef DA_CORE_MATERIALS_LAMBERTIAN_HEADER
#define DA_CORE_MATERIALS_LAMBERTIAN_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/materials/material.h"
#include "core/textures/texture.h"

namespace Core
{

class Lambertian: public Material
{
public:
    Lambertian(const Utils::pointer<Texture> &texture) noexcept;
    Lambertian(const Lambertian &rhs) noexcept;

    virtual ~Lambertian() noexcept = default;

    Lambertian &operator=(const Lambertian &rhs) noexcept;

    Math::rgb emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;

    bool scatter(const Math::Ray &rIn, const HitRecord &record
                , Math::rgb &attenuation, Math::Ray &rOut) const noexcept override;
private:
    Utils::pointer<Texture> m_albedo;
};

inline Lambertian::Lambertian(const Utils::pointer<Texture> &texture) noexcept
    : m_albedo(texture) {}

inline Lambertian::Lambertian(const Lambertian &rhs) noexcept
    : m_albedo(rhs.m_albedo) {}

inline Lambertian &Lambertian::operator=(const Lambertian &rhs) noexcept
{
    m_albedo = rhs.m_albedo;
    return *this;
}

inline Math::rgb Lambertian::emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return {0, 0, 0};
}

inline bool Lambertian::scatter(const Math::Ray &rIn, const HitRecord &record
                            , Math::rgb &attenuation, Math::Ray &rOut) const noexcept
{
    rOut = Math::Ray(
        record.point,
        glm::normalize(Math::Sampler::uniform_on_hemishpere(record.normal))
    );
    attenuation = m_albedo->evaluate(record.uv, record.point);
    return true;
}

}

#endif