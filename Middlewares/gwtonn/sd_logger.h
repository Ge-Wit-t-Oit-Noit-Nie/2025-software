/*
 * sd_logger.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Rudi Middel
 */

#ifndef INC_SD_LOGGER_H_
#define INC_SD_LOGGER_H_

#define LOG_FILENAME "logger.txt"

void sd_logger_open();
void sd_logger_close();
void sd_logger_print(char *);

#endif /* INC_SD_LOGGER_H_ */
