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
