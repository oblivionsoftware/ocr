#include "tachyon/core/string.h"

#include <vector>

namespace tachyon {

std::string formatStringVarArgs(const char *format, va_list args)
{
    va_list argsCopy;
    va_copy(argsCopy, args);
    int formattedSize = vsnprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);

    std::vector<char> buffer(formattedSize + 1);
    vsnprintf(buffer.data(), buffer.size(), format, args);

    return {buffer.data()};
}

std::string formatString(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    auto result = formatStringVarArgs(format, args);
    va_end(args);

    return result;
}

}
