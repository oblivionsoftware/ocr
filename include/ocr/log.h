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

/**
 * The global logging leve, any messages at a lower level than this are discarded at compile time.
 */
#define OCR_LOG_LEVEL OCR_LOG_INFO

/**
 * Convenience macro to log at the specified level only if enabled.
 */
#define OCR_LOG(level, ...) do {\
    if (OCR_LOG_LEVEL <= (level)) {\
        ocr_log(level, __FILE__, __LINE__, __VA_ARGS__); \
    }\
} while (0)

#define OCR_TRACE(...) OCR_LOG(OCR_LOG_TRACE, __VA_ARGS__)
#define OCR_DEBUG(...) OCR_LOG(OCR_LOG_DEBUG, __VA_ARGS__)
#define OCR_INFO(...) OCR_LOG(OCR_LOG_INFO, __VA_ARGS__)
#define OCR_WARN(...) OCR_LOG(OCR_LOG_WARN, __VA_ARGS__)
#define OCR_ERROR(...) OCR_LOG(OCR_LOG_ERROR, __VA_ARGS__)

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
