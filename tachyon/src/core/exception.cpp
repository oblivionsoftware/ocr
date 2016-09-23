#include "tachyon/core/exception.h"

#include "tachyon/core/string.h"

namespace tachyon {

Exception::Exception(const char *file, const char *function, i32 line, const std::string &message)
        : _message {message},
          _file {file},
          _function {function},
          _line {line}
{
    _fullMessage = formatString("%s - %s - %s(%d)", message.c_str(), function, file, line);
}

Exception::Exception(const char* file, const char* function, i32 line, const char* format, ...)
    : _file {file},
      _function {function},
      _line {line}
{
    va_list args;
    va_start(args, format);
    _message = formatString(format, args);
    va_end(args);

    _fullMessage = formatString("%s - %s - %s(%d)", _message.c_str(), function, file, line);
}

}
