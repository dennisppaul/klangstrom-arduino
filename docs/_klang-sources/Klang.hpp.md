---
layout: libdoc
title: Klang.hpp
permalink: /Klang.hpp/
index: 30
---

```c
/*
 * Klang – a node+text-based synthesizer library
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

#ifndef Klang_hpp
#define Klang_hpp

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* --- USER DEFINES --- */

#if __has_include("KlangConfiguration.h")
#include "KlangConfiguration.h"
#endif

/* --- DEBUG --- */

//#define DEBUG_SYNTHESIZER
//#define DEBUG_AUDIO_POOL
//#define DEBUG_CONNECTION_POOL
//#define DEBUG_SYNTHESIZER
//#define DEBUG_DAC
//#define DEBUG_OSC
//#define DEBUG_PRINT_EVENTS
//#define DEBUG_SHOW_DEFAULT_WARNINGS

/* --- DEFAULTS --- */

#ifndef KLANG_DEBUG_LEVEL
#define KLANG_DEBUG_LEVEL 0
#endif

#ifndef KLANG_DEFINES
#include "KlangDefines.hpp"
#endif

#ifndef KLANG_AUDIO_RATE
#define KLANG_AUDIO_RATE 48000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_RATE to default value: 48000
#endif
#endif

#ifndef KLANG_AUDIO_BLOCKS
#define KLANG_AUDIO_BLOCKS 32
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_BLOCKS to default value: 32
#endif
#endif

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 512
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SAMPLES_PER_AUDIO_BLOCK to default value: 512
#endif
#endif

#if !defined(float_FLOAT) && !defined(float_INT16)
#warning float is not defined properly. choosing `float_FLOAT`. check if `KlangDefines.hpp` is included in `KlangConfiguration.h`?
#endif

#ifndef KLANG_HEAP_ALLOCATION
#define KLANG_HEAP_ALLOCATION 0
#endif
#if KLANG_HEAP_ALLOCATION > 0
#define KLANG_USE_HEAP_ALLOCATION
#endif

/* --- SIGNAL --- */

const static float SIGNAL_MIN = -1.0;
const static float SIGNAL_MAX = 1.0;

#define KLANG_FILL_AUDIO_BUFFER(DST, VAL) memset(DST, VAL, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(float))
#define KLANG_COPY_AUDIO_BUFFER(DST, SRC) memcpy(DST, SRC, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(float))

/* --- CONSTANTS --- */

#define DEFAULT_FREQUENCY (KLANG_AUDIO_RATE / 256.0 * 0.5)
static const CONNECTION_ID KLANG_CONNECTION_ERROR  = -1;
static const CONNECTION_ID KLANG_CREATE_NODE_ERROR = -1;
static const uint16_t      KLANG_AUDIO_RATE_UINT16 = KLANG_AUDIO_RATE;

namespace klang {
    class Node;
    class Connection;

    class Klang {
    public:
        static Klang &instance() {
            static Klang instance;
            return instance;
        }

        static CONNECTION_ID connect(Node &pOutputNode, Node &pInputNode);
        static CONNECTION_ID connect(Node &pOutputNode, CHANNEL_ID pOutputChannel, Node &pInputNode, CHANNEL_ID pInputChannel);
        static bool          disconnect(Node &pOutputNode, CHANNEL_ID pOutputChannel, Node &pInputNode, CHANNEL_ID pInputChannel);
        static bool          disconnect(CONNECTION_ID pConnectionID);

        FRAME_TYPE frame_index();
        void       reset_frame_index();

        static void lock() { instance().mLock = true; }
        static void unlock() { instance().mLock = false; }
        static bool islocked() { return instance().mLock; }

        bool frame_begin();
        void frame_end();

    private:
        Klang(Klang const &);
        void operator=(Klang const &);
        Klang() {}

        FRAME_TYPE    mFrameCounter = 0;
        volatile bool mLock         = false;
#ifdef DEBUG_SYNTHESIZER
        bool mFirstFrame = true;
#endif
    };
}  // namespace klang

#endif /* Klang_hpp */

```
