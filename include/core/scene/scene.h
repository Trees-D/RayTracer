#ifndef DA_CORE_SCENE_SCENE_HEADER
#define DA_CORE_SCENE_SCENE_HEADER

#include "math/math.h"
#include "utils/utils.h"
#include "core/camera/camera.h"
#include "core/shapes/shape.h"
#include "core/accelerate/bvhtree.h"

#include <vector>

namespace Core
{

class Scene
{
public:
    Scene() noexcept;

    virtual ~Scene() noexcept = default;

    void add(const Utils::pointer<Shape> &shape) noexcept;

    void set_camera(const Camera &camera) noexcept;

    bool intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept;

    bool build_tree() noexcept;

public:
    Camera camera;
    std::vector<Utils::pointer<Shape>> shapes;
    Utils::pointer<BVHTree> bvh_tree;
    
private:
    Scene(const Scene &rhs) noexcept = delete;
    Scene &operator=(const Scene &rhs) noexcept = delete;
};

inline Scene::Scene() noexcept
{
    camera = Camera();
    std::vector<Utils::pointer<Shape>>().swap(shapes);
    bvh_tree = nullptr;
}

inline void Scene::add(const Utils::pointer<Shape> &shape) noexcept
{
    shapes.push_back(shape);
}

inline void Scene::set_camera(const Camera &camera) noexcept
{
    this->camera = camera;
}

inline bool Scene::intersect(const Math::Ray &ray, float tMin, float tMax, HitRecord &record) const noexcept
{
    if (bvh_tree)
        return bvh_tree->intersect(ray, tMin, tMax, record);

    bool hit_anything = false;
    HitRecord tmp_record;
    float distance = tMax;
    for (const auto iter : shapes) {
        if (iter->intersect(ray, tMin, distance, tmp_record)) {
            hit_anything = true;
            distance = tmp_record.distance;
            record = tmp_record;
        }
    }
    return hit_anything;
}

inline bool Scene::build_tree() noexcept
{
    this->bvh_tree = Utils::new_ptr<BVHTree>(shapes, 0, shapes.size());
    return true;
}

}

#endif