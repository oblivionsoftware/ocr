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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The global logging level, any messages at a lower level than this are discarded at compile time.
 */
#define OCR_LOG_LEVEL OCR_LOG_INFO

/**
 * Convenience logging macro that adds the current file and line.

 * @param level The logging level.
 */
#define OCR_LOG(level, ...) ocr_log(level, __FILE__, __LINE__, __VA_ARGS__)

/**
 * Gets whether or not the specified log level is enabled.
 *
 * @param level The log level.
 * @return Non-zero if enabled, zero otherwise.
 */
#define OCR_LOG_ENABLED(level) (OCR_LOG_LEVEL <= (level))

#if OCR_LOG_ENABLED(OCR_LOG_TRACE)
    #define OCR_TRACE(...) OCR_LOG(OCR_LOG_TRACE, __VA_ARGS__)
#else
    #define OCR_TRACE(...)
#endif

#if OCR_LOG_ENABLED(OCR_LOG_DEBUG)
    #define OCR_DEBUG(...) OCR_LOG(OCR_LOG_DEBUG, __VA_ARGS__)
#else
    #define OCR_DEBUG(...)
#endif

#if OCR_LOG_ENABLED(OCR_LOG_INFO)
    #define OCR_INFO(...) OCR_LOG(OCR_LOG_INFO, __VA_ARGS__)
#else
    #define OCR_INFO(...)
#endif

#if OCR_LOG_ENABLED(OCR_LOG_WARN)
    #define OCR_WARN(...) OCR_LOG(OCR_LOG_WARN, __VA_ARGS__)
#else
    #define OCR_WARN(...)
#endif

#if OCR_LOG_ENABLED(OCR_LOG_ERROR)
    #define OCR_ERROR(...) OCR_LOG(OCR_LOG_ERROR, __VA_ARGS__)
#else
    #define OCR_ERROR(...)
#endif

typedef enum ocr_log_level {
    OCR_LOG_TRACE,
    OCR_LOG_DEBUG,
    OCR_LOG_INFO,
    OCR_LOG_WARN,
    OCR_LOG_ERROR
} ocr_log_level_t;

/**
 * Logs the specified output to the default log sink.
 *
 * @param level The log message level.
 * @param file The file name.
 * @param line The line number.
 * @param format The message format.
 */
void ocr_log(ocr_log_level_t level, const char *file, int line, const char *format, ...);

#ifdef __cplusplus
}
#endif
