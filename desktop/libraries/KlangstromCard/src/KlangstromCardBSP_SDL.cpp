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

#if KLST_ARCH == KLST_ARCH_CPU

#include <stdio.h>
#include <string.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "KlangstromCardBSP_SDL.h"
#include "KlangstromWaveFile.h"
#include "tinydir.h"
#include "tinyfiledialogs.h"

using namespace klangstrom;

bool klangstrom::KlangstromCardBSP_SDL::begin(const char *pFolderPath) {
    mFolderPath = pFolderPath;
    return true;
}

bool klangstrom::KlangstromCardBSP_SDL::begin() {
    mFolderPath = tinyfd_selectFolderDialog("Select a directory", NULL);
    if (!mFolderPath) {
        tinyfd_messageBox(
            "Error",
            "Select folder name is not valid",
            "ok",
            "error",
            1);
        return true;
    }
    return false;
    // tinyfd_messageBox("The selected folder is", mFolderPath, "ok", "info", 1);
}

void klangstrom::KlangstromCardBSP_SDL::get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles) {
    if (mFolderPath != nullptr) {
        tinydir_dir dir;
        tinydir_open_sorted(&dir, mFolderPath);
        for (int i = 0; i < dir.n_files; i++) {
            tinydir_file file;
            tinydir_readfile_n(&dir, &file, i);
            if (!file.is_dir && (!KlangstromCard_is_prefixed(".", file.name) || pIncludeHiddenFiles)) {
                pFiles.push_back(file.name);
                // pFiles.push_back(std::string{dir->path} + "/" + file.name);
            }
        }
        tinydir_close(&dir);
    }
}

int klangstrom::KlangstromCardBSP_SDL::create_file(const String pFileName) {
    const String mFilePath = KlangstromCard_get_full_path(pFileName);
    mOutFile               = fopen(mFilePath.c_str(), "wb");

    if (mOutFile == nullptr) {
        debug_print_error("could not open file: ");
        debug_print_error(mFilePath.c_str());
        return 1;
    }
    return 0;
}

int klangstrom::KlangstromCardBSP_SDL::open(const String pFileName, const uint8_t pReadWriteFlag) {
    const String mFilePath = KlangstromCard_get_full_path(pFileName);
    switch (pReadWriteFlag) {
        case FILE_READ_ONLY:
            mFile = fopen(mFilePath.c_str(), "r");
            break;
        case FILE_WRITE_ONLY:
            mFile = fopen(mFilePath.c_str(), "w");
            break;
        case FILE_READ_WRITE:
            mFile = fopen(mFilePath.c_str(), "r+");
            break;
    }

    if (mFile == nullptr) {
        debug_print_error("could not open file: ");
        debug_print_error(mFilePath.c_str());
        return 1;
    }
    return 0;
}

bool klangstrom::KlangstromCardBSP_SDL::is_open() {
    return mFile != nullptr;
}

void klangstrom::KlangstromCardBSP_SDL::close() {
    if (mFile != nullptr) {
        fclose(mFile);
        mFile = nullptr;
    }
    if (mOutFile != nullptr) {
        fclose(mOutFile);
        mOutFile = nullptr;
    }
}

// /* C++17 */
// void klangstrom::KlangstromCard::get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles) {
//     std::string path = mFolderPath;
//     // printf("+++ listing files in directory: \r\n");
//     for (const auto &entry : std::filesystem::directory_iterator(path)) {
//         pFiles.push_back(String(entry.path().c_str()));
//     }
// }

// static int getFileSize(FILE *inFile) {
//     int fileSize = 0;
//     fseek(inFile, 0, SEEK_END);

//     fileSize = ftell(inFile);

//     fseek(inFile, 0, SEEK_SET);
//     return fileSize;
// }

void klangstrom::KlangstromCardBSP_SDL::print_WAV_header(WaveHeader_t *mHeader) {
    const uint16_t mBytesPerSample = mHeader->bitsPerSample / 8;
    const uint32_t mNumSamples     = mHeader->Subchunk2Size / mBytesPerSample / mHeader->NumOfChan;

    // cout << "File is                    : " << getFileSize(mFile) << " bytes." << endl;
    cout << "RIFF header                : " << mHeader->RIFF[0] << mHeader->RIFF[1] << mHeader->RIFF[2] << mHeader->RIFF[3] << endl;
    cout << "WAVE header                : " << mHeader->WAVE[0] << mHeader->WAVE[1] << mHeader->WAVE[2] << mHeader->WAVE[3] << endl;
    cout << "FMT                        : " << mHeader->fmt[0] << mHeader->fmt[1] << mHeader->fmt[2] << mHeader->fmt[3] << endl;
    // cout << "Data size                  : " << mHeader->ChunkSize << endl;
    // Display the sampling Rate from the header
    cout << "Sampling Rate              : " << mHeader->SamplesPerSec << endl;
    cout << "Bits per sample            : " << mHeader->bitsPerSample << endl;
    cout << "Number of channels         : " << mHeader->NumOfChan << endl;
    cout << "Number of bytes per second : " << mHeader->bytesPerSec << endl;
    cout << "Data length                : " << mHeader->Subchunk2Size << endl;
    cout << "Audio Format               : " << mHeader->AudioFormat << endl;  // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    cout << "Number of Samples          : " << mNumSamples << endl;
    cout << "Bytes per Sample Frame     : " << mHeader->bytePerSampleFrame << endl;
    cout << "Data string                : " << mHeader->Subchunk2ID[0] << mHeader->Subchunk2ID[1] << mHeader->Subchunk2ID[2] << mHeader->Subchunk2ID[3] << endl;
}

int klangstrom::KlangstromCardBSP_SDL::BSP_read_block(uint8_t *pReadBuffer, uint32_t pReadBufferSize) {
    return fread(pReadBuffer, 1, pReadBufferSize, mFile);
}

int klangstrom::KlangstromCardBSP_SDL::BSP_write_block(uint8_t *pWriteBuffer, uint32_t pWriteBufferSize, bool pSync) {
    return fwrite(pWriteBuffer, 1, pWriteBufferSize, mOutFile);
}

void klangstrom::KlangstromCardBSP_SDL::debug_print_error(const char *pString) {
    fprintf(stderr, "+++ %s\n", pString);
}

#endif  // KLST_ARCH == KLST_ARCH_CPU
