#ifndef DA_UTILS_POINTER_HEADER
#define DA_UTILS_POINTER_HEADER

#include <memory>

namespace Utils
{

template <typename T>
using pointer = std::shared_ptr<T>;

template <typename T, typename ...Args>
const pointer<T> new_ptr(Args &&...args)
{
    return pointer<T>(new T(std::forward<Args>(args)...));
}

}

#endif