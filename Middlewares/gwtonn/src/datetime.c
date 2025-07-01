
/**
 * @file datetime.c
 * @brief Implementation of date and time utility functions.
 *
 * This file contains functions for handling and manipulating date and time
 * values, such as formatting, parsing, and calculating differences between
 * dates. It is designed to be used in C projects that require basic datetime
 * operations without relying on external libraries.
 *
 * @author R. Middel
 * @date 2025-06-25
 * @version 1.0
 *
 * @see datetime.h
 *
 * @copyright Copyright (c) 2025 R. Middel.
 * This file is part of the Base project and is licensed under the MIT License.
 * See the LICENSE file in the root of the project for full license text.
 * SPDX-License-Identifier: MIT
 *
 */

#include "datetime.h"

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

/**
 * @brief Encodes a datetime structure into a dense 64-bit integer format.
 *
 */
void dt_encode(const datetime_t *datetime, dt_dense_time *encoded) {

    *encoded = (uint64_t)((int32_t)datetime->year + 0x8000);

    *encoded *= 12;
    *encoded += datetime->month;

    *encoded *= 31;
    *encoded += datetime->day;

    *encoded *= 24;
    *encoded += datetime->hour;

    *encoded *= 60;
    *encoded += datetime->minute;

    *encoded *= 60;
    *encoded += datetime->second;

    *encoded *= 1000;
    *encoded += datetime->millisecond;
}

/**
 * @brief Decodes a dense 64-bit timestamp into a datetime structure.
 *
 */
void dt_decode(dt_dense_time encoded, datetime_t *out) {

    out->millisecond = encoded % 1000;
    encoded /= 1000;

    out->second = encoded % 60;
    encoded /= 60;

    out->minute = encoded % 60;
    encoded /= 60;

    out->hour = encoded % 24;
    encoded /= 24;

    out->day = encoded % 31;
    encoded /= 31;

    out->month = encoded % 12;
    encoded /= 12;

    out->year = ((int32_t)encoded - 0x8000);
}

/**
 * @brief Prints a formatted timestamp to the specified output stream.
 *
 */
void dt_print(FILE *target, datetime_t *timestamp) {

    fprintf(target, DT_FORMAT_STRING, timestamp->year, timestamp->month,
            timestamp->day, timestamp->hour, timestamp->minute,
            timestamp->second, timestamp->millisecond);
}