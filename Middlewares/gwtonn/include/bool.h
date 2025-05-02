/**
 ******************************************************************************
 * @file   bool.h
 * @brief  Define boolean type and constants
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
#ifndef __MIDDLEWARES_GWTONN_BOOL_H
#define __MIDDLEWARES_GWTONN_BOOL_H

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {   // Use C linkage for C++ compilers
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#if defined (__cplusplus)
}
#endif // End of extern "C" block

#endif /* __MIDDLEWARES_GWTONN_BOOL_H */