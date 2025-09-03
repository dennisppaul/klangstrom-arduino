/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Klangstrom.h"
#ifdef KLST_PERIPHERAL_ENABLE_SD_CARD
#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_STM32

#include <stdarg.h>
#include <stdio.h>

#include "main.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "SDCard.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KLST_DISABLE_SDCARD_PRINT_CLIENT
#define println(format, ...) _println_(format, ##__VA_ARGS__)
#else
#define println(format, ...)
#endif // KLST_DISABLE_SDCARD_PRINT_CLIENT

extern SD_HandleTypeDef hsd2;

// TODO maybe replace with 'console_printf'
static void _println_(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n\r");
    va_end(args);
}

bool sdcard_init() {
    MX_SDMMC2_SD_Init();
    MX_FATFS_Init();
#ifdef KLST_SDCARD_SLOW_INITIALIZATION
    // TODO is not necessary with all tested SD cards so far ...
    /* a slower initialization may help with slower or older SD cards */
    hsd2.Init.ClockDiv = 0x20; // `SDMMC_INIT_CLK_DIV` or another divider ...
                               // clock speed is 25 MHz, reasonable init speed seems to be 400KHz
                               // so a divider of 25MHz / 400KHz = 62.5 would be ideal, but a divider
                               // value of ( 0x20 = 2 * 32 = ) 64 is the closest possible value.
    if (HAL_SD_Init(&hsd2) != HAL_OK) {
        println("SDCard: init failed");
        return false;
    }
    hsd2.Init.ClockDiv = 0; // reset to default i.e 25 MHz
#endif

    if (HAL_SD_Init(&hsd2) != HAL_OK) {
        println("SDCard: init failed");
        return false;
    }
    //    println("SDCard: init completed");
    return true;
}

bool sdcard_deinit() {
    FATFS_UnLinkDriver(SDPath);
    if (HAL_SD_DeInit(&hsd2) != HAL_OK) {
        println("SDCard: deinit failed");
        return false;
    }
    //    println("SDCard: deinit completed");
    return true;
}

bool sdcard_reinit() {
    //    uint32_t state = HAL_SD_GetState(&hsd2);
    //    uint32_t card_state = HAL_SD_GetCardState(&hsd2);
    //    println("state      : %i", state);
    //    println("card state : %i", card_state);
    HAL_StatusTypeDef status = HAL_SD_InitCard(&hsd2);
    if (status != HAL_OK) {
        println("SDCard: card re-init failed");
        return false;
    }
    return true;
}

bool sdcard_detected() {
    if (!HAL_GPIO_ReadPin(_CARD_SDMMC_CHIP_DETECT_GPIO_Port, _CARD_SDMMC_CHIP_DETECT_Pin)) {
        return true;
    } else {
        return false;
    }
}

bool sdcard_status() {
    println("SDCard: checking status");

    if (sdcard_detected()) {
        println("SDCard: card detected");
    } else {
        println("SDCard: no card detected");
        return false;
    }

    HAL_SD_CardStatusTypeDef card_status;
    HAL_StatusTypeDef        status = HAL_SD_GetCardStatus(&hsd2, &card_status);
    if (status != HAL_OK) {
        println("SDCard: get card status failed");
        return false;
    }
    println("SDCard: # Card Status");
    println("SDCard: DataBusWidth ............ : %i", card_status.DataBusWidth);
    println("SDCard: SecuredMode ............. : %i", card_status.SecuredMode);
    println("SDCard: CardType ................ : %i", card_status.CardType);
    println("SDCard: ProtectedAreaSize ....... : %li", card_status.ProtectedAreaSize);
    println("SDCard: SpeedClass .............. : %i", card_status.SpeedClass);
    println("SDCard: PerformanceMove ......... : %i", card_status.PerformanceMove);
    println("SDCard: AllocationUnitSize ...... : %i", card_status.AllocationUnitSize);
    println("SDCard: EraseSize ............... : %i", card_status.EraseSize);
    println("SDCard: EraseTimeout ............ : %i", card_status.EraseTimeout);
    println("SDCard: EraseOffset ............. : %i", card_status.EraseOffset);
    println("SDCard: UhsSpeedGrade ........... : %i", card_status.UhsSpeedGrade);
    println("SDCard: UhsAllocationUnitSize ... : %i", card_status.UhsAllocationUnitSize);
    println("SDCard: VideoSpeedClass ......... : %i", card_status.VideoSpeedClass);

    HAL_SD_CardInfoTypeDef card_info;
    status = HAL_SD_GetCardInfo(&hsd2, &card_info);
    if (status != HAL_OK) {
        println("SDCard: get card info failed");
        return false;
    }
    println("SDCard: # Card Info");
    println("SDCard: CardType ................ : %li", card_info.CardType);
    println("SDCard: CardVersion ............. : %li", card_info.CardVersion);
    println("SDCard: Class ................... : %li", card_info.Class);
    println("SDCard: RelCardAdd .............. : %li", card_info.RelCardAdd);
    println("SDCard: BlockNbr ................ : %li", card_info.BlockNbr);
    println("SDCard: BlockSize ............... : %li", card_info.BlockSize);
    println("SDCard: LogBlockNbr ............. : %li", card_info.LogBlockNbr);
    println("SDCard: LogBlockSize ............ : %li", card_info.LogBlockSize);
    println("SDCard: CardSpeed ............... : %li", card_info.CardSpeed);

    println("SDCard: SDPath .................. : %c%c%c%c", SDPath[0], SDPath[1], SDPath[2], SDPath[3]);
    println("SDCard: _MAX_SS ................. : %i", _MAX_SS);

    return true;
}

bool sdcard_mount() {
    println("SDCard: mounting FS ...");
    if (!HAL_SD_GetCardState(&hsd2)) {
        println("SDCard: card not ready … reinitializing");
        sdcard_reinit();
    }
    if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 1) != FR_OK) {
        println("SDCard: f_mount failed");
        return false;
    }
    return true;
}

bool sdcard_unmount() {
    //    println("SDCard: ... unmounting FS");
    f_mount(&SDFatFS, (TCHAR const*) NULL, 0);
    return true;
}

bool sdcard_format(uint8_t format) {
    const uint8_t mFormat = (format == 0) ? FM_ANY : format;
    BYTE          work[_MAX_SS];
    println("SDCard: creating volume ( this may take a while approx 6sec/1GB ) ...");
    if (f_mkfs((TCHAR const*) SDPath, mFormat, 0, work, sizeof(work)) != FR_OK) {
        println("SDCard: f_mkfs failed");
        return false;
    }
    return true;
}

bool sdcard_list(std::string path, std::vector<std::string>& files, std::vector<std::string>& directories, bool show_hidden_files) {
    // TODO implement show_hidden_files
    //    println("SDCard: list all directories + files");
    DIR     dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, path.c_str());
    if (res != FR_OK) {
        println("SDCard: f_opendir failed : %i", res);
        return false;
    }

    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) {
            break; // Break on error or end of dir
        }

        // Print the name of the file or directory
        if (fno.fattrib & AM_DIR) {
            //            println("        Directory: %s", fno.fname);
            directories.push_back(fno.fname);
        } else {
            //            println("        File     : %s", fno.fname);
            files.push_back(fno.fname);
        }
    }
    f_closedir(&dir);
    return true;
}

bool sdcard_file_open(std::string filepath, uint8_t flags) {
    BYTE mFlags = FA_READ; // TODO not so sure about this … what about the other options?
    switch (flags) {
        case FILE_READ_ONLY:
            mFlags = FA_READ;
            break;
        case FILE_WRITE_ONLY:
            mFlags = FA_WRITE;
            break;
        case FILE_READ_WRITE:
            mFlags = FA_READ | FA_WRITE;
            break;
    }
    FRESULT res = f_open(&SDFile, filepath.c_str(), mFlags);
    if (res != FR_OK) {
        println("SDCard: f_open failed : %i", res);
        return false;
    }
    //    println("SDCard: opened file");
    return true;
}

uint32_t sdcard_file_write(uint8_t* bytes, uint32_t bytes_to_write) {
    println("SDCard: writing to file");
    uint32_t byteswritten;
    FRESULT  res = f_write(&SDFile, bytes, bytes_to_write, (UINT*) &byteswritten);
    if (res != FR_OK || (byteswritten == 0)) {
        println("SDCard: f_write failed : %i", res);
        return 0;
    }
    //    println("SDCard: written %i bytes", byteswritten);
    return byteswritten;
}

uint32_t sdcard_file_read(uint8_t* bytes, uint32_t bytes_to_read) {
    uint32_t bytesread;
    FRESULT  res = f_read(&SDFile, bytes, bytes_to_read, (UINT*) &bytesread);
    if (res != FR_OK) {
        println("SDCard: f_read failed: %i", res);
        return 0;
    }
    //    println("SDCard: read %i bytes", bytesread);
    return bytesread;
}

bool sdcard_file_close() {
    FRESULT res = f_close(&SDFile);
    if (res != FR_OK) {
        println("SDCard: closing file failed : %i", res);
        return false;
    }
    //    println("SDCard: closed file");
    return true;
}

bool sdcard_file_create(const std::string filename) {
    FRESULT res = f_open(&SDFile, filename.c_str(), FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        println("SDCard: error creating file: %d", res);
        if (res == FR_INVALID_NAME && filename.size() > 12) {
            println("SDCard: make sure filesystem supports long ( > 12 ) filenames");
        }
        return false;
    }
    //    println("SDCard: creating file: %s", filename.c_str());
    return true;
}

bool sdcard_file_seek(uint32_t position) {
    FRESULT res = f_lseek(&SDFile, position);
    if (res != FR_OK) {
        println("SDCard: error seeking in file: %d", res);
        f_close(&SDFile);
        return false;
    }
    return true;
}

bool sdcard_file_eof() {
    if (f_eof(&SDFile)) {
        println("SDCard: end of file reached");
        return true;
    }
    return false;
}

static void print_attributes(unsigned char attr) {
    if (attr & AM_RDO) {
        println("Read only");
    }
    if (attr & AM_HID) {
        println("Hidden");
    }
    if (attr & AM_SYS) {
        println("System");
    }
    if (attr & AM_DIR) {
        println("Directory");
    }
    if (attr & AM_ARC) {
        println("Archive");
    }
}

bool sdcard_file_attributes(const std::string filename) {
    FILINFO fno;
    FRESULT res = f_stat(filename.c_str(), &fno);
    if (res != FR_OK) {
        println("SDCard: error getting file attributes: %d", res);
        return false;
    }
    println("SDCard: file attributes");
    println("SDCard: File name: %s", fno.fname);
    println("SDCard: File size: %li", fno.fsize);
    println("SDCard: File date: %i", fno.fdate);
    println("SDCard: File time: %i", fno.ftime);
    println("SDCard: File attr: %i", fno.fattrib);
    print_attributes(fno.fattrib);
    return true;

    //#if _USE_CHMOD && !_FS_READONLY
    //    FRESULT res1;
    //    res1 = f_chmod("TEILCHEN.RAW", 0, AM_RDO); // Clear the read-only attribute
    //    if (res1 != FR_OK) {
    //        printf("Error changing file attributes: %d\n", res1);
    //    }
    //#endif
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PERIPHERAL_ENABLE_SD_CARD
#endif // KLST_PANDA_STM32