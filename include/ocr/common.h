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

#include <stdint.h>
#include <stdlib.h>

#define OCR_INLINE static __inline 

#define OCR_PLATFORM_WINDOWS 1
#define OCR_PLATFORM_LINUX 2
#define OCR_PLATFORM_MAC 3

#ifdef _WIN32
    #define OCR_PLATFORM OCR_PLATFORM_WINDOWS
    #define OCR_PLATFORM_NAME "Windows"
#elif __APPLE__
    #define OCR_PLATFORM OCR_PLATFORM_MAC
    #define OCR_PLATFORM_NAME "Mac OSX"
#elif __linux__
    #define OCR_PLATFORM OCR_PLATFORM_LINUX
    #define OCR_PLATFORM_NAME "Linux"
#else
    #error "Unsupported Platform"
#endif

namespace ocr {

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

typedef float r32;
typedef double r64;

enum class Status {
    Ok,
    NoMemory,
    IoError,
    GeneralError
};

/**
 * Gets a static string description of a status code.
 *
 * @param status The status code.
 * @return The string representation of the code. "unknown error" if the code is not recognized.
 */
const char *strerror(Status status);

OCR_INLINE constexpr bool succeeded(Status status) {
    return status == Status::Ok;
}

OCR_INLINE constexpr bool failed(Status status) {
    return status != Status::Ok;
}

/**
 * Gets the number of bytes in a number of kilobytes.
 *
 * @param kb The number of kilobytes.
 * @return The number of bytes.
 */
OCR_INLINE constexpr size_t kb(size_t kb) {
    return kb * 1024;
}


/**
 * Gets the number of bytes in a number of megabytes.
 *
 * @param mb The number of megabytes.
 * @return The number of bytes.
 */
OCR_INLINE constexpr size_t mb(size_t mb) {
    return kb(mb) * 1024;
}


/**
 * Gets the number of bytes in a number of gigabytes.
 *
 * @param gb The number of gigabytes.
 * @return The number of bytes.
 */
OCR_INLINE constexpr size_t gb(size_t gb) {
    return mb(gb) * 1024;
}

}
