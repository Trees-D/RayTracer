#ifndef DA_UTILS_LOGGER_HEADER
#define DA_UTILS_LOGGER_HEADER

#include <cstdarg>
#include <iostream>

namespace Utils
{

class Logger
{
public:
    static void error(const char *format, ...) noexcept;
    static void debug(const char *format, ...) noexcept;
private:
    ~Logger() noexcept = delete;
};

inline void Logger::debug(const char *format, ...) noexcept
{
    va_list args;
    va_start (args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    return ;
}

inline void Logger::error(const char *format, ...) noexcept
{
    va_list args;
    va_start (args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

}

#endif