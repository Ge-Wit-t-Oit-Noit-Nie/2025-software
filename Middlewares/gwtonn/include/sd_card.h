#ifndef __GWTONN_INCLUDE_SD_CARD_H__
#define __GWTONN_INCLUDE_SD_CARD_H__

#include "fatfs.h"

#ifdef __cplusplus
extern "C"
{
#endif // End of extern "C" block

    /**
     * \brief  Check on the SD Card if a file exists.
     *
     * This function will check if the file exists on the SD Card. It will mount
     * the SD Card and check if the file exists. If it does, it will return FR_OK,
     * else it will return any of the #FRESULT.
     * \param[in] filename the filename to check to
     * \retval FR_OK in case of sucess; else any of the #FRESULT.
     */
    FRESULT file_exists(const char *filename);
    /**
     * \brief  Write a file to the SD Card.
     *
     * Writing to the SD Card requires some extra steps, such as mounting the SD
     * Card and checking the status. This function handles that all. After the
     * checkes, it will open the file for APPEND and write the content of content
     * to the file.
     * \param[in] filename the filename to write to
     * \param[in] content the content to write (must the \0 terminated)
     * \retval FR_OK in case of sucess; else any of the #FRESULT.
     */
    FRESULT
    write_file(const char *filename, const char *content);
    /**
     * \brief  Load the program (binary) into the specified memory location.
     *
     * This function will read the binary and load it into the specific memory
     * location.
     *
     * \param[in] filename the filename to read from to
     * \param[in] address_memory the location to write to
     * \retval FR_OK in case of sucess; else any of the #FRESULT.
     */
    FRESULT load_progam_from_sd_to_flash(const char *filename, uint32_t address_memory);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif //__GWTONN_INCLUDE_SD_CARD_H__