#ifndef DA_MATH_AABB_HEADER
#define DA_MATH_AABB_HEADER

#include "math/vec.h"
#include "math/ray.h"

namespace Math
{

class AABB
{
public:
    AABB() noexcept;
    AABB(const vec3 &mn, const vec3 &mx) noexcept;
    AABB(const AABB &rhs) noexcept;

    virtual ~AABB() noexcept = default;

    AABB &operator=(const AABB &rhs) noexcept;

    bool intersect(const Ray &ray, float tMin, float tMax) const noexcept;
public:
    vec3 minimum;
    vec3 maximum;
};

inline AABB::AABB() noexcept
    : minimum(0, 0, 0), maximum(0, 0, 0) {}

inline AABB::AABB(const vec3 &mn, const vec3 &mx) noexcept
    : minimum(mn), maximum(mx) {}

inline AABB::AABB(const AABB &rhs) noexcept
    : minimum(rhs.minimum), maximum(rhs.maximum) {}

inline AABB &AABB::operator=(const AABB &rhs) noexcept
{
    this->minimum = rhs.minimum;
    this->maximum = rhs.maximum;
    return *this;
}

inline bool AABB::intersect(const Ray &ray, float tMin, float tMax) const noexcept
{
    for (int i = 0; i < 3; ++ i) {
        float inv = 1.f / ray.dir[i];
        float t0 = (minimum[i] - ray.origin[i]) * inv;
        float t1 = (maximum[i] - ray.origin[i]) * inv;
        if (inv < 0) {
            float t = t0;
            t0 = t1;
            t1 = t;
        }
        tMin < t0 ? tMin = t0 : 0;
        tMax > t1 ? tMax = t1 : 0;
        if (tMin >= tMax)
            return false;
    }
    return true;
}

inline const AABB merge_bbox(const AABB &box1, const AABB &box2) noexcept
{
    vec3 mn = {
        std::min(box1.minimum.x, box2.minimum.x),
        std::min(box1.minimum.y, box2.minimum.y),
        std::min(box1.minimum.z, box2.minimum.z)
    };

    vec3 mx = {
        std::max(box1.maximum.x, box2.maximum.x),
        std::max(box1.maximum.y, box2.maximum.y),
        std::max(box1.maximum.z, box2.maximum.z)
    };

    return AABB(mn, mx);
}

}

#endif