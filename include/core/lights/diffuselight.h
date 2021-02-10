#ifndef CORE_LIGHTS_DIFFUSELIGHT_HEADER
#define CORE_LIGHTS_DIFFUSELIGHT_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/materials/material.h"
#include "core/textures/solidcolor.h"
#include "core/textures/texture.h"

namespace Core
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Math::rgb &color = {1, 1, 1}) noexcept;
    DiffuseLight(const Utils::pointer<Texture> &emit) noexcept;
    DiffuseLight(const DiffuseLight &rhs) noexcept;

    virtual ~DiffuseLight() noexcept = default;

    DiffuseLight &operator=(const DiffuseLight &rhs) noexcept;

    Math::rgb emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;

    bool scatter(const Math::Ray &rIn, const HitRecord &record
                        , Math::rgb &attenuation, Math::Ray &rOut) const noexcept override;
private:
    Utils::pointer<Texture> m_emit;
};

inline DiffuseLight::DiffuseLight(const Math::rgb &color) noexcept
    : m_emit(Utils::new_ptr<SolidColorTexture>(color))
{
}

inline DiffuseLight::DiffuseLight(const Utils::pointer<Texture> &emit) noexcept
    : m_emit(emit) {}

inline DiffuseLight::DiffuseLight(const DiffuseLight &rhs) noexcept
    : m_emit(rhs.m_emit) {}

inline DiffuseLight &DiffuseLight::operator=(const DiffuseLight &rhs) noexcept
{
    m_emit = rhs.m_emit;
    return *this;
}

inline Math::rgb DiffuseLight::emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return m_emit->evaluate(uv, point);
}

inline bool DiffuseLight::scatter(const Math::Ray &rIn, const HitRecord &record
                                    , Math::rgb &attenuation, Math::Ray &rOut) const noexcept
{
    return false;
}

}

#endif