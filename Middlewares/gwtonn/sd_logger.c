/*
 * sd_logger.c
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#include "fatfs.h"
#include "sd_logger.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define CLEAR_BUFFER(buffer) memset(buffer, '\0', sizeof(buffer))

FATFS filesystem;  // file system

/**
 * @brief Function implementing the logTask thread.
 * 
 * This function will run a log task that will write to the SD card.
 * It will wait for messages from the loggerQueue and write them to the SD card.
 * 
 * In case the card cannot be mounted, the task will clear the buffer and disable the card.
 * 
 * @param argument: Not used
 * @retval None
 */
void startLogTask(void *argument)
{
	UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning
	MSGQUEUE_OBJ_t msg;
	osStatus_t status;
	char string[BUFFER_SIZE]; // to store strings..
	uint8_t mount_status = FR_OK; // flag to check if the card is enabled
	FIL fil;					  // File

	osMutexId_t mutex_id;
	mutex_id = osMutexNew(NULL);

	while (1)
	{
		status = osMessageQueueGet(loggerQueueHandle, &msg, NULL, osWaitForever); // wait for message
		if (osOK == status)
		{
			CLEAR_BUFFER(string);
			snprintf(string, BUFFER_SIZE, "%d,%ld,%d\n", msg.message, msg.program_counter, msg.shutdown_index_register);
			printf(string);

			if (osOK == osMutexAcquire(mutex_id, 0))
			{
				mount_status = f_mount(&filesystem, "", 1); // mount the file system
				if (FR_OK == mount_status) {
					if (FR_OK == f_open(&fil, LOG_FILENAME, FA_OPEN_APPEND | FA_READ | FA_WRITE))
					{
						/* write the string to the file */
						f_puts(string, &fil);
					}
					f_close(&fil);
					mount_status = f_mount(&filesystem, "", 1); // mount the file system
				} else {
					printf("Card not mounted\n\r");
				}
				osMutexRelease(mutex_id);
			}
			else
			{
				printf("Could not get the MUTEX 'mutex_sd_card_lockHandle'\n\r");
			}
		}
	}

	osMutexDelete(mutex_id);
}