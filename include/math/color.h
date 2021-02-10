#ifndef DA_MATH_COLOR_HEADER
#define DA_MATH_COLOR_HEADER

#ifndef GLM_FORCE_SWIZZLE
    #define GLM_FORCE_SWIZZLE
#endif
#include <glm/glm.hpp>

#include <string>

namespace Math
{

using rgb = glm::vec3;
using rgba = glm::vec4;

constexpr rgb BLACK = {0, 0, 0};
constexpr rgb WHITE = {1, 1, 1};

inline rgb cast_to_rgb(int r, int g, int b) noexcept
{
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        Utils::Logger::error("[ERROR] function `cast_to_rgb(r, g, b)`:\n\trgb values (%d, %d, %d) out of [0, 256).\n"
                            , r, g, b);
    return rgb(r / 255.f, g / 255.f, b / 255.f);
}

inline rgb cast_to_rgb(const std::string &hex) noexcept
{
    // this function is UNCHECKED.
    int value;
    sscanf(hex.c_str(), "#%x", &value);
    return cast_to_rgb((value & 0xFF0000) >> 16, (value & 0xFF00) >> 8, (value & 0xFF));
}

}

#endif