#ifndef DA_CORE_ACCELERATE_BVHTREE_HEADER
#define DA_CORE_ACCELERATE_BVHTREE_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/shapes/shape.h"
#include "core/shapes/record.h"

#include <vector>
#include <algorithm>

namespace Core
{

class BVHTree : public Shape
{
public:
    BVHTree() noexcept;
    BVHTree(const std::vector<Utils::pointer<Shape>> &objects, int l, int r) noexcept;

    virtual ~BVHTree() noexcept = default;

    bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept override;
    bool bbox(Math::AABB &box) const noexcept override;
public:
    static bool comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2, int axis) noexcept;
    static bool x_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept;
    static bool y_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept;
    static bool z_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept;
public:
    Utils::pointer<Shape> left_child;
    Utils::pointer<Shape> right_child;
    Math::AABB bounding_box;
};

inline BVHTree::BVHTree() noexcept
    : left_child(nullptr)
    , right_child(nullptr)
    , bounding_box()
{
}

inline BVHTree::BVHTree(const std::vector<Utils::pointer<Shape>> &objects, int l, int r) noexcept
{
    auto objs = objects;
    int axis = static_cast<int>(2.999f * Math::Sampler::uniform01());
    auto cmp = (axis == 0
                    ? x_comparator
                    : (axis == 1 ? y_comparator : z_comparator));
    if (r - l == 1)
    {
        left_child = right_child = objs[l];
    }
    else if (r - l == 2)
    {
        if (cmp(objs[l], objs[l + 1]))
        {
            left_child = objs[l];
            right_child = objs[l + 1];
        }
        else
        {
            left_child = objs[l + 1];
            right_child = objs[l];
        }
    }
    else
    {
        std::sort(objs.begin() + l, objs.begin() + r, cmp);
        int mid = l + (r - l) / 2;
        left_child = Utils::new_ptr<BVHTree>(objs, l, mid);
        right_child = Utils::new_ptr<BVHTree>(objs, mid, r);
    }

    Math::AABB boxl, boxr;

    if (!left_child->bbox(boxl) || !right_child->bbox(boxr))
        Utils::Logger::error("[ERROR] class `BVHTree` constructor `BVHTree(objs, l, r)`:\n\tno bounding box.\n");

    bounding_box = Math::merge_bbox(boxl, boxr);
}

inline bool BVHTree::intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept
{
    if (!bounding_box.intersect(ray, tMin, tMax))
        return false;
    
    bool left = left_child->intersect(ray, tMin, tMax, record);
    bool right = right_child->intersect(ray, tMin, left ? record.distance : tMax, record);
    return left || right;
}

inline bool BVHTree::bbox(Math::AABB &box) const noexcept
{
    box = bounding_box;
    return true;
}

inline bool BVHTree::comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2, int axis) noexcept
{
    Math::AABB box1;
    Math::AABB box2;
    if (!obj1->bbox(box1) || !obj2->bbox(box2))
        return false;
    
    return box1.minimum[axis] < box2.minimum[axis];
}

inline bool BVHTree::x_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept
{
    return comparator(obj1, obj2, 0);
}

inline bool BVHTree::y_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept
{
    return comparator(obj1, obj2, 1);
}

inline bool BVHTree::z_comparator(const Utils::pointer<Shape> &obj1, const Utils::pointer<Shape> &obj2) noexcept
{
    return comparator(obj1, obj2, 2);
}

}

#endif