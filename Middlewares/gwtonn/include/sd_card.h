#ifndef __GWTONN_INCLUDE_SD_CARD_H__
#define __GWTONN_INCLUDE_SD_CARD_H__

#include "fatfs.h"

#ifdef __cplusplus
extern "C"
{
#endif // End of extern "C" block

    FRESULT file_exists(const char *filename);
    FRESULT
    write_file(const char *filename, const char *content);
    FRESULT load_progam_from_sd_to_flash(const char *filename, uint32_t address_memory);

#ifdef __cplusplus
}
#endif // End of extern "C" block

#endif //__GWTONN_INCLUDE_SD_CARD_H__