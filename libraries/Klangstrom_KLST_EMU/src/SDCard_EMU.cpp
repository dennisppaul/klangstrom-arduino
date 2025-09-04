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
* WITHOUT ANY WARRANTY{return true;} without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include <string>
#include "Console.h"
#include "SDCard.h"

#include "tinydir.h"
#include "tinyfiledialogs.h"

#include "KlangstromEmulator.h"

static bool mCardDetected = true;
static bool mMounted      = false;

class DrawableSDCard : public Drawable {
public:
    DrawableSDCard() {}

    void draw(umfeld::PGraphics* g) override {
        g->pushMatrix();
        g->translate(0, 0);
        if (mCardDetected) {
            g->fill(0, 255, 0);
        } else {
            g->fill(255, 0, 0);
        }
        g->popMatrix();
    }

private:
};

#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_MOUNTED(mMounted)                   \
    do {                                          \
        if (!(mMounted)) {                        \
            console_error("SDCard: not mounted"); \
            return false;                         \
        }                                         \
    } while (0)

#define CHECK_FILE(mFile)                    \
    do {                                     \
        if (mFile == nullptr) {              \
            console_error("no file opened"); \
            return false;                    \
        }                                    \
    } while (0)

static const char* mFolderPath = nullptr;
static FILE*       mFile       = nullptr;

static std::string get_full_path(std::string pFile) {
    return std::string(mFolderPath + pFile);
}

static bool is_prefixed(const char* pre, const char* str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool sdcard_init_dir(const char* pFolderPath) {
    mFolderPath = pFolderPath;
    return true;
}

bool sdcard_init() {
    static bool initialized = false;
    if (!initialized) {
        mFolderPath = tinyfd_selectFolderDialog("Select a directory", NULL);
        if (!mFolderPath) {
            tinyfd_messageBox(
                "Error",
                "Select folder name is not valid",
                "ok",
                "error",
                1);
            console_error("SDCard: could not set folder path.");
            return false;
        }
        initialized = true;
        umfeld::KlangstromEmulator::instance()->register_drawable(new DrawableSDCard());
        console_status("SDCard: set path to '%s'", mFolderPath);
        return true;
    }
    return false;
}

bool sdcard_reinit() { return true; }

bool sdcard_deinit() { return true; }

bool sdcard_detected() { return true; } // TODO implement maybe with visual representation

bool sdcard_status() {
    console_println("folder path: %s", mFolderPath);
    return true;
}

bool sdcard_mount() {
    mMounted = true;
    return true;
}

bool sdcard_unmount() {
    if (mFile != nullptr) {
        fclose(mFile);
        mFile = nullptr;
    }
    mMounted = false;
    return true;
}

bool sdcard_format(uint8_t format) {
    sdcard_mount();
    std::vector<std::string> files;
    std::vector<std::string> directories;
    sdcard_list("/", files, directories, true);
    for (std::string file: files) {
        std::string s = std::string{mFolderPath} + "/" + file;
        remove(s.c_str());
        console_println("rm %s", file.c_str());
    }
    for (std::string directory: directories) {
        std::string s = std::string{mFolderPath} + "/" + directory;
        if (strcmp(directory.c_str(), ".") == 0 || strcmp(directory.c_str(), "..") == 0) {
            continue;
        }
        remove(s.c_str());
        console_println("rm %s", directory.c_str());
    }
    return true;
}

bool sdcard_list(std::string path, std::vector<std::string>& files, std::vector<std::string>& directories, bool show_hidden_files) {
    CHECK_MOUNTED(mMounted);
    if (mFolderPath != nullptr) {
        tinydir_dir dir;
        std::string mFullPath = std::string{mFolderPath} + "/" + path;
        tinydir_open_sorted(&dir, mFullPath.c_str());
        for (int i = 0; i < dir.n_files; i++) {
            tinydir_file file;
            tinydir_readfile_n(&dir, &file, i);
            if (file.is_dir) {
                if ((!is_prefixed(".", file.name) || show_hidden_files)) {
                    directories.push_back(file.name);
                    // files.push_back(std::string{dir.path} + "/" + file.name);
                }
            } else {
                if ((!is_prefixed(".", file.name) || show_hidden_files)) {
                    files.push_back(file.name);
                }
            }
        }
        tinydir_close(&dir);
    }
    return true;
}

bool sdcard_file_open(std::string filepath, uint8_t flags) {
    CHECK_MOUNTED(mMounted);
    const std::string mFilePath = get_full_path(filepath);
    switch (flags) {
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
        console_error("could not open file: %s", mFilePath.c_str());
        return false;
    }
    return true;
}

uint32_t sdcard_file_write(uint8_t* bytes, uint32_t bytes_to_write) {
    CHECK_MOUNTED(mMounted);
    CHECK_FILE(mFile);
    return fwrite(bytes, 1, bytes_to_write, mFile);
}

uint32_t sdcard_file_read(uint8_t* bytes, uint32_t bytes_to_read) {
    CHECK_MOUNTED(mMounted);
    CHECK_FILE(mFile);
    return fread(bytes, 1, bytes_to_read, mFile);
}

bool sdcard_file_close() {
    CHECK_MOUNTED(mMounted);
    CHECK_FILE(mFile);
    fclose(mFile);
    mFile = nullptr;
    return true;
}

bool sdcard_create_file(const std::string pFileName) {
    CHECK_MOUNTED(mMounted);
    const std::string mFilePath = get_full_path(pFileName);
    mFile                       = fopen(mFilePath.c_str(), "wb");

    if (mFile == nullptr) {
        console_error("could not open file: %s", mFilePath.c_str());
        return false;
    }
    return true;
}

bool sdcard_file_seek(uint32_t position) {
    CHECK_MOUNTED(mMounted);
    CHECK_FILE(mFile);
    if (fseek(mFile, position, SEEK_SET)) {
        console_error("could not seek to position: %d", position);
        return false;
    } else {
        return true;
    }
}

bool sdcard_file_eof() {
    CHECK_MOUNTED(mMounted);
    CHECK_FILE(mFile);
    return feof(mFile);
}


#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
