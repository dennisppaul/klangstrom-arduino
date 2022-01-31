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

#ifndef klangstrom_card_h
#define klangstrom_card_h

// @todo(add support for folders e.g `cd`, `get_folder_list`)

#include <WString.h>

#include <vector>

#include "KlangstromWaveFile.h"

using namespace std;

namespace klangstrom {
    class KlangstromCard {
    public:
        static const uint8_t FILE_READ_ONLY  = 0x00;
        static const uint8_t FILE_WRITE_ONLY = 0x01;
        static const uint8_t FILE_READ_WRITE = 0x02;

        static const uint8_t NO_ERROR                       = 0x00;
        static const uint8_t ERROR_FILE_NOT_OPEN            = 0x01;
        static const uint8_t ERROR_WAV_HEADER               = 0x02;
        static const uint8_t ERROR_WAV_16_BIT_ONLY          = 0x03;
        static const uint8_t ERROR_MEMORY_ALLOCATION_FAILED = 0x04;

        /**
         * @brief initialize the card
         *
         */
        virtual bool begin()                        = 0;
        virtual bool begin(const char *pFolderPath) = 0;

        /**
         * @brief retreive a list of files
         *
         * @param pFiles
         * @param pIncludeHiddenFiles
         */
        virtual void get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles = false) = 0;

        /**
         * @brief load float values (IEEE 754) into buffer
         *
         * @param pBuffer
         * @param pLength
         * @return int32_t number of bytes written to buffer. `-1` if error
         */
        int32_t load_float(float *pBuffer, uint32_t pLength) { return -1; }

        /**
         * @brief opens a file for reading, writing or both
         *
         * @param pFileName
         * @param pReadWriteFlag
         * @return int error code ( 0 = no error )
         */
        virtual int open(const String pFileName, const uint8_t pReadWriteFlag = FILE_READ_ONLY) = 0;

        virtual bool is_open() = 0;

        /**
         * @brief closes a file and releases all resources
         *
         */
        virtual void close() = 0;

        virtual void print_WAV_header(WaveHeader_t *mHeader) = 0;

        uint8_t load_WAV_header(WaveHeader_t *mHeader) {
            if (!is_open()) {
                debug_print_error("WAV file not open.\n");
                return ERROR_FILE_NOT_OPEN;
            }
            const uint8_t mHeaderSize = sizeof(WaveHeader_t);
            size_t        mBytesRead  = BSP_read_block((uint8_t *)mHeader, mHeaderSize);

            if (mBytesRead <= 0) {
                debug_print_error("could not read WAV file header.\n");
                return ERROR_WAV_HEADER;
            }

            return NO_ERROR;
        }

        uint8_t load_WAV_data(WaveHeader_t *mHeader) {
            return NO_ERROR;
        }

        /**
         * @brief loads the data of WAV file into instance
         *
         * @param pWaveFile
         * @return int error code ( 0 = no error )
         */
        int load_WAV(KlangstromWaveFile *pWaveFile) {
            WaveHeader_t *mHeader = new WaveHeader_t();

            /* read header */
            uint8_t mLoadHeaderResult = load_WAV_header(mHeader);
            if (mLoadHeaderResult != NO_ERROR) {
                return mLoadHeaderResult;
            }

            if (mHeader->bitsPerSample != 16) {
                debug_print_error("only 16bit WAV files are supported.\n");
                return ERROR_WAV_16_BIT_ONLY;
            }

            /* read data */
            const uint16_t mBytesPerSample = mHeader->bitsPerSample / 8;
            const uint8_t  mNumChannels    = mHeader->NumOfChan;
            const uint32_t mNumSamples     = mHeader->Subchunk2Size / mBytesPerSample / mNumChannels;
            uint8_t        mReadBuffer[READ_BUFFER_SIZE];

            pWaveFile->header            = mHeader;
            pWaveFile->sample_data       = new uint8_t[mNumSamples * mNumChannels * mBytesPerSample];
            pWaveFile->number_of_samples = mNumSamples;

            if (pWaveFile->sample_data == nullptr) {
                return ERROR_MEMORY_ALLOCATION_FAILED;
            }

            int32_t       mSampleFrameIndex = 0;
            size_t        mElementsRead     = 0;
            const uint8_t mElementSize      = mHeader->bytePerSampleFrame;
            while ((mElementsRead = BSP_read_block(mReadBuffer, READ_BUFFER_SIZE)) > 0) {
                for (uint32_t i = 0; i < mElementsRead; i += mElementSize) {
                    for (uint8_t j = 0; j < mNumChannels; j++) {
                        const uint32_t mReadBufferIndex                    = i + mBytesPerSample * j;
                        const uint32_t mSampleBufferIndex                  = mSampleFrameIndex + mNumSamples * j;
                        pWaveFile->sample_data[mSampleBufferIndex * 2 + 0] = mReadBuffer[mReadBufferIndex + 0];
                        pWaveFile->sample_data[mSampleBufferIndex * 2 + 1] = mReadBuffer[mReadBufferIndex + 1];
                    }
                    mSampleFrameIndex++;
                }
            }

            return NO_ERROR;
        }

    protected:
        static const uint16_t READ_BUFFER_SIZE = 128;

        virtual int  BSP_read_block(uint8_t *pReadBuffer, uint32_t pReadBufferSize) = 0;
        virtual void debug_print_error(const char *pString)                         = 0;

        inline int16_t BYTES_TO_INT_16(uint8_t *raw) {
            /* @todo(what about endianess?) */
            return *(int16_t *)raw;
        }

        inline float BYTE_TO_FLOAT_32(uint8_t *raw) {
            /* @todo(what about endianess?) */
            // check https://stackoverflow.com/questions/12242187/how-can-i-convert-4-bytes-storing-an-ieee-754-floating-point-number-to-a-float-v
            return *(float *)raw;
        }
    };
}  // namespace klangstrom

extern klangstrom::KlangstromCard *CardPtr;
#define Card (*CardPtr)

#endif  // klangstrom_card_h