#ifndef DA_CORE_SHAPES_SPHERE_HEADER
#define DA_CORE_SHAPES_SPHERE_HEADER

#include "core/shapes/shape.h"
#include "math/math.h"

namespace Core
{

class Sphere : public Shape
{
public:
    Sphere() noexcept;
    Sphere(const Math::vec3 &center, float radius, const Utils::pointer<Material> &material) noexcept;
    Sphere(const Sphere &rhs) noexcept;

    virtual ~Sphere() noexcept = default;

    Sphere &operator=(const Sphere &rhs) noexcept;

    bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept override;
    
    bool bbox(Math::AABB &box) const noexcept override;

    const Math::vec2 get_uv(const Math::vec3 &point) const noexcept;
private:
    Math::vec3 m_center;
    float m_radius;
    Utils::pointer<Material> m_material;
};

inline Sphere::Sphere() noexcept
    : m_center(0, 0, 0)
    , m_radius(0)
    , m_material(nullptr)
{
}

inline Sphere::Sphere(const Math::vec3 &center, float radius, const Utils::pointer<Material> &material) noexcept
    : m_center(center)
    , m_radius(radius)
    , m_material(material)
{
}

inline Sphere::Sphere(const Sphere &rhs) noexcept
    : m_center(rhs.m_center)
    , m_radius(rhs.m_radius)
    , m_material(rhs.m_material)
{
}

inline Sphere &Sphere::operator=(const Sphere &rhs) noexcept
{
    m_center = rhs.m_center;
    m_radius = rhs.m_radius;
    m_material = rhs.m_material;
    return *this;
}

inline bool Sphere::intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept
{
    Math::vec3 co = ray.origin - m_center;
    Math::vec3 d = ray.dir;
    float a = glm::dot(d, d);
    float b = glm::dot(co, d);
    float c = glm::dot(co, co) - m_radius * m_radius;
    float discr = b * b - a * c;
    if (discr < 0)
        return false;
    discr = sqrt(discr);
    float t;
    float inv_a = 1.f / a;
    t = (-b - discr) * inv_a;
    if (t < tMin || t > tMax)
    {
        t = (-b + discr) * inv_a;
        if (t < tMin || t > tMax)
            return false;
    }
    record.distance = t;
    record.point = ray(t);
    record.normal = glm::normalize(record.point - m_center);
    record.material = m_material;
    record.uv = get_uv(record.normal);
    return true;
}

inline bool Sphere::bbox(Math::AABB &box) const noexcept
{
    box.minimum = m_center - Math::vec3(m_radius, m_radius, m_radius);
    box.maximum = m_center + Math::vec3(m_radius, m_radius, m_radius);
    return true;
}

inline const Math::vec2 Sphere::get_uv(const Math::vec3 &v) const noexcept
{
    // float phi = std::atan2(v.z, v.x);
    // float theta = std::asin(v.y);
    // return {
    //     1 - (phi + Math::PI) / Math::TWO_PI,
    //     (theta + Math::HALF_PI) / Math::PI
    // };
    float theta = std::acos(-v.y);
    float phi = std::atan2(-v.z, v.x) + Math::PI;

    return {
        phi / Math::TWO_PI,
        theta / Math::PI
    };
}

}

#endif