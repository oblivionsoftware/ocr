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

#include <exception>
#include <string>

#include "tachyon/core/common.h"

/**
 * Macro for throwing an exception that injects the correct file, function, and line number.
 *
 * @param message The message to throw.
 */
#define TACHYON_THROW(...) throw tachyon::Exception(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

namespace tachyon {

/**
 * Base class for all exceptions thrown by tachyon.
 */
class Exception : public std::exception {

public:

    /**
     * Constructs an exception with the specified error message.
     *
     * @param file The name of the file.
     * @param function The name of the function throwing the error.
     * @param line The line number.
     * @param message The error message.
     */
    Exception(const char *file, const char *function, i32 line, const std::string &message);

    /**
     * Constructs an exception with the specified error message.
     *
     * @param file The name of the file.
     * @param function The name of the function throwing the error.
     * @param line The line number.
     * @param format The printf style format string.
     */
    Exception(const char *file, const char *function, i32 line, const char *format, ...);

    /**
     * Gets the line number where the exception was thrown.
     *
     * @return The line number.
     */
    i32 line() const {
        return _line;
    }

    /**
     * Gets the exception message.
     *
     * @return The exception message.
     */
    const std::string &message() const {
        return _message;
    }

    /**
     * Gets the name of the file from which the exception was thrown.
     *
     * @return The name of the file.
     */
    const char *file() const {
        return _file;
    }

    /**
     * Gets the name of the function from which the exception was thrown.
     *
     * @return The name of the function.
     */
    const char *function() const {
        return _function;
    }

    /**
     * Gets the full exception message.
     * @return The full message.
     */
    virtual const char *what() const throw() {
        return _fullMessage.c_str();
    }

private:

    std::string _message;

    std::string _fullMessage;

    const char *_file;

    const char *_function;

    i32 _line;

};

}
