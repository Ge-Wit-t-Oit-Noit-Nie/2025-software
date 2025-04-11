/*
 * sd_logger.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#ifndef INC_SD_LOGGER_H_
#define INC_SD_LOGGER_H_

/// Filename for the logger to write to.
#define LOG_FILENAME "logger.txt"

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
    uint8_t message;
    uint32_t program_counter;
    uint16_t shutdown_index_register;
} MSGQUEUE_OBJ_t;

#endif /* INC_SD_LOGGER_H_ */
