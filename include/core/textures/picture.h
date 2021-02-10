#ifndef DA_CORE_TEXTURES_PICTURE_HEADER
#define DA_CORE_TEXTURES_PICTURE_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/textures/texture.h"

namespace Core
{

class PictureTexture : public Texture
{
public:
    PictureTexture(const Utils::Image &img) noexcept;
    PictureTexture(const std::string &filepath) noexcept;
    PictureTexture(const PictureTexture &rhs) noexcept;

    virtual ~PictureTexture() noexcept = default;

    PictureTexture &operator=(const PictureTexture &rhs) noexcept;

    const Math::rgb evaluate(const Math::vec2 &uv, const Math::vec3 &point) const noexcept override;
private:
    Utils::Image m_pic;
};

inline PictureTexture::PictureTexture(const Utils::Image &img) noexcept
    : m_pic(img) {}

inline PictureTexture::PictureTexture(const std::string &filepath) noexcept
    : m_pic(filepath) {}

inline PictureTexture::PictureTexture(const PictureTexture &rhs) noexcept
    : m_pic(rhs.m_pic) {}

inline PictureTexture &PictureTexture::operator=(const PictureTexture &rhs) noexcept
{
    m_pic = rhs.m_pic;
    return *this;
}

inline const Math::rgb PictureTexture::evaluate(const Math::vec2 &uv, const Math::vec3 &point) const noexcept
{
    return m_pic.sample(uv);
}

}

#endif