#ifndef DA_CORE_MATERIALS_DIELECTRIC_HEADER
#define DA_CORE_MATERIALS_DIELECTRIC_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/materials/material.h"

namespace Core
{

class Dielectric : public Material
{
public:
    Dielectric(float ior = 1.f) noexcept;
    Dielectric(const Dielectric &rhs) noexcept;

    virtual ~Dielectric() noexcept = default;

    Dielectric &operator=(const Dielectric &rhs) noexcept;

    Math::rgb emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;

    bool scatter(const Math::Ray &rIn, const HitRecord &record
                , Math::rgb &attenuation, Math::Ray &rOut) const noexcept override;
private:
    float m_ior;
};

inline Dielectric::Dielectric(float ior) noexcept
    : m_ior(ior)
{
}

inline Dielectric::Dielectric(const Dielectric &rhs) noexcept
    : m_ior(rhs.m_ior)
{
}

inline Dielectric &Dielectric::operator=(const Dielectric &rhs) noexcept
{
    m_ior = rhs.m_ior;
    return *this;
}

inline Math::rgb Dielectric::emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return {0, 0, 0};
}

inline bool Dielectric::scatter(const Math::Ray &rIn, const HitRecord &record
                , Math::rgb &attenuation, Math::Ray &rOut) const noexcept
{
    attenuation = {1, 1, 1};
    float ni, nt;
    Math::vec3 normal;
    float cosine;
    if (glm::dot(rIn.dir, record.normal) > 0)
    {
        normal = -record.normal;
        ni = m_ior;
        nt = 1.f;
        cosine = m_ior * glm::dot(rIn.dir, record.normal);
    }
    else
    {
        normal = record.normal;
        ni = 1.f;
        nt = m_ior;
        cosine = -glm::dot(rIn.dir, record.normal);
    }
    Math::vec3 refl = Math::reflect(rIn.dir, record.normal);
    Math::vec3 refr;
    float refr_p = (Math::refract(rIn.dir, normal, ni, nt, refr) ? Math::schlick(cosine, m_ior) : 1.f);
    rOut.origin = record.point;
    rOut.dir = glm::normalize((Math::Sampler::uniform01() < refr_p ? refl : refr));
    return true;
}

}

#endif