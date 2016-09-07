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

#define OCR_PLATFORM_WINDOWS 0
#define OCR_PLATFORM_LINUX 1
#define OCR_PLATFORM_MAC 2

#ifdef _WIN32
    #define OCR_PLATFORM OCR_PLATFORM_WINDOWS
    #define OCR_PLATFORM_NAME "Windows"
#elif __APPLE__
    #define OCR_PLATFORM OCR_PLATFORM_MAC
    #define OCR_PLATFORM_NAME "Mac OSX"
#elif __linux__
    #define OCR_PLATFOMR OCR_PLATFORM_LINUX
    #define OCR_PLATFORM_NAME "Linux"
#else
    #error "Unsupported Platform"
#endif

#ifdef __cplusplus
    #define OCR_EXTERN_C_BEGIN extern "C" {
    #define OCR_EXTERN_C_END }
#else
    #define OCR_EXTERN_C_BEGIN
    #define OCR_EXTERN_C_END
#endif

/**
 * Gets whether or not the specified status is successful.
 *
 * @param status The status to check.
 * @return Non-Zero if the status is OCR_OK, false otherwise.
 */
#define OCR_SUCCEEDED(status) ((status) == OCR_OK)

/**
 * Gets whether or not the specified status indicates failure.
 *
 * @param status The status to check.
 * @return Non-zero if the status is not OCR_OK, false otherwise.
 */
#define OCR_FAILED(status) ((status) != OCR_OK)

OCR_EXTERN_C_BEGIN

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

typedef enum ocr_status {
    OCR_OK,
    OCR_NO_MEMORY,
    OCR_IO_ERROR,
} ocr_status_t;


/**
 * Gets a static string description of a status code.
 *
 * @param status The status code.
 * @return The string representation of the code. "unknown error" if the code is not recognized.
 */
const char *ocr_strerror(ocr_status_t status);


/**
 * Gets the number of bytes in a number of kilobytes.
 *
 * @param kb The number of kilobytes.
 * @return The number of bytes.
 */
OCR_INLINE size_t ocr_kb(size_t kb)
{
    return kb * 1024;
}


/**
 * Gets the number of bytes in a number of megabytes.
 *
 * @param mb The number of megabytes.
 * @return The number of bytes.
 */
OCR_INLINE size_t ocr_mb(size_t mb)
{
    return ocr_kb(mb) * 1024;
}


/**
 * Gets the number of bytes in a number of gigabytes.
 *
 * @param gb The number of gigabytes.
 * @return The number of bytes.
 */
OCR_INLINE size_t ocr_gb(size_t gb)
{
    return ocr_mb(gb) * 1024;
}

OCR_EXTERN_C_END
