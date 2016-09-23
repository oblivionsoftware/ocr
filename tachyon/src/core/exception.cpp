/*
 * Copyright 2016 Jeff Upton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    _message = formatStringVarArgs(format, args);
    va_end(args);

    _fullMessage = formatString("%s - %s - %s(%d)", _message.c_str(), function, file, line);
}

}
