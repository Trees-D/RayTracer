#ifndef DA_CORE_TEXTURES_SOLIDCOLOR_HEADER
#define DA_CORE_TEXTURES_SOLIDCOLOR_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/textures/texture.h"

namespace Core
{

class SolidColorTexture : public Texture
{
public:
    SolidColorTexture(const Math::rgb &color = {0, 0, 0}) noexcept;
    SolidColorTexture(const SolidColorTexture &rhs) noexcept;

    virtual ~SolidColorTexture() noexcept = default;

    SolidColorTexture &operator=(const SolidColorTexture &rhs) noexcept;

    const Math::rgb evaluate(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;
private:
    Math::rgb m_color;
};

inline SolidColorTexture::SolidColorTexture(const Math::rgb &color) noexcept
    : m_color(color)
{
}

inline SolidColorTexture::SolidColorTexture(const SolidColorTexture &rhs) noexcept
    : m_color(rhs.m_color)
{
}

inline SolidColorTexture &SolidColorTexture::operator=(const SolidColorTexture &rhs) noexcept
{
    m_color = rhs.m_color;
    return *this;
}

inline const Math::rgb SolidColorTexture::evaluate(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return m_color;
}

}

#endif