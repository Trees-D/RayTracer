#ifndef DA_CORE_SHAPES_RECORD_HEADER
#define DA_CORE_SHAPES_RECORD_HEADER

#include "math/math.h"
#include "utils/utils.h"

namespace Core
{

class Material;

struct HitRecord
{
    Math::vec3 point;
    Math::vec3 normal;
    float distance;

    Utils::pointer<Material> material;
    Math::vec2 uv;
};

}

#endif