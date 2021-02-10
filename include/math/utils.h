#ifndef DA_MATH_UTILS_HEADER
#define DA_MATH_UTILS_HEADER

#include <cmath>
#include <limits>

#include "utils/logger.h"
#include "math/vec.h"

namespace Math
{

constexpr float PI = 3.1415926535f;
constexpr float TWO_PI = PI * 2;
constexpr float HALF_PI = PI / 2.f;

constexpr float INF = std::numeric_limits<float>::infinity();
constexpr float EPS = 0.001f;

template <typename T, typename U>
inline const T clamp(T value, U min, U max) noexcept
{
    const auto tmin = static_cast<T>(min);
    const auto tmax = static_cast<T>(max);
    if (tmin > tmax)
        Utils::Logger::error("[ERROR] function `clamp(value, min, max)`:\n\tmin > max.\n");
    
    if (value < tmin)
        return tmin;
    else if (value > tmax)
        return tmax;
    else
        return value;
}

template <typename T, typename S>
inline const T lerp(T val1, T val2, S t) noexcept
{
    return (static_cast<S>(1) - t) * val1 + t * val2;
}

inline float degree(float radian) noexcept { return radian / PI * 180.; }

inline float radian(float degree) noexcept { return degree / 180. * PI; }

inline vec3 reflect(const vec3 &in, const vec3 &normal) noexcept
{
    return in - 2.f * glm::dot(in, normal) * normal;
}

inline bool refract(const vec3 &in, const vec3 &normal, float ni, float nt, vec3 &out)
{
    vec3 uin = glm::normalize(in);
    float dn = glm::dot(uin, normal);
    float n = ni / nt;
    float discr = 1.f - n * n * (1 - dn * dn);
    if (discr <= 0)
        return false;
    out = n * (uin - dn * normal) - normal * sqrt(discr);
    return true;
}

inline float schlick(float cosine, float ior) noexcept
{
    float R0 = (1 - ior) / (1 + ior);
    R0 *= R0;
    return R0 + (1 - R0) * pow(1 - cosine, 5);
}

}

#endif