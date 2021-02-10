#ifndef CORE_SHAPES_TRIANGLE_HEADER
#define CORE_SHAPES_TRIANGLE_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "shape.h"

namespace Core
{

struct Vertex {
    Math::vec3 point;
    Math::vec2 uv;
    Math::vec3 normal;

    Vertex() noexcept
        : point(0, 0, 0)
        , uv(0, 0)
        , normal(0, 0, 0)
    {
    }

    Vertex(const Math::vec3 &point, const Math::vec2 &uv = {0, 0}, const Math::vec3 &normal = {0, 0, 0}) noexcept
        : point(point)
        , uv(uv)
        , normal(normal)
    {
    }
    
    virtual ~Vertex() noexcept = default;

    Vertex &operator=(const Vertex &rhs) noexcept
    {
        point = rhs.point;
        uv = rhs.uv;
        normal = rhs.normal;
        return *this;
    }
};

class Triangle : public Shape
{
public:
    Triangle() noexcept;
    Triangle(const Math::vec3 &v0, const Math::vec3 &v1, const Math::vec3 &v2, const Utils::pointer<Material> &material) noexcept;
    Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, const Utils::pointer<Material> &material) noexcept;
    Triangle(const Triangle &rhs) noexcept;

    virtual ~Triangle() noexcept = default;

    Triangle &operator=(const Triangle &rhs) noexcept;

    bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept override;

    bool bbox(Math::AABB &box) const noexcept override;

    const Math::vec2 get_uv(const Math::vec3 &point, Math::vec3 &normal) const noexcept;
private:
    Vertex m_v0, m_v1, m_v2;
    Math::vec3 m_normal;
    Utils::pointer<Material> m_material;
};

inline Triangle::Triangle() noexcept
    : m_v0(), m_v1(), m_v2()
    , m_normal()
    , m_material(nullptr)
{
}

inline Triangle::Triangle(const Math::vec3 &v0, const Math::vec3 &v1, const Math::vec3 &v2, const Utils::pointer<Material> &material) noexcept
    : m_v0(v0), m_v1(v1), m_v2(v2)
    , m_material(material)
{
    m_normal = glm::normalize(glm::cross(m_v1.point - m_v0.point, m_v2.point - m_v0.point));
    if (glm::length(m_v0.normal) == 0)
        m_v0.normal = m_normal;
    if (glm::length(m_v1.normal) == 0)
        m_v0.normal = m_normal;
    if (glm::length(m_v2.normal) == 0)
        m_v0.normal = m_normal;
}

inline Triangle::Triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, const Utils::pointer<Material> &material) noexcept
    : m_v0(v0), m_v1(v1), m_v2(v2)
    , m_material(material)
{
    m_normal = glm::normalize(glm::cross(m_v1.point - m_v0.point, m_v2.point - m_v0.point));
    if (glm::length(m_v0.normal) == 0)
        m_v0.normal = m_normal;
    if (glm::length(m_v1.normal) == 0)
        m_v0.normal = m_normal;
    if (glm::length(m_v2.normal) == 0)
        m_v0.normal = m_normal;
}

inline Triangle::Triangle(const Triangle &rhs) noexcept
    : m_v0(rhs.m_v0), m_v1(rhs.m_v1), m_v2(rhs.m_v2)
    , m_normal(rhs.m_normal)
    , m_material(rhs.m_material)
{
}

inline Triangle &Triangle::operator=(const Triangle &rhs) noexcept
{
    m_v0 = rhs.m_v0;
    m_v1 = rhs.m_v1;
    m_v2 = rhs.m_v2;
    m_normal = glm::normalize(rhs.m_normal);
    m_material = rhs.m_material;
    return *this;
}

inline bool Triangle::intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept
{
    if (glm::dot(ray.dir, m_normal) >= 0)
        return false;
    const Math::vec3 edge1 = m_v1.point - m_v0.point;
    const Math::vec3 edge2 = m_v2.point - m_v0.point;
    const Math::vec3 h = glm::cross(ray.dir, edge2);
    float a = glm::dot(edge1, h);
    if (a == 0)
        return false;
    float inv_a = 1.f / a;
    Math::vec3 s = ray.origin - m_v0.point;
    float u = inv_a * glm::dot(s, h);
    if (u < 0.f || u > 1.f)
        return false;
    Math::vec3 q = glm::cross(s, edge1);
    float v = inv_a * glm::dot(ray.dir, q);
    if (v < 0.f || u + v > 1.f)
        return false;
    float t = inv_a * glm::dot(edge2, q);
    if (t < tMin || t > tMax)
        return false;
    record.distance = t;
    record.point = ray(t);
    Math::vec3 n;
    record.uv = get_uv(record.point, n);
    record.normal = glm::normalize(glm::dot(n, ray.dir) >= 0 ? -n : n);
    record.material = m_material;
    return true;
}

inline bool Triangle::bbox(Math::AABB &box) const noexcept
{
    box.minimum.x = std::min(std::min(m_v0.point.x, m_v1.point.x), m_v2.point.x) - 0.1;
    box.minimum.y = std::min(std::min(m_v0.point.y, m_v1.point.y), m_v2.point.y) - 0.1;
    box.minimum.z = std::min(std::min(m_v0.point.z, m_v1.point.z), m_v2.point.z) - 0.1;

    box.maximum.x = std::max(std::max(m_v0.point.x, m_v1.point.x), m_v2.point.x) + 0.1;
    box.maximum.y = std::max(std::max(m_v0.point.y, m_v1.point.y), m_v2.point.y) + 0.1;
    box.maximum.z = std::max(std::max(m_v0.point.z, m_v1.point.z), m_v2.point.z) + 0.1;

    return true;
}

inline const Math::vec2 Triangle::get_uv(const Math::vec3 &point, Math::vec3 &normal) const noexcept
{
    auto f1 = m_v0.point - point;
    auto f2 = m_v1.point - point;
    auto f3 = m_v2.point - point;
    float a = glm::length(glm::cross(m_v1.point - m_v0.point, m_v2.point - m_v0.point));
    float a1 = glm::length(glm::cross(f2, f3)) / a;
    float a2 = glm::length(glm::cross(f3, f1)) / a;
    float a3 = glm::length(glm::cross(f1, f2)) / a;

    auto uv = m_v0.uv * a1 + m_v1.uv * a2 + m_v2.uv * a3;
    normal = m_v0.normal * a1 + m_v1.normal * a2 + m_v2.normal * a3;
    return uv;
}

}

#endif