#ifndef DA_CORE_MATERIALS_METAL_HEADER
#define DA_CORE_MATERIALS_METAL_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/materials/material.h"
#include "core/textures/texture.h"

namespace Core
{

class Metal : public Material
{
public:
    Metal(const Utils::pointer<Texture> &texture, float fuzzy = 0) noexcept;
    Metal(const Metal &rhs) noexcept;

    virtual ~Metal() noexcept = default;

    Metal &operator=(const Metal &rhs) noexcept;

    Math::rgb emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;

    bool scatter(const Math::Ray &rIn, const HitRecord &record
                , Math::rgb &attenuation, Math::Ray &rOut) const noexcept override;
private:
    Utils::pointer<Texture> m_albedo;
    float m_fuzzy;
};

inline Metal::Metal(const Utils::pointer<Texture> &texture, float fuzzy) noexcept
    : m_albedo(texture), m_fuzzy(Math::clamp(fuzzy, 0, 1)) {}

inline Metal::Metal(const Metal &rhs) noexcept
    : m_albedo(rhs.m_albedo), m_fuzzy(rhs.m_fuzzy) {}

inline Metal &Metal::operator=(const Metal &rhs) noexcept
{
    m_albedo = rhs.m_albedo;
    m_fuzzy = rhs.m_fuzzy;
    return *this;
}

inline Math::rgb Metal::emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return {0, 0, 0};
}

inline bool Metal::scatter(const Math::Ray &rIn, const HitRecord &record
                        , Math::rgb &attenuation, Math::Ray &rOut) const noexcept
{
    Math::vec3 refl = Math::reflect(rIn.dir, record.normal);
    rOut.origin = record.point;
    rOut.dir = glm::normalize(refl + m_fuzzy * Math::Sampler::uniform_in_sphere());
    attenuation = m_albedo->evaluate(record.uv, record.point);
    return glm::dot(rOut.dir, record.normal) > 0;
}

}

#endif