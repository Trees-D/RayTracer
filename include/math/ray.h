#ifndef DA_MATH_RAY_HEADER
#define DA_MATH_RAY_HEADER

#include "math/vec.h"

namespace Math
{

class Ray
{
public:
    Ray() noexcept;
    Ray(const vec3 &ori, const vec3 &dir) noexcept;
    
    virtual ~Ray() noexcept = default;

    const vec3 operator()(float t) const noexcept;

public:
    vec3 origin;
    vec3 dir;
};

inline Ray::Ray() noexcept
    : origin({0, 0, 0})
    , dir({0, 0, 0})
{
}

inline Ray::Ray(const vec3 &ori, const vec3 &dir) noexcept
    : origin(ori)
    , dir(dir)
{
}

inline const vec3 Ray::operator()(float t) const noexcept
{
    return origin + t * dir;
}

}

#endif