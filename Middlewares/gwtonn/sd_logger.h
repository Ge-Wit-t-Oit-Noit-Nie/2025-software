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
 *  - index: the index of the message
 */
typedef struct
{
    uint8_t message;
    uint8_t index;
} MSGQUEUE_OBJ_t;

/**
* @brief Open the SD Card and prepare for writing.
* 
* This function mounts the file system on the SD card.
*/
void sd_logger_open();
/**
 * @brief Close the SD Card 
 *
 * This function unmounts the file system on the SD card.
 */
void sd_logger_close();
/**
 * @brief Write to the SD Card
 *
 * This function writes to the SD Card
 */
void sd_logger_print(char *);

#endif /* INC_SD_LOGGER_H_ */
