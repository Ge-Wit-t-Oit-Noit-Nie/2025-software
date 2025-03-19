/*
 * sd_logger.c
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#include "fatfs.h"
#include "sd_logger.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <string.h>
#include <stdio.h>

FATFS filesystem;  // file system

void sd_logger_open() {
	f_mount(&filesystem, "/", 1);
}
void sd_logger_close() {
	f_mount(&filesystem, "/", 1);
}
void sd_logger_print(char *string) {
	FIL fil; // File

	if (FR_OK == f_open(&fil, LOG_FILENAME, FA_OPEN_APPEND | FA_READ | FA_WRITE))
	{
		/* write the string to the file */
		f_puts(string, &fil);
	}

	f_close (&fil);

}

