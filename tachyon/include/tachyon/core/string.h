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

#pragma once

#include <cstdarg>
#include <string>

namespace tachyon {

/**
 * Formats a string with printf style format.
 *
 * @param format The format string.
 * @param args The format arguments.
 * @return The result of formatting the string.
 */
std::string formatStringVarArgs(const char *format, va_list args);

/**
 * Formats a string with printf style format.
 *
 * @param format The format string.
 * @return The result of formatting the string.
 */
std::string formatString(const char *format, ...);

}

