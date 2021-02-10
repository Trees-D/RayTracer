#ifndef DA_CORE_SHAPES_MESH_RECTANGLE_HEADER
#define DA_CORE_SHAPES_MESH_RECTANGLE_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/materials/material.h"
#include "core/shapes/shape.h"
#include "core/shapes/record.h"
#include "core/shapes/triangle.h"

namespace Core
{

class Rectangle : public Shape
{
public:
    Rectangle() noexcept;
    Rectangle(const Math::vec3 &top_left, const Math::vec3 &lower_left
            , const Math::vec3 &top_right, const Math::vec3 &lower_right
            , const Utils::pointer<Material> &material) noexcept;
    Rectangle(const Vertex &top_left, const Vertex &lower_left
            , const Vertex &top_right, const Vertex &lower_right
            , const Utils::pointer<Material> &material) noexcept;
    Rectangle(const Rectangle &rhs) noexcept;

    virtual ~Rectangle() noexcept = default;

    Rectangle &operator=(const Rectangle &rhs) noexcept;

    bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept override;

    bool bbox(Math::AABB &box) const noexcept override;
private:
    Utils::pointer<Triangle> m_tri0, m_tri1;
    Utils::pointer<Material> m_material;
    Math::AABB m_bounding_box;
};

inline Rectangle::Rectangle() noexcept
    : m_tri0(), m_tri1()
    , m_material(nullptr)
    , m_bounding_box()
{
}

inline Rectangle::Rectangle(const Math::vec3 &top_left, const Math::vec3 &lower_left
            , const Math::vec3 &top_right, const Math::vec3 &lower_right
            , const Utils::pointer<Material> &material) noexcept
{
    Math::vec3 edge1 = top_left - lower_left,
               edge2 = top_right - top_left,
               edge3 = lower_right - top_right,
               edge4 = lower_left - lower_right;
    if (!(
        glm::length(edge1) == glm::length(edge3) &&
        glm::length(edge2) == glm::length(edge4) &&
        glm::dot(edge1, edge2) == 0 && 
        glm::dot(edge2, edge3) == 0 &&
        glm::dot(edge3, edge4) == 0 &&
        glm::dot(edge4, edge1) == 0
    ))
        Utils::Logger::error("[ERROR] class `Rectagnle` constructor:\n\tnot a rectangle.\n");
    
    m_tri0 = Utils::new_ptr<Triangle>(lower_right, top_right, top_left, material);
    m_tri1 = Utils::new_ptr<Triangle>(lower_right, top_left, lower_left, material);
    m_material = material;
    Math::AABB box1, box2;
    if (!m_tri0->bbox(box1) || !m_tri1->bbox(box2))
        Utils::Logger::error("[ERROR] class `Rectangle` constructor:\n\tno bounding box.\n");
    m_bounding_box = Math::merge_bbox(box1, box2);
}

inline Rectangle::Rectangle(const Vertex &top_left, const Vertex &lower_left
            , const Vertex &top_right, const Vertex &lower_right
            , const Utils::pointer<Material> &material) noexcept
{
    Math::vec3 edge1 = top_left.point - lower_left.point,
               edge2 = top_right.point - top_left.point,
               edge3 = lower_right.point - top_right.point,
               edge4 = lower_left.point - lower_right.point;
    if (!(
        glm::length(edge1) == glm::length(edge3) &&
        glm::length(edge2) == glm::length(edge4) &&
        glm::dot(edge1, edge2) == 0 && 
        glm::dot(edge2, edge3) == 0 &&
        glm::dot(edge3, edge4) == 0 &&
        glm::dot(edge4, edge1) == 0
    ))
        Utils::Logger::error("[ERROR] class `Rectagnle` constructor:\n\tnot a rectangle.\n");
    
    m_tri0 = Utils::new_ptr<Triangle>(lower_right, top_right, top_left, material);
    m_tri1 = Utils::new_ptr<Triangle>(lower_right, top_left, lower_left, material);
    m_material = material;
    Math::AABB box1, box2;
    if (!m_tri0->bbox(box1) || !m_tri1->bbox(box2))
        Utils::Logger::error("[ERROR] class `Rectangle` constructor:\n\tno bounding box.\n");
    m_bounding_box = Math::merge_bbox(box1, box2);
}

inline Rectangle::Rectangle(const Rectangle &rhs) noexcept
    : m_tri0(rhs.m_tri0), m_tri1(rhs.m_tri1)
    , m_material(rhs.m_material)
    , m_bounding_box(rhs.m_bounding_box)
{
}

inline Rectangle &Rectangle::operator=(const Rectangle &rhs) noexcept
{
    m_tri0 = rhs.m_tri0;
    m_tri1 = rhs.m_tri1;
    m_material = rhs.m_material;
    m_bounding_box = rhs.m_bounding_box;
    return *this;
}

inline bool Rectangle::intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept
{
    if (!m_bounding_box.intersect(ray, tMin, tMax))
        return false;
    bool tri0 = m_tri0->intersect(ray, tMin, tMax, record);
    bool tri1 = m_tri1->intersect(ray, tMin, tri0 ? record.distance : tMax, record);
    return tri0 || tri1;
}

inline bool Rectangle::bbox(Math::AABB &box) const noexcept
{
    box = m_bounding_box;
    return true;
}

}

#endif