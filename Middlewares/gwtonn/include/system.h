/**
 ******************************************************************************
 * @file   system.h
 * @brief  Declaration of various system functions
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

#ifndef __MIDDLEWARES_GWTONN_SYSTEM_H
#define __MIDDLEWARES_GWTONN_SYSTEM_H 

/**
 * @brief  Function to send a character to the debug viewer.
 * @param  ch: Character to be sent.
 * @retval Character sent.
 */
int __io_putchar(int ch);

#endif /* __MIDDLEWARES_GWTONN_SYSTEM_H */