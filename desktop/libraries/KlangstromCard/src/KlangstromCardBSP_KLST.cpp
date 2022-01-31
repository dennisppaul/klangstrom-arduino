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

#include "KlangstromDefinesArduino.h"

#if KLST_ARCH == KLST_ARCH_MCU

#include "KlangstromCardBSP_KLST.h"

using namespace klangstrom;

bool klangstrom::KlangstromCardBSP_KLST::begin() {
    if (!sd.begin(SdSpiConfig(SDCARD_SPI_CS,
                              SHARED_SPI,
                              SD_SCK_MHZ(50),
                              mKLST_SDCARD_SPI))) {
        debug_print_error("problem starting SD card");
        return true;
    }
    return false;
}

bool klangstrom::KlangstromCardBSP_KLST::begin(const char *pFolderPath) {
    // @TODO(maybe set some kind of file root here?)
    return begin();
}

void klangstrom::KlangstromCardBSP_KLST::get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles) {
#ifdef KLST_CARD_OPTIMIZE_SPACE
    FatFile root;
    FatFile file;
#else
    FsFile root;
    FsFile file;
#endif
    const static uint16_t MAX_FILENAME_LENGTH = 256;

    char mName[MAX_FILENAME_LENGTH];
    if (!root.open("/")) {
        debug_print_error("open root");
    }
    while (file.openNext(&root, O_RDONLY)) {
        if (!file.isHidden() || pIncludeHiddenFiles) {
            file.getName(mName, MAX_FILENAME_LENGTH);
            String s = mName;
            pFiles.push_back(s);
        }
        file.close();
    }
}

int klangstrom::KlangstromCardBSP_KLST::open(const String pFileName, const uint8_t pReadWriteFlag) {
    bool mIsOpen;
#ifdef KLST_CARD_OPTIMIZE_SPACE
    mFile = new FatFile();
    // @TODO decide if it s ok to use SFN aka 8.3 file names ...
    // mIsOpen = mFile->openExistingSFN(pFileName.c_str());
    mIsOpen = mFile->open(pFileName.c_str());
#else
    mFile = &(sd.open(pFileName));
#endif

    if (!mIsOpen) {
        debug_print_error("could not open file: ");
        return ERROR_FILE_NOT_OPEN;
    }
    return 0;
}

bool klangstrom::KlangstromCardBSP_KLST::is_open() {
    return mFile != nullptr && mFile->isOpen();
}

void klangstrom::KlangstromCardBSP_KLST::close() {
    if (mFile != nullptr) {  // && mFile->isOpen()) {
        mFile->close();
        delete mFile;
        mFile = nullptr;
    }
}

void klangstrom::KlangstromCardBSP_KLST::print_WAV_header(WaveHeader_t *mHeader) {
    const uint16_t mBytesPerSample = mHeader->bitsPerSample / 8;
    const uint32_t mNumSamples     = mHeader->Subchunk2Size / mBytesPerSample / mHeader->NumOfChan;

    Serial.print("RIFF header                ");
    Serial.print((char)mHeader->RIFF[0]);
    Serial.print((char)mHeader->RIFF[1]);
    Serial.print((char)mHeader->RIFF[2]);
    Serial.println((char)mHeader->RIFF[3]);
    Serial.print("WAVE header                ");
    Serial.print((char)mHeader->WAVE[0]);
    Serial.print((char)mHeader->WAVE[1]);
    Serial.print((char)mHeader->WAVE[2]);
    Serial.println((char)mHeader->WAVE[3]);
    Serial.print("FMT                        ");
    Serial.print((char)mHeader->fmt[0]);
    Serial.print((char)mHeader->fmt[1]);
    Serial.print((char)mHeader->fmt[2]);
    Serial.println((char)mHeader->fmt[3]);

    // Display the sampling Rate from the header
    Serial.print("Sampling Rate              ");
    Serial.println(mHeader->SamplesPerSec);
    Serial.print("Bits per sample            ");
    Serial.println(mHeader->bitsPerSample);
    Serial.print("Number of channels         ");
    Serial.println(mHeader->NumOfChan);
    Serial.print("Number of bytes per second ");
    Serial.println(mHeader->bytesPerSec);
    Serial.print("Data length                ");
    Serial.println(mHeader->Subchunk2Size);
    Serial.print("Audio Format               ");
    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    Serial.println(mHeader->AudioFormat);
    Serial.print("Number of Samples          ");
    Serial.println(mNumSamples);
    Serial.print("Bytes per Sample Frame     ");
    Serial.println(mHeader->bytePerSampleFrame);
    Serial.print("Data string                ");
    Serial.print((char)mHeader->Subchunk2ID[0]);
    Serial.print((char)mHeader->Subchunk2ID[1]);
    Serial.print((char)mHeader->Subchunk2ID[2]);
    Serial.println((char)mHeader->Subchunk2ID[3]);
}

int klangstrom::KlangstromCardBSP_KLST::BSP_read_block(uint8_t *pReadBuffer, uint32_t pReadBufferSize) {
    return mFile->read(pReadBuffer, pReadBufferSize);
}

void klangstrom::KlangstromCardBSP_KLST::debug_print_error(const char *pString) {
#if KLST_CARD_PRINT_DEBUG == 1
    Serial.print("+++ ");
    Serial.println(pString);
#endif
}

#endif  // KLST_ARCH == KLST_ARCH_MPU
