#ifndef DA_CORE_CAMERA_CAMERA_HEADER
#define DA_CORE_CAMERA_CAMERA_HEADER

#include "utils/utils.h"
#include "math/math.h"

namespace Core
{

class Camera
{
public:
    Camera(const Math::vec3 &look_from = {0, 2, 3}
         , const Math::vec3 &look_at = {0, 0, 0}
         , const Math::vec3 &vup = {0, 1, 0}
         , float vfov = 60
         , float aspect = 2
         , float aperture = 0.1
         , float focus_dist = 3
    ) noexcept;
    
    virtual ~Camera() noexcept = default;

    Camera &operator=(const Camera &rhs) noexcept;
    
    Math::Ray operator()(float s, float t) const noexcept;
    
public:
    Math::vec3 origin;
    Math::vec3 lower_left_corner;
    Math::vec3 horizontal;
    Math::vec3 vertical;
    Math::vec3 u, v, w;
    float width;
    float height;
    float lens_radius;
};

inline Camera::Camera(const Math::vec3 &look_from
         , const Math::vec3 &look_at
         , const Math::vec3 &vup
         , float vfov
         , float aspect
         , float aperture
         , float focus_dist
    ) noexcept
{
    lens_radius = aperture / 2.f;
    float theta = Math::radian(vfov);
    height = 2 * tan(theta / 2) * focus_dist;
    width = aspect * height;
    origin = look_from;
    w = glm::normalize(look_from - look_at);
    u = glm::normalize(glm::cross(vup, w));
    v = glm::cross(w, u);
    lower_left_corner = origin - width / 2.f * u - height / 2.f * v - focus_dist * w;
    horizontal = width * u;
    vertical = height * v;
}

inline Camera &Camera::operator=(const Camera &rhs) noexcept
{
    origin = rhs.origin;
    lower_left_corner = rhs.lower_left_corner;
    horizontal = rhs.horizontal;
    vertical = rhs.vertical;
    u = rhs.u;
    v = rhs.v;
    w = rhs.w;
    width = rhs.width;
    height = rhs.height;
    lens_radius = rhs.lens_radius;
    return *this;
}

inline Math::Ray Camera::operator()(float s, float t) const noexcept
{
    Math::vec2 rd = lens_radius * Math::Sampler::uniform_in_disk();
    auto ray_origin = origin + rd.x * u + rd.y * v;
    auto ray_dest = lower_left_corner + s * horizontal + t * vertical;
    return Math::Ray(ray_origin, glm::normalize(ray_dest - ray_origin));
}

}

#endif