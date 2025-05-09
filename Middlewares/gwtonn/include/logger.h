/**
 ******************************************************************************
 * @file   logger.h
 * @brief  Headerfile for the logger.c file
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
#ifndef INC_SD_LOGGER_H_
#define INC_SD_LOGGER_H_

/// Filename for the logger to write to.
#define LOG_FILENAME "logger.txt"

#ifdef __cplusplus
extern "C" {
#endif // End of extern "C" block

/// Definitions for loggerQueue
extern osMessageQueueId_t loggerQueueHandle;
/// Attributes for the loggerQueue  message queue.
extern const osMessageQueueAttr_t loggerQueue_attributes;

/**
 * @brief Message Queue Object
 *
 * This object is used to pass messages between tasks.
 *  - message: the message to pass
 *  - program_counter: the status of the program counter
 *  - shutdown_index_register: the status of the shutdown index register
 */
typedef struct
{
    uint16_t instruction_pointer;
    uint16_t shutdown_index_register;
    uint16_t temperature;
    uint16_t vrefint;
    uint8_t trigger;
} telemetry_t;

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif /* INC_SD_LOGGER_H_ */
