#ifndef DA_CORE_TEXTURES_TEXTURE_HEADER
#define DA_CORE_TEXTURES_TEXTURE_HEADER

#include "math/math.h"
#include "utils/utils.h"

namespace Core
{

class Texture
{
public:
    virtual ~Texture() noexcept = default;
    virtual const Math::rgb evaluate(const Math::vec2 &uv, const Math::vec3 &point) const noexcept = 0;
};

}

#endif