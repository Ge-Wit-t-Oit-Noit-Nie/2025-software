/**
 ******************************************************************************
 * @file   logger.c
 * @brief  Implementation of the logger
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

#include "fatfs.h"
#include "logger.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "internal_sensors.h"

#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define CLEAR_BUFFER(buffer) memset(buffer, '\0', sizeof(buffer))

FATFS filesystem; // file system

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
void logger_task(void *argument)
{
	UNUSED(argument); // Mark variable as 'UNUSED' to suppress 'unused-variable' warning
	telemetry_t telemetry;
	osStatus_t status;
	char string[BUFFER_SIZE];	  // to store strings..
	uint8_t mount_status = FR_OK; // flag to check if the card is enabled
	FIL fil;					  // File
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;

	osMutexId_t mutex_id;
	mutex_id = osMutexNew(NULL);

	while (1)
	{
		status = osMessageQueueGet(loggerQueueHandle, &telemetry, NULL, osWaitForever); // wait for message
		if (osOK == status)
		{
			is_get_date_time(&gDate , &gTime); // get the time from the RTC

			CLEAR_BUFFER(string);
			snprintf(string, BUFFER_SIZE, "[%02d:%02d:%02d],0x%08X,0x%08X,0x%08X,0x%08X,0x%08X\n\r",
					 gTime.Hours,
					 gTime.Minutes,
					 gTime.Seconds,
					 telemetry.instruction_pointer,
					 telemetry.shutdown_index_register,
					 telemetry.temperature,
					 telemetry.vrefint,
					 telemetry.trigger); // format the string to write to the file
			printf(string); // print to uart

			if (osOK == osMutexAcquire(mutex_id, 0))
			{
				mount_status = f_mount(&filesystem, "", 1); // mount the file system
				if (FR_OK == mount_status)
				{
					if (FR_OK == f_open(&fil, LOG_FILENAME, FA_OPEN_APPEND | FA_READ | FA_WRITE))
					{
						/* write the string to the file */
						f_puts(string, &fil);
					}
					else
					{
						printf("Cannot open file (errno: %d)\n\r", mount_status);
					}
					f_close(&fil);
					f_mount(NULL, "", 0); // unmount the file system
				}
				else
				{
					printf("Card not mounted (errno: %d)\n\r", mount_status);
					USER_initialize(0); // Call USER_initialize with 0 this is in user diskio
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