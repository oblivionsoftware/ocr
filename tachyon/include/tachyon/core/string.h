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

