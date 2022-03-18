/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#ifndef klangstrom_card_bsp_klst_h
#define klangstrom_card_bsp_klst_h

// @TODO add `SdFat` library files to klangstrom
#include <SPI.h>
#include <SdFat.h>

#include "KlangstromCard.h"

// @TODO(test unoptimzed implementation)
#define KLST_CARD_OPTIMIZE_SPACE
#ifndef KLST_CARD_PRINT_DEBUG
#define KLST_CARD_PRINT_DEBUG 0
#endif

using namespace std;

namespace klangstrom {
    class KLST_SDCARD_SPI : public SdSpiBaseClass {
    public:
        void activate() {
            mSPI.beginTransaction(m_spiSettings);
        }

        void begin(SdSpiConfig config) {
            (void)config;
            mSPI.setMOSI(SDCARD_SPI_MOSI);
            mSPI.setMISO(SDCARD_SPI_MISO);
            mSPI.setSCLK(SDCARD_SPI_SCK);
            mSPI.setSSEL(SDCARD_SPI_CS);
            mSPI.begin();
        }

        void deactivate() {
            mSPI.endTransaction();
        }

        uint8_t receive() {
            return mSPI.transfer(0XFF);
        }

        uint8_t receive(uint8_t *buf, size_t count) {
            for (size_t i = 0; i < count; i++) {
                buf[i] = mSPI.transfer(0XFF);
            }
            return 0;
        }

        void send(uint8_t data) {
            mSPI.transfer(data);
        }

        void send(const uint8_t *buf, size_t count) {
            for (size_t i = 0; i < count; i++) {
                mSPI.transfer(buf[i]);
            }
        }

        void setSckSpeed(uint32_t maxSck) {
            m_spiSettings = SPISettings(maxSck, MSBFIRST, SPI_MODE0);
        }

    private:
        SPISettings m_spiSettings;
        SPIClass    mSPI = SPIClass(
               SDCARD_SPI_MOSI,
               SDCARD_SPI_MISO,
               SDCARD_SPI_SCK,
               SDCARD_SPI_CS);
    };

    class KlangstromCardBSP_STM32 : public KlangstromCard {
    public:
        bool begin();
        bool begin(const char *pFolderPath);
        void get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles = false);
        int  open(const String pFileName, const uint8_t pReadWriteFlag = FILE_READ_ONLY);
        bool is_open();
        void close();
        void print_WAV_header(WaveHeader_t *mHeader);

        int  create_file(const String pFileName);

    protected:
        int  BSP_read_block(uint8_t *pReadBuffer, uint32_t pReadBufferSize);
        int  BSP_write_block(uint8_t *pWriteBuffer, uint32_t pWriteBufferSize, bool pSync);
        void debug_print_error(const char *pString);

    private:
#ifdef KLST_CARD_OPTIMIZE_SPACE
        SdFat32  sd;
        FatFile *mFile    = nullptr;
        FatFile *mOutFile = nullptr;
#else
        SdFat   sd;
        FsFile *mFile    = nullptr;
        FsFile *mOutFile = nullptr;
#endif

        KLST_SDCARD_SPI *mKLST_SDCARD_SPI = new KLST_SDCARD_SPI();
    };
}  // namespace klangstrom

#endif  // klangstrom_card_bsp_klst_h