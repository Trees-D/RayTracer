#ifndef DA_CORE_SHAPES_SHAPE_HEADER
#define DA_CORE_SHAPES_SHAPE_HEADER

#include "core/shapes/record.h"
#include "math/math.h"

namespace Core
{

class Shape
{
public:
    virtual ~Shape() noexcept = default;
    virtual bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept = 0;
    virtual bool bbox(Math::AABB &box) const noexcept = 0;
};

}

#endif