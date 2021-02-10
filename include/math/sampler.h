#ifndef DA_MATH_SAMPLER_HEADER
#define DA_MATH_SAMPLER_HEADER

#include "math/vec.h"
#include "math/utils.h"

#include <random>

namespace Math
{

static std::uniform_real_distribution<float> map(0.0f, 1.0f);
static std::default_random_engine engine;

class Sampler
{
public:
    inline static const float uniform01() noexcept
    {
        return map(engine);
    }

    inline static const vec2 uniform_in_square() noexcept
    {
        return {
            uniform01(), uniform01()
        };
    }

    inline static const vec3 uniform_in_cube() noexcept
    {
        return {
            uniform01(), uniform01(), uniform01()
        };
    }

    inline static const vec2 uniform_in_disk() noexcept
    {
        auto u = uniform_in_square();
        if (u[0] == 0 && u[1] == 0)
            return {0, 0};
        float theta, r;
        if (std::abs(u[0]) > std::abs(u[1])) {
            r = u[0];
            theta = PI / 4.f * (u[1] / u[0]);
        }
        else {
            r = u[1];
            theta = HALF_PI - PI / 4.f * (u[0] / u[1]);
        }
        return {
            r * std::cos(theta),
            r * std::sin(theta)
        };
    }

    inline static const vec2 uniform_on_disk() noexcept
    {
        float theta = uniform01() * TWO_PI;
        return {
            cos(theta), sin(theta)
        };
    }

    inline static const vec3 uniform_on_sphere() noexcept
    {
        float x1 = uniform01();
        float x2 = uniform01();

        float phi = TWO_PI * x2;
        float t = 2 * std::sqrt(x1 * (1 - x1));
        return {
            std::cos(phi) * t,
            std::sin(phi) * t,
            1 - 2 * x1
        };
    }

    inline static const vec3 uniform_in_sphere() noexcept
    {
        vec3 rst = uniform_on_sphere();
        return uniform01() * rst;
    }

    inline static const vec3 uniform_on_hemishpere(const vec3 &normal) noexcept
    {
        const vec3 rst = uniform_on_sphere();
        if (glm::dot(rst, normal) < 0)
            return -rst;
        return rst;
    }

private:
    virtual ~Sampler() noexcept = delete;
};

}

#endif