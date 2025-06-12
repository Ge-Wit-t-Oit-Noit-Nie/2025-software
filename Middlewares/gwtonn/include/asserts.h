/**
 ******************************************************************************
 * @file   asserts.h
 * @brief  Define assert macros for error handling
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 Ge Wit't Oit Noit Nie.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef __MIDDLEWARES_GWTONN_UTILS_H
#define __MIDDLEWARES_GWTONN_UTILS_H

#if defined(__cplusplus)
extern "C"
{ // Use C linkage for C++ compilers
#endif

#define ASSERT_NULL(x, err_code)             \
    if (NULL == x)                           \
    {                                        \
        printf("Error: %s is NULL\n\r", #x); \
        return err_code;                     \
    }

#if defined(__cplusplus)
}
#endif // End of extern "C" block

#endif /* __MIDDLEWARES_GWTONN_UTILS_H */