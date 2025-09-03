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

#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "KlangstromEnvironment.h"

#ifdef __cplusplus
extern "C" {
#endif

static constexpr uint8_t FILE_READ_ONLY  = 0x00;
static constexpr uint8_t FILE_WRITE_ONLY = 0x01;
static constexpr uint8_t FILE_READ_WRITE = 0x02;

/* all functions are implemented as A/BSP */

bool     sdcard_init();
bool     sdcard_reinit(); // TODO i do not like this … maybe this can be done in a smarter way
bool     sdcard_deinit();
bool     sdcard_detected();
bool     sdcard_status();
bool     sdcard_mount();
bool     sdcard_unmount();
bool     sdcard_format(uint8_t format = 0);
bool     sdcard_list(std::string path, std::vector<std::string>& files, std::vector<std::string>& directories, bool show_hidden_files = false);
bool     sdcard_file_open(std::string filepath, uint8_t flags);
uint32_t sdcard_file_write(uint8_t* bytes, uint32_t bytes_to_write);
uint32_t sdcard_file_read(uint8_t* bytes, uint32_t bytes_to_read);
bool     sdcard_file_close();
bool     sdcard_file_create(const std::string filename);
bool     sdcard_file_seek(uint32_t position);
bool     sdcard_file_eof();
bool     sdcard_file_attributes(const std::string filename);

#ifdef __cplusplus
}
#endif