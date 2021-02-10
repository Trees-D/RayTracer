#ifndef DA_CORE_MATERIALS_MATERIAL_HEADER
#define DA_CORE_MATERIALS_MATERIAL_HEADER

#include "core/shapes/record.h"
#include "math/math.h"
#include "utils/utils.h"

namespace Core
{

class Material
{
public:
    virtual ~Material() noexcept = default;
    virtual Math::rgb emitted(const Math::vec2 &uv, const Math::vec3 &point) const noexcept = 0;
    virtual bool scatter(const Math::Ray &rIn, const HitRecord &record
                        , Math::rgb &attenuation, Math::Ray &rOut) const noexcept = 0;
};

}

#endif