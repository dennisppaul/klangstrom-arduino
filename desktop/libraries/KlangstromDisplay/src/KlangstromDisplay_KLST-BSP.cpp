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

#include "KlangstromDefines.hpp"

#if (KLST_ARCH == KLST_ARCH_MCU)

#include <stdbool.h>

#include "Arduino.h"
#include "KlangstromDisplay_KLST-BSP.h"

#if defined(KLST_BOARD_KLST_TINY)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(KLST_BOARD_KLST_CORE)
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#elif defined(KLST_BOARD_KLST_SHEEP)
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#else
#warning @KlangstromDisplay_KLST-BSP architecture not supported
#endif

#define ILI9341_USE_OLD_DRIVER  // also commented out DMA in `main.c`

#ifdef ILI9341_USE_OLD_DRIVER
// @TODO(move HAL reference to `KlangstromDisplay_KLST_BSP_SHEEP`)
// @TODO(optimze by first collecting data and then sending it as a chunk …
// looking at the overhead of the function this could improve the speed quite dramatically …
// see https://github.com/martnak/STM32-ILI9341/blob/master/Src/ILI9341/ILI9341_STM32_Driver.c)
// @todo(move static methods to class + get rid of ILI9341_WIDTH and ILI9341_HEIGHT)

// klangstrom::KlangstromDisplay_KLST_BSP* DisplayBSPPtr = new klangstrom::KlangstromDisplay_KLST_BSP();
// klangstrom::KlangstromDisplay*         DisplayPtr    = DisplayBSPPtr;
// klangstrom::KlangstromDisplay_KLST_BSP* DisplayBSPPtr = new klangstrom::KlangstromDisplay_KLST_BSP();
// klangstrom::KlangstromDisplay*         DisplayPtr    = DisplayBSPPtr;

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/*** Redefine if necessary ***/
#define ILI9341_SPI_PORT hspi3
extern SPI_HandleTypeDef ILI9341_SPI_PORT;

#define ILI9341_RES_Pin       GPIO_PIN_1
#define ILI9341_RES_GPIO_Port GPIOD
#define ILI9341_CS_Pin        GPIO_PIN_15
#define ILI9341_CS_GPIO_Port  GPIOA
#define ILI9341_DC_Pin        GPIO_PIN_0
#define ILI9341_DC_GPIO_Port  GPIOD

#define BURST_MAX_SIZE  512
#define SPI_MIN_TIMEOUT 100
unsigned char burst_buffer[BURST_MAX_SIZE];

// default orientation
#define ILI9341_WIDTH    240
#define ILI9341_HEIGHT   320
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)

// rotate right
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// rotate left
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// upside down
/*
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)
*/

/****************************/

static void ILI9341_Select() {
    // @TODO(move HAL reference to `KlangstromDisplay_KLST_BSP_SHEEP`)
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}

void ILI9341_Unselect() {
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET);
}

static void ILI9341_Reset() {
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
}

static void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ILI9341_SPI_PORT, &cmd, sizeof(cmd), SPI_MIN_TIMEOUT);
}

static void ILI9341_WriteData(uint8_t* buff, size_t buff_size) {
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while (buff_size > 0) {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&ILI9341_SPI_PORT, buff, chunk_size, SPI_MIN_TIMEOUT);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
}

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A);  // CASET
    {
        uint8_t data[] = {
            (uint8_t)((x0 >> 8) & 0xFF),
            (uint8_t)(x0 & 0xFF),
            (uint8_t)((x1 >> 8) & 0xFF),
            (uint8_t)(x1 & 0xFF)};
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B);  // RASET
    {
        uint8_t data[] = {
            (uint8_t)((y0 >> 8) & 0xFF),
            (uint8_t)(y0 & 0xFF),
            (uint8_t)((y1 >> 8) & 0xFF),
            (uint8_t)(y1 & 0xFF)};
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C);  // RAMWR
}

static void ILI9341_Draw_Colour_Burst(uint16_t Colour, uint32_t Size) {
    // SENDS COLOUR
    uint32_t Buffer_Size = 0;
    if ((Size * 2) < BURST_MAX_SIZE) {
        Buffer_Size = Size;
    } else {
        Buffer_Size = BURST_MAX_SIZE;
    }

    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
    ILI9341_Select();

    unsigned char chifted = Colour >> 8;
    for (uint32_t j = 0; j < Buffer_Size; j += 2) {
        burst_buffer[j]     = chifted;
        burst_buffer[j + 1] = Colour;
    }

    uint32_t Sending_Size         = Size * 2;
    uint32_t Sending_in_Block     = Sending_Size / Buffer_Size;
    uint32_t Remainder_from_block = Sending_Size % Buffer_Size;

    if (Sending_in_Block != 0) {
        for (uint32_t j = 0; j < (Sending_in_Block); j++) {
            HAL_SPI_Transmit(&ILI9341_SPI_PORT, (unsigned char*)burst_buffer, Buffer_Size, SPI_MIN_TIMEOUT);
        }
    }

    // REMAINDER!
    HAL_SPI_Transmit(&ILI9341_SPI_PORT, (unsigned char*)burst_buffer, Remainder_from_block, SPI_MIN_TIMEOUT);
    ILI9341_Unselect();
}

static void ILI9341_Init() {
    ILI9341_Select();
    ILI9341_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(100);

    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint8_t data[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint8_t data[] = {0x00, 0xC1, 0x30};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint8_t data[] = {0x85, 0x00, 0x78};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0xEA);
    {
        uint8_t data[] = {0x00, 0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint8_t data[] = {0x64, 0x03, 0x12, 0x81};
        ILI9341_WriteData(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint8_t data[] = {0x20};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint8_t data[] = {0x23};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint8_t data[] = {0x10};
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint8_t data[] = {0x3E, 0x28};
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint8_t data[] = {0x86};
        ILI9341_WriteData(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = {0x48};
        ILI9341_WriteData(data, sizeof(data));
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint8_t data[] = {0x55};
        ILI9341_WriteData(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint8_t data[] = {0x00, 0x18};
        ILI9341_WriteData(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint8_t data[] = {0x08, 0x82, 0x27};
        ILI9341_WriteData(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint8_t data[] = {0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint8_t data[] = {0x01};
        ILI9341_WriteData(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint8_t data[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                          0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
        ILI9341_WriteData(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint8_t data[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                          0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
        ILI9341_WriteData(data, sizeof(data));
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = {ILI9341_ROTATION};
        ILI9341_WriteData(data, sizeof(data));
    }

    ILI9341_Unselect();
}

static void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;

    ILI9341_Select();

    ILI9341_SetAddressWindow(x, y, x + 1, y + 1);
    uint8_t data[] = {
        (uint8_t)(color >> 8),
        (uint8_t)(color & 0xFF)};
    ILI9341_WriteData(data, sizeof(data));

    ILI9341_Unselect();
}

static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, KlangstromDisplayFont font, uint16_t color, uint16_t bgcolor) {
    if (ch < 32 || ch > 126) {
        return;
    }
    uint32_t i, b, j;
    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);
    for (i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; j++) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {
                    (uint8_t)(color >> 8),
                    (uint8_t)(color & 0xFF)};
                ILI9341_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = {
                    (uint8_t)(bgcolor >> 8),
                    (uint8_t)(bgcolor & 0xFF)};
                ILI9341_WriteData(data, sizeof(data));
            }
        }
    }
    ILI9341_Unselect();
}

static void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;
    if ((x + w - 1) >= ILI9341_WIDTH) {
        w = ILI9341_WIDTH - x;
    }
    if ((y + h - 1) >= ILI9341_HEIGHT) {
        h = ILI9341_HEIGHT - y;
    }
    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ILI9341_Draw_Colour_Burst(color, h * w);
    ILI9341_Unselect();

    // // clipping
    // if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) {
    //     return;
    // }
    // if ((x + w - 1) >= ILI9341_WIDTH) {
    //     w = ILI9341_WIDTH - x;
    // }
    // if ((y + h - 1) >= ILI9341_HEIGHT) {
    //     h = ILI9341_HEIGHT - y;
    // }

    // ILI9341_Select();
    // ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    // // @TODO(remove HAL reference)
    // uint8_t data[] = {
    //     (uint8_t)(color >> 8),
    //     (uint8_t)(color & 0xFF)};
    // HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);
    // for (y = h; y > 0; y--) {
    //     for (x = w; x > 0; x--) {
    // // @TODO(optimze by first collecting data and then sending it as a chunk …
    // // looking at the overhead of the function this could improve the speed quite dramatically …
    // // see https://github.com/martnak/STM32-ILI9341/blob/master/Src/ILI9341/ILI9341_STM32_Driver.c)
    // HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), SPI_MIN_TIMEOUT);
    //     }
    // }
    //
    // ILI9341_Unselect();
}

static void ILI9341_FillScreen(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

static void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT))
        return;
    if ((x + w - 1) >= ILI9341_WIDTH)
        return;
    if ((y + h - 1) >= ILI9341_HEIGHT)
        return;

    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ILI9341_WriteData((uint8_t*)data, sizeof(uint16_t) * w * h);
    ILI9341_Unselect();
}

// static void ILI9341_InvertColors(bool invert) {
//     ILI9341_Select();
//     ILI9341_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
//     ILI9341_Unselect();
// }

/* --- BSP --- */

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_init() {
    ILI9341_Init();
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_set_pixel(const uint16_t x, const uint16_t y, const uint16_t color) {
    ILI9341_DrawPixel(x, y, color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_clear_background() {
    const uint16_t color = convert_RGB_888i_565i(mColorBackground.r, mColorBackground.g, mColorBackground.b);
    ILI9341_FillScreen(color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line_vertical(uint16_t x0, uint16_t y0, uint16_t h, uint16_t color) {
    // BSP_line(x0, y0, x0, y0 + h, color);

    if ((x0 >= width()) || (y0 >= height())) {
        return;
    }
    if ((y0 + h - 1) >= height()) {
        h = height() - y0;
    }

    ILI9341_SetAddressWindow(x0, y0, x0, y0 + h);
    ILI9341_Draw_Colour_Burst(color, h);

    // ILI9341_Select();
    // uint8_t       data[h * 2];
    // const uint8_t data_high = (uint8_t)(color >> 8);
    // const uint8_t data_low  = (uint8_t)(color & 0xFF);
    // for (uint16_t i = 0; i < h * 2; i += 2) {
    //     data[i + 0] = data_high;
    //     data[i + 1] = data_low;
    // }
    // ILI9341_WriteData(data, h * 2);
    // ILI9341_Unselect();
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line_horizontal(uint16_t x0, uint16_t y0, uint16_t w, uint16_t color) {
    // BSP_line(x0, y0, x0 + w, y0, color);

    if ((x0 >= width()) || (y0 >= height())) {
        return;
    }
    if ((x0 + w - 1) >= width()) {
        w = width() - x0;
    }

    ILI9341_SetAddressWindow(x0, y0, x0 + w, y0);
    ILI9341_Draw_Colour_Burst(color, w);

    // ILI9341_Select();
    // uint8_t       data[w * 2];
    // const uint8_t data_high = (uint8_t)(color >> 8);
    // const uint8_t data_low  = (uint8_t)(color & 0xFF);
    // for (uint16_t i = 0; i < w * 2; i += 2) {
    //     data[i + 0] = data_high;
    //     data[i + 1] = data_low;
    // }
    // ILI9341_WriteData(data, w * 2);
    // ILI9341_Unselect();
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    /**
     * @brief "Write a line.  Bresenham's algorithm - thx wikpedia" thx adafruit
     *
     */
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_uint16_t(x0, y0);
        _swap_uint16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_uint16_t(x0, x1);
        _swap_uint16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            BSP_set_pixel(y0, x0, color);
        } else {
            BSP_set_pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_rect_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ILI9341_FillRectangle(x, y, w, h, color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    BSP_line_horizontal(x, y, w - 1, color);
    BSP_line_horizontal(x, y + h - 1, w, color);
    BSP_line_vertical(x, y, h, color);
    BSP_line_vertical(x + w - 1, y, h - 1, color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_character(uint16_t x, uint16_t y, char ch, const uint16_t foreground_color, const uint16_t background_color) {
    ILI9341_WriteChar(x, y, ch, *font, foreground_color, background_color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_character_scaled(uint16_t x, uint16_t y, uint8_t scale, char ch, const uint16_t foreground_color, const uint16_t background_color) {
    if (scale == 1) {
        BSP_character(x, y, ch, foreground_color, background_color);
        return;
    }
    if (ch < 32 || ch > 126) {
        return;
    }
    if (font != nullptr) {
        uint32_t i, b, j;
        for (i = 0; i < font->height; i++) {
            b = font->data[(ch - 32) * font->height + i];
            for (j = 0; j < font->width; j++) {
                uint16_t mColor = ((b << j) & 0x8000) ? foreground_color : background_color;
                BSP_rect_fill(x + j * scale, y + i * scale, scale, scale, mColor);
            }
        }
    }
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    ILI9341_DrawImage(x, y, w, h, data);
}

#else  // ILI9341_USE_OLD_DRIVER

/* --- BSP --- */

#include "ili9341.h"
#include "ili9341_font.h"
#include "ili9341_gfx.h"

extern SPI_HandleTypeDef hspi3;
#define ILI9341_RES_Pin       GPIO_PIN_1
#define ILI9341_RES_GPIO_Port GPIOD
#define ILI9341_CS_Pin        GPIO_PIN_15
#define ILI9341_CS_GPIO_Port  GPIOA
#define ILI9341_DC_Pin        GPIO_PIN_0
#define ILI9341_DC_GPIO_Port  GPIOD

ili9341_t* _lcd;

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_init() {
    _lcd = ili9341_new(
        &hspi3,
        ILI9341_RES_GPIO_Port, ILI9341_RES_Pin,
        ILI9341_CS_GPIO_Port, ILI9341_CS_Pin,
        ILI9341_DC_GPIO_Port, ILI9341_DC_Pin,
        isoPortrait,
        NULL, 0,
        NULL, 0,
        itsNotSupported,
        itnNONE);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_set_pixel(const uint16_t x, const uint16_t y, const uint16_t color) {
    ili9341_draw_pixel(_lcd, color, x, y);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_clear_background() {
    const uint16_t color = convert_RGB_888i_565i(mColorBackground.r, mColorBackground.g, mColorBackground.b);
    ili9341_fill_screen(_lcd, color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line_vertical(uint16_t x0, uint16_t y0, uint16_t h, uint16_t color) {
    ili9341_draw_line(_lcd, color, x0, y0, x0, y0 + h - 1);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line_horizontal(uint16_t x0, uint16_t y0, uint16_t w, uint16_t color) {
    ili9341_draw_line(_lcd, color, x0, y0, x0 + w, y0);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    ili9341_draw_line(_lcd, color, x0, y0, x1, y1);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_rect_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9341_draw_rect(_lcd, color, x, y, w, h);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9341_draw_rect(_lcd, color, x, y, w, h);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_character(uint16_t x, uint16_t y, char ch, const uint16_t foreground_color, const uint16_t background_color) {
    // ILI9341_WriteChar(x, y, ch, *font, foreground_color, background_color);
}

void klangstrom::KlangstromDisplay_KLST_BSP::BSP_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    // ILI9341_DrawImage(x, y, w, h, data);
}
#endif  // ILI9341_USE_OLD_DRIVER

#endif  // (KLST_ARCH==KLST_ARCH_MCU)
