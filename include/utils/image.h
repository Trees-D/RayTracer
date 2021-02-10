#ifndef DA_UTILS_IMAGE_HEADER
#define DA_UTILS_IMAGE_HEADER

#include <cstdint>
#include <string>

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "math/utils.h"
#include "math/color.h"
#include "utils/logger.h"

namespace Utils
{

using Math::rgb;
using Math::rgba;

class Image
{
public:
    Image() noexcept;
    Image(const std::string &filepath, bool flip = true) noexcept;
    Image(int width, int height, int channel = 3) noexcept;
    Image(const Image &rhs) noexcept;

    virtual ~Image() noexcept;

    Image &operator=(const Image &rhs) noexcept;

    float &operator()(int x, int y, int channel = 3) noexcept;
    const float &operator()(int x, int y, int channel = 3) const noexcept;

    const rgba pixel(int x, int y) const noexcept;

    void set(int x, int y, const rgb &color) noexcept;
    void set(int x, int y, const rgba &color) noexcept;
    
    void add(int x, int y, const rgb &color) noexcept;
    void add(int x, int y, const rgba &color) noexcept;

public:
    bool load(const std::string &filepath, bool flip = true) noexcept;
    bool save(const std::string &filename, bool flip = true, bool gamma = true) const noexcept;
    void init(int width, int height, int channel) noexcept;
    void free() noexcept;

public:
    const rgba sample(const Math::vec2 &uv) const noexcept;

public:
    const int width() const noexcept;
    const int height() const noexcept;
    const int channel() const noexcept;

private:
    float *m_data;
    int m_width;
    int m_height;
    int m_channel;
};

inline Image::Image() noexcept
    : m_data(nullptr), m_width(0), m_height(0), m_channel(0)
{
}

inline Image::Image(const std::string &filepath, bool flip) noexcept
    : m_data(nullptr)
{
    load(filepath, flip);
}

inline Image::Image(int width, int height, int channel) noexcept
    : m_data(nullptr)
{
    init(width, height, channel);
}

inline Image::Image(const Image &rhs) noexcept
    : m_data(nullptr)
{
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_channel = rhs.m_channel;

    const int size = m_width * m_height * m_channel;
    m_data = new float[size];
    memcpy(m_data, rhs.m_data, size * sizeof(float));
}

inline Image::~Image() noexcept
{
    free();
}

inline Image &Image::operator=(const Image &rhs) noexcept
{
    free();
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_channel = rhs.m_channel;
    
    const int size = m_width * m_height * m_channel;
    m_data = new float[size];
    memcpy(m_data, rhs.m_data, size * sizeof(float));
    return *this;
}

inline float &Image::operator()(int x, int y, int channel) noexcept
{
    if (channel >= m_channel)
        Logger::error("[ERROR] class `Image` operator(x=%d, y=%d, channel=%d):\n\tchannel should be less than %d.\n"
                    , x, y, channel, m_channel);
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        Logger::error("[ERROR] class `Image` operator(x=%d, y=%d, channel=%d):\n\tImage size is [[0, %d), [0, %d)].\n"
                    , x, y, channel, m_width, m_height);
    return m_data[(y * m_width + x) * m_channel + channel];
}

inline const float &Image::operator()(int x, int y, int channel) const noexcept
{
    return const_cast<Image*>(this)->operator()(x, y, channel);
}

inline const rgba Image::pixel(int x, int y) const noexcept
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        Logger::error("[ERROR] class `Image` function `pixel(x=%d, y=%d)`:\n\tImage size is [[0, %d), [0, %d)].\n"
                    , x, y, m_width, m_height);
    rgba rst = {0, 0, 0, 0};
    for (int i = 0; i < m_channel; ++ i)
        rst[i] = (*this)(x, y, i);
    return rst;
}

inline void Image::set(int x, int y, const rgb &color) noexcept
{
    if (m_channel < 3)
        Logger::error("[ERROR] class `Image` function `set(x=%d, y=%d, rgb)`:\n\tImage channel is %d.\n"
                    , x, y, m_channel);
    for (int i = 0; i < 3; ++ i)
        (*this)(x, y, i) = color[i];
}

inline void Image::set(int x, int y, const rgba &color) noexcept
{
    for (int i = 0; i < m_channel; ++ i)
        (*this)(x, y, i) = color[i];
}

inline void Image::add(int x, int y, const rgb &color) noexcept
{
    if (m_channel < 3)
        Logger::error("[ERROR] class `Image` function `add(x=%d, y=%d, rgb)`:\n\tImage channel is %d.\n"
                    , x, y, m_channel);
    for (int i = 0; i < 3; ++ i)
        (*this)(x, y, i) += color[i];
}

inline void Image::add(int x, int y, const rgba &color) noexcept
{
    for (int i = 0; i < m_channel; ++ i)
        (*this)(x, y, i) += color[i];
}

inline bool Image::load(const std::string &filepath, bool flip) noexcept
{
    free();
    stbi_set_flip_vertically_on_load(flip);

    auto data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channel, 0);
    if (!data)
        return false;
    const int size = m_width * m_height * m_channel;
    m_data = new float[size];
    for (int i = 0; i < size; ++ i)
        m_data[i] = static_cast<float>(data[i]) / 255.f;
    stbi_image_free(data);

    return true;
}

inline bool Image::save(const std::string &filename, bool flip, bool gamma) const noexcept
{
    if (!(m_data != nullptr && m_channel > 0 && m_channel <= 4))
        Logger::error("[ERROR] class `Image` function `save(filename=%s, flip=%s):\n\tInvalid image.\n"
                    , filename.c_str(), (flip ? "True" : "False"));
    
    stbi_flip_vertically_on_write(flip);
    const int size = m_width * m_height * m_channel;
    auto data = new stbi_uc[size];
    for (int i = 0; i < size; ++ i)
    {
        float value;
        if (!gamma)
            value = m_data[i] * 255.f;
        else
            value = pow(m_data[i], 1.f / 2.2f) * 255.f;
        value = Math::clamp(value, 0, 255);
        data[i] = static_cast<stbi_uc>(value + 0.5);
    }
    auto rst = stbi_write_png(filename.c_str(), m_width, m_height, m_channel, data, m_width * m_channel);
    delete [] data;
    return rst;
}

inline void Image::init(int width, int height, int channel) noexcept
{
    if (channel <= 0 || channel > 4)
        Logger::error("[ERROR] class `Image` function `init(width=%d, height=%d, channel=%d)`:\n\tchannel should be [1, 2, 3, 4] not %d.\n"
                    , width, height, channel, channel);
    free();
    m_width = width;
    m_height = height;
    m_channel = channel;
    const int size = m_width * m_height * m_channel;
    m_data = new float[size];
}

inline void Image::free() noexcept
{
    m_width = m_height = m_channel = 0;
    if (m_data != nullptr)
        delete [] m_data;
    m_data = nullptr;
}

inline const int Image::width() const noexcept
{
    return m_width;
}

inline const int Image::height() const noexcept
{
    return m_height;
}

inline const int Image::channel() const noexcept
{
    return m_channel;
}

inline const rgba Image::sample(const Math::vec2 &uv) const noexcept
{
    float x = Math::clamp(uv.x, 0.f, 0.999999f) * m_width;
    float y = Math::clamp(uv.y, 0.f, 0.999999f) * m_height;

    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = Math::clamp(x0 + 1, 0, m_width - 1);
    int y1 = Math::clamp(y + 1, 0, m_height - 1);

    const rgba color[] = {
        pixel(x0, y0),
        pixel(x1, y0),
        pixel(x0, y1),
        pixel(x1, y1)
    };

    float tx = std::abs(x - x0);
    float ty = std::abs(y - y0);

    const rgba rst = Math::lerp(
        Math::lerp(color[0], color[1], tx),
        Math::lerp(color[2], color[3], tx),
        ty
    );

    return rst;
}

}

#endif