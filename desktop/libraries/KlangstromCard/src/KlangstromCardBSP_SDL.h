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

#ifndef klangstrom_card_bsp_sdl_h
#define klangstrom_card_bsp_sdl_h

#include "KlangstromCard.h"

namespace klangstrom {
    class KlangstromCardBSP_SDL : public KlangstromCard {
    public:
        bool begin();
        bool begin(const char *pFolderPath);
        void get_file_list(vector<String> &pFiles, bool pIncludeHiddenFiles = false);
        int  open(const String pFileName, const uint8_t pReadWriteFlag = FILE_READ_ONLY);
        bool is_open();
        void close();
        void print_WAV_header(WaveHeader_t *mHeader);

    protected:
        int  BSP_read_block(uint8_t *pReadBuffer, uint32_t pReadBufferSize);
        void debug_print_error(const char *pString);

    private:
        const char *mFolderPath = nullptr;
        FILE *mFile;

        bool KlangstromCard_is_prefixed(const char *pre, const char *str) {
            return strncmp(pre, str, strlen(pre)) == 0;
        }

        String KlangstromCard_get_full_path(String pFile) {
            return String(mFolderPath + pFile);
        }
    };
}  // namespace klangstrom

#endif  // klangstrom_card_bsp_sdl_h