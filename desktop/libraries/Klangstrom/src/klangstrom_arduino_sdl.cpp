/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#if (KLST_ARCH == KLST_ARCH_DESKTOP)

#define KLST_SDL_USE_AUDIO_INPUT
#define KLST_SDL_INITIALIZE_BEAT
#define KLST_SDL_DEFAULT_BPM 120

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <vector>

#include "Arduino.h"
#include "SDL.h"
#include "SDL_audio.h"
#include "SDL_render.h"

#ifdef KLST_SDL_USE_OSC
#include "ip/UdpSocket.h"
#include "osc/OscOutboundPacketStream.h"
#include "osc/OscPacketListener.h"
#include "osc/OscReceivedElements.h"
#endif

/* KLST */

#include "KLST_SDL-adapter.h"
#include "KlangstromApplicationArduino.h"
#include "klangstrom_arduino_debug.h"
#include "klangstrom_arduino_proxy_encoder.h"
#include "klangstrom_arduino_proxy_serial.h"
#include "klangstrom_arduino_sdl.h"

/* --- USBHost adapter --- */

#define EMULATE_USB_HOST

#ifdef EMULATE_USB_HOST
#include "USBHostCallbacks.h"
#endif  // EMULATE_USB_HOST

/* KLST -------------------------------------------------------------------------------- */

extern KLST_Emulator mEmulator;

#define Q(x)       #x
#define QUOTE(x)   Q(x)
#define APP_HEADER QUOTE(KLANG_APP_CLASS.hpp)

#if defined(KLANG_TARGET_MCU)
#define KLANG_NL "\n\r"
#else
#define KLANG_NL "\n"
#endif

/* KLST -------------------------------------------------------------------------------- */

float clamp(float pValue) {
    return pValue > 1.0 ? 1.0 : (pValue < -1.0 ? -1.0 : pValue);
}

float FLOAT_32(uint8_t *pBytes, uint32_t pOffset) {
    float output;

    *((uint8_t *)(&output) + 0) = pBytes[0 + pOffset];
    *((uint8_t *)(&output) + 1) = pBytes[1 + pOffset];
    *((uint8_t *)(&output) + 2) = pBytes[2 + pOffset];
    *((uint8_t *)(&output) + 3) = pBytes[3 + pOffset];

    return output;
}

void delay(uint32_t pMS) {
    this_thread::sleep_for(chrono::milliseconds(pMS));
}

/* KLST -------------------------------------------------------------------------------- */

using namespace klangstrom;
using namespace std;

#ifdef KLST_SDL_USE_OSC
thread             mOSCThread;
UdpTransmitSocket *mTransmitSocket = nullptr;
#endif

thread mLoopThread;

int                           mBufferOffset                          = 0;
uint32_t                      mBeatCounter                           = 0;
SDL_TimerID                   mTimerID                               = 0;
bool                          mTimerDisabled                         = false;
SDL_Window                   *mWindow                                = nullptr;
SDL_Renderer                 *gRenderer                              = nullptr;
bool                          mQuitFlag                              = false;
bool                          mCapsLockDown                          = false;
bool                          fMouseButtonPressed[MOUSE_NUM_BUTTONS] = {false};
int8_t                        mCurrentEncoder                        = NUMBER_OF_ENCODERS > 0 ? 0 : -1;
KlangstromArduinoProxyEncoder mEncoders[NUMBER_OF_ENCODERS];
static uint8_t                fKeyBuffer[KEYBOARD_NUM_KEYS]     = {0};
static uint8_t                fKeyCodeBuffer[KEYBOARD_NUM_KEYS] = {0};
static EventKeyboard          fKeyboardEvent                    = {.keys     = fKeyBuffer,
                                                                   .keycodes = fKeyCodeBuffer};
int                           fMousePreviousX                   = 0;
int                           fMousePreviousY                   = 0;

const static int8_t AUDIO_DEFAULT_DEVICE         = -1;
int8_t              mAudioInputDeviceID          = AUDIO_DEFAULT_DEVICE;
int8_t              mAudioOutputDeviceID         = AUDIO_DEFAULT_DEVICE;
SDL_AudioDeviceID   mAudioInputDeviceInternalID  = 0;
SDL_AudioDeviceID   mAudioOutputDeviceInternalID = 0;

const static uint8_t NUM_INPUT_SWAP_BUFFERS = 2;
uint8_t              mCurrentInputBufferID  = 0;
float                fOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
float                fInputBuffers[NUM_INPUT_SWAP_BUFFERS][KLANG_INPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
float                fInternalAudioOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK * NUM_INPUT_SWAP_BUFFERS];

#define DEBUG_AUDIO_DEVICE
#define AUDIO_INPUT  SDL_TRUE
#define AUDIO_OUTPUT SDL_FALSE

static void init_encoders();
static void loop_encoders();

/* KlangstromDisplay ------------------------------------------------------------------- */

#ifdef KLST_SDL_HAS_DISPLAY

klangstrom::KlangstromCallback *mDisplayEventListener = nullptr;

void registerKlangstromDisplayCallback(klangstrom::KlangstromCallback *pDisplayEventListener) {
    mDisplayEventListener = pDisplayEventListener;
}

void draw_klangstrom_display() {
    if (mDisplayEventListener != nullptr) {
        mDisplayEventListener->callback();
    }
}

uint16_t getSDLRendererWidth() {
    return SCREEN_WIDTH;
}

uint16_t getSDLRendererHeight() {
    return SCREEN_HEIGHT;
}

SDL_Renderer *getSDLRenderer() {
    return gRenderer;
}

#endif

/* KLST -------------------------------------------------------------------------------- */

void init_main() {
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            fOutputBuffers[j][i]                                              = 0;
            fInternalAudioOutputBuffers[j][i]                                 = 0;
            fInternalAudioOutputBuffers[j][i + KLANG_SAMPLES_PER_AUDIO_BLOCK] = 0;
        }
    }

    init_SDL();
    init_renderer();
    init_app();
    init_encoders();

    const char *mAudioDriverName = SDL_GetCurrentAudioDriver();
    if (mAudioDriverName) {
#ifdef DEBUG_AUDIO_DEVICE
        KLANG_LOG("+++ (INFO) audio subsystem initialized with driver '%s'", mAudioDriverName);
#endif
    } else {
        KLANG_LOG_ERR("+++ (ERROR) audio subsystem not initialized.");
    }
    /*
     * @REF(https://wiki.libsdl.org/SDL_AudioInit?highlight=%28%5CbCategoryAudio%5Cb%29%7C%28CategoryEnum%29%7C%28CategoryStruct%29)
     *
     * > This function is used internally, and should not be used unless you have a specific need to designate the audio
     * > driver you want to use. You should normally use SDL_Init() or SDL_InitSubSystem().
     *
     */
//    if (SDL_AudioInit(mAudioDriverName)) {
//        printf("+++ (ERROR) Audio driver failed to initialize: %s\n", mAudioDriverName);
//    }
#ifdef DEBUG_AUDIO_DEVICE
    list_audio_devices();
#endif
    init_audio_output();
#ifdef KLST_SDL_USE_AUDIO_INPUT
    init_audio_input();
    SDL_PauseAudioDevice(mAudioInputDeviceInternalID, SDL_FALSE);
#endif
    SDL_PauseAudioDevice(mAudioOutputDeviceInternalID, SDL_FALSE);
}

void loop_thread() {
    while (!mQuitFlag) {
        loop();
    }
}

void pre_setup() {
    register_audioblock(audioblock);
}

void post_setup() {
    /* @note(if beat has not been set in `setup` start with default BPM */
    if (mTimerID == 0 && !mTimerDisabled) {
#ifdef KLST_SDL_INITIALIZE_BEAT
        klangstrom_arduino_beats_per_minute(KLST_SDL_DEFAULT_BPM);
#endif
    }
}

void loop_main() {
    configure();
    pre_setup();
    setup();
    post_setup();
    mLoopThread = thread(loop_thread);
    while (!mQuitFlag) {
        loop_event();
        loop_renderer();
        // @note(moved `loop()` to own thread)
    }
    mLoopThread.detach();
}

bool get_encoder_button_state(uint8_t encoder) {
    if (encoder >= NUMBER_OF_ENCODERS) {
        return false;
    }
    return mEncoders[encoder].button_pressed;
}

static void handle_key_pressed_capslock(SDL_Event &e) {
    const SDL_Scancode  mScanCode              = e.key.keysym.scancode;
    const static int8_t ENCODER_ROTATION_SPEED = 1;
    switch (mScanCode) {
        case SDL_SCANCODE_UP:
            mEncoders[mCurrentEncoder].rotate(ENCODER_ROTATION_SPEED);
            break;
        case SDL_SCANCODE_DOWN:
            mEncoders[mCurrentEncoder].rotate(-ENCODER_ROTATION_SPEED);
            break;
        case SDL_SCANCODE_SPACE:
            if (e.key.repeat == 0) {
                mEncoders[mCurrentEncoder].button_pressed = true;
            }
            break;
        case SDL_SCANCODE_RIGHT:
            mCurrentEncoder++;
            mCurrentEncoder %= NUMBER_OF_ENCODERS;
            break;
        case SDL_SCANCODE_LEFT:
            mCurrentEncoder += NUMBER_OF_ENCODERS - 1;
            mCurrentEncoder %= NUMBER_OF_ENCODERS;
            break;
        default:
            break;
    }
}

static void handle_key_released_capslock(SDL_Event &e) {
    const SDL_Scancode mScanCode = e.key.keysym.scancode;
    switch (mScanCode) {
        case SDL_SCANCODE_SPACE:
            if (e.key.repeat == 0) {
                mEncoders[mCurrentEncoder].button_pressed = false;
            }
            break;
        default:
            break;
    }
}

static bool isKeyValid(SDL_Event &e) {
    const SDL_Keycode mKey = e.key.keysym.sym;
    return mKey > 0 && mKey <= SDLK_DELETE;
    // return mKey >= SDLK_SPACE && mKey <= SDLK_DELETE;
}

int8_t findKeyCode(uint8_t pKeyCode) {
    for (uint8_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
        if (fKeyCodeBuffer[i] == pKeyCode) {
            return i;
        }
    }
    return -1;
}

int8_t findFreeKeyCode() {
    for (uint8_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
        if (fKeyCodeBuffer[i] == 0) {
            return i;
        }
    }
    return -1;
}

static void handle_keyboard_events(SDL_Event &e) {
    if (!e.key.repeat) {
        const bool         mKeyPressed = (e.type == SDL_KEYDOWN);
        const uint8_t      mKey        = isKeyValid(e) ? e.key.keysym.sym : 0;
        const SDL_Scancode mKeyCode    = e.key.keysym.scancode;
        const uint8_t      mKeyCode8U  = (mKeyCode < 0 || mKeyCode > 255) ? 0 : (uint8_t)mKeyCode;
        const int8_t       mKeyCodeID  = findKeyCode(mKeyCode8U);
        if (mKeyCodeID >= 0) {
            if (!mKeyPressed) {
                fKeyCodeBuffer[mKeyCodeID] = 0;
                fKeyBuffer[mKeyCodeID]     = 0;
            }
        } else {
            if (mKeyPressed) {
                const int8_t mFreeKeyCodeID = findFreeKeyCode();
                if (mFreeKeyCodeID >= 0) {
                    fKeyCodeBuffer[mFreeKeyCodeID] = mKeyCode8U;
                    fKeyBuffer[mFreeKeyCodeID]     = mKey;
                }
            }
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
            fKeyboardEvent.SHIFT_LEFT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
            fKeyboardEvent.SHIFT_RIGHT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_LGUI) {
            fKeyboardEvent.GUI_LEFT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_RGUI) {
            fKeyboardEvent.GUI_RIGHT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_LALT) {
            fKeyboardEvent.ALT_LEFT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_RALT) {
            fKeyboardEvent.ALT_RIGHT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_LCTRL) {
            fKeyboardEvent.CTRL_LEFT = mKeyPressed;
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_RCTRL) {
            fKeyboardEvent.CTRL_RIGHT = mKeyPressed;
        }
#ifdef EMULATE_USB_HOST
        usb_host_call_event_receive(EVENT_KEYBOARD, &fKeyboardEvent);
#else
        event_receive(EVENT_KEYBOARD, &fKeyboardEvent);
#endif
    }
}

static void handle_key_pressed(SDL_Event &e) {
    if (!e.key.repeat) {
        const SDL_Scancode mKeyCode = e.key.keysym.scancode;
        if (mKeyCode == SDL_SCANCODE_CAPSLOCK) {
            mCapsLockDown = !mCapsLockDown;
        } else {
            if (mCapsLockDown) {
                handle_key_pressed_capslock(e);
            } else {
#ifdef EMULATE_USB_HOST
                if (isKeyValid(e)) {
                    const SDL_Keycode mKey = e.key.keysym.sym;
                    usb_host_call_key_pressed(mKey);
                }
#endif
#ifdef DEBUG_PRINT_EVENTS
                KLANG_LOG("key pressed: %s", SDL_GetScancodeName(mKeyCode));
#endif
            }
        }
    }
}

static void handle_key_released(SDL_Event &e) {
    if (e.key.keysym.scancode == SDL_SCANCODE_CAPSLOCK) {
    } else {
        if (mCapsLockDown) {
            handle_key_released_capslock(e);
        } else {
#ifdef EMULATE_USB_HOST
            if (isKeyValid(e)) {
                const SDL_Keycode mKey = e.key.keysym.sym;
                usb_host_call_key_released(mKey);
            }
#endif
#ifdef DEBUG_PRINT_EVENTS
            const char mKey = e.key.keysym.scancode;
            KLANG_LOG("key released: %c", SDL_GetScancodeName((SDL_Scancode)mKey)[0]);
#endif
        }
    }
}

static void init_encoders() {
    for (int8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        mEncoders[i].ID = i;
    }
}

static void loop_encoders() {
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        mEncoders[i].update(i == mCurrentEncoder);
    }
}

void handle_mouse_event_receive(int x, int y) {
    EventMouse e;
    e.x      = x;
    e.y      = y;
    e.LEFT   = fMouseButtonPressed[MOUSE_BUTTON_LEFT];
    e.RIGHT  = fMouseButtonPressed[MOUSE_BUTTON_RIGHT];
    e.MIDDLE = fMouseButtonPressed[MOUSE_BUTTON_MIDDLE];
#ifdef EMULATE_USB_HOST
    usb_host_call_event_receive(EVENT_MOUSE, &e);
#else
    event_receive(EVENT_MOUSE, &e);
#endif
}

static uint8_t getKLSTMouseButton(const bool *pPrevMouseButtonStates) {
    for (uint8_t i = 0; i < MOUSE_NUM_BUTTONS; i++) {
        if (fMouseButtonPressed[i] != pPrevMouseButtonStates[i]) {
            return i;
        }
    }
    return MOUSE_BUTTON_NONE;
}

void loop_event() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            mQuitFlag = true;
        }
        if (e.type == SDL_QUIT) {
            mQuitFlag = true;
        }
        /* --- MOUSE --- */
        if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEMOTION) {
            int          x_SDL, y_SDL;
            const Uint32 mButtonState = SDL_GetMouseState(&x_SDL, &y_SDL);
            int          x            = x_SDL - fMousePreviousX;
            int          y            = y_SDL - fMousePreviousY;
            fMousePreviousX           = x_SDL;
            fMousePreviousY           = y_SDL;
            bool mPrevMouseButtonStates[MOUSE_NUM_BUTTONS];
            for (uint8_t i = 0; i < MOUSE_NUM_BUTTONS; i++) {
                mPrevMouseButtonStates[i] = fMouseButtonPressed[i];
                fMouseButtonPressed[i]    = (mButtonState & (1 << i));
            }
            const uint8_t mCurrentKLSTMouseButton = getKLSTMouseButton(mPrevMouseButtonStates);
#ifdef EMULATE_USB_HOST
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                usb_host_call_mouse_pressed(mCurrentKLSTMouseButton, x, y);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                usb_host_call_mouse_released(mCurrentKLSTMouseButton, x, y);
            }
            if (e.type == SDL_MOUSEMOTION) {
                usb_host_call_mouse_moved(fMouseButtonPressed, x, y);
            }
#endif
            handle_mouse_event_receive(x, y);
        }
        /* --- KEYBOARD --- */
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            if (e.type == SDL_KEYDOWN) {
                handle_key_pressed(e);
            } else if (e.type == SDL_KEYUP) {
                handle_key_released(e);
            }
            handle_keyboard_events(e);
        }
    }
    /* --- ENCODERS --- */
    loop_encoders();
}

static float display_scale;
static float display_x;
static float display_y;

static void line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer,
                       x1 * display_scale + display_x,
                       y1 * display_scale + display_y,
                       x2 * display_scale + display_x,
                       y2 * display_scale + display_y);
}

static const int16_t GUI_START_X       = 20;
static const int16_t GUI_START_Y       = 20;
static const int16_t GUI_UNIT_HEIGHT   = 32;
static const int16_t GUI_UNIT_WIDTH    = 16;
static const int16_t GUI_UNIT_SPACING  = 4;
static const int16_t GUI_ELEMENT_WIDTH = GUI_UNIT_WIDTH * 2 + GUI_UNIT_SPACING;

void draw_oscilloscope() {
    // TODO this is fixed to 2 channels. change function so it draws any number of channels …
    display_scale = 0.5f;
    display_x     = GUI_START_X;
    display_y     = GUI_START_Y + (GUI_UNIT_HEIGHT + GUI_UNIT_SPACING) * 2;

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    line(gRenderer, 0, SCREEN_HEIGHT / 2 - 1, SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 1);
    line(gRenderer, 0, SCREEN_HEIGHT / 2 + 1, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + 1);

    line(gRenderer, 0, 0, SCREEN_WIDTH, 0);
    line(gRenderer, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
    line(gRenderer, 0, 0, 0, SCREEN_HEIGHT);
    line(gRenderer, SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int j        = (i - 1) + mBufferOffset;
        int k        = i + mBufferOffset;
        int x0       = SCREEN_WIDTH * (float)(i - 1) / (float)KLANG_SAMPLES_PER_AUDIO_BLOCK;
        int x1       = SCREEN_WIDTH * (float)i / (float)KLANG_SAMPLES_PER_AUDIO_BLOCK;
        int y0_left  = clamp(fInternalAudioOutputBuffers[LEFT][j]) * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4;
        int y1_left  = clamp(fInternalAudioOutputBuffers[LEFT][k]) * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4;
        int y0_right = clamp(fInternalAudioOutputBuffers[RIGHT][j]) * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4 * 3;
        int y1_right = clamp(fInternalAudioOutputBuffers[RIGHT][k]) * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4 * 3;
        line(gRenderer, x0, y0_left, x1, y1_left);
        line(gRenderer, x0, y0_right, x1, y1_right);
    }
}

void draw_capslock() {
    static const int16_t mWidth  = GUI_UNIT_WIDTH / 2;
    static const int16_t mHeight = GUI_UNIT_HEIGHT;
    SDL_Rect             r;
    r.x = GUI_START_X - mWidth - GUI_UNIT_SPACING;
    r.y = GUI_START_Y + GUI_UNIT_HEIGHT + GUI_UNIT_SPACING;
    r.w = mWidth;
    r.h = mHeight;
    if (mCapsLockDown) {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(gRenderer, &r);
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0x7F, 0x7F, 0x7F, 0xFF);
        SDL_RenderDrawRect(gRenderer, &r);
    }
}

void draw_encoders() {
    static const int16_t mEncoderSpacing = GUI_ELEMENT_WIDTH + GUI_UNIT_SPACING;
    static const int16_t mHeight         = GUI_UNIT_HEIGHT;
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        KlangstromArduinoProxyEncoder &e = mEncoders[i];
        e.dimensions.x                   = GUI_START_X + i * mEncoderSpacing;
        e.dimensions.y                   = GUI_START_Y + GUI_UNIT_HEIGHT + GUI_UNIT_SPACING;
        e.dimensions.w                   = GUI_ELEMENT_WIDTH;
        e.dimensions.h                   = mHeight;
    }
    for (uint8_t i = 0; i < NUMBER_OF_ENCODERS; i++) {
        mEncoders[i].draw(gRenderer, mCapsLockDown);
    }
}

uint16_t getKlangstromDisplayPosX() {
    static const int16_t mSpacing = GUI_ELEMENT_WIDTH + GUI_UNIT_SPACING;
    return GUI_START_X + mSpacing * 13;
}

uint16_t getKlangstromDisplayPosY() {
    static const int16_t mSpacing = GUI_UNIT_HEIGHT + GUI_UNIT_SPACING;
    return GUI_START_Y + mSpacing * 2;
}

void draw_LEDs() {
    static const int16_t mWidth      = GUI_UNIT_WIDTH;
    static const int16_t mHeight     = GUI_UNIT_HEIGHT;
    static const int16_t mLEDSpacing = mWidth + GUI_UNIT_SPACING;
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
        SDL_Rect r;
        r.x = GUI_START_X + i * mLEDSpacing;
        r.y = GUI_START_Y;
        r.w = mWidth;
        r.h = mHeight;
        if (mEmulator.getLEDs()[i]) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawRect(gRenderer, &r);
    }
}

void loop_renderer() {
    SDL_SetRenderDrawColor(gRenderer, 0x20, 0x20, 0x20, 0xFF);
    SDL_RenderClear(gRenderer);

    draw_oscilloscope();
    draw_capslock();
    draw_encoders();
    draw_LEDs();
#ifdef KLST_SDL_HAS_DISPLAY
    draw_klangstrom_display();
#endif

    SDL_RenderPresent(gRenderer);
}

void list_audio_devices() {
    /* input devices */
    const int mNumberOfAvailableAudioInputDevices = SDL_GetNumAudioDevices(AUDIO_INPUT);
    if (mNumberOfAvailableAudioInputDevices < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio input device! Error: %s", SDL_GetError());
    }
    KLANG_LOG("+++ (INFO) found audio input devices: ");
    for (int i = 0; i < mNumberOfAvailableAudioInputDevices; ++i) {
        KLANG_LOG("           %i: %s", i, SDL_GetAudioDeviceName(i, AUDIO_INPUT));
    }
    /* output devices */
    const int mNumberOfAvailableAudioOutputDevices = SDL_GetNumAudioDevices(AUDIO_OUTPUT);
    if (mNumberOfAvailableAudioOutputDevices < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio output device! Error: %s", SDL_GetError());
    }
    KLANG_LOG("+++ (INFO) found audio output devices: ");
    for (int i = 0; i < mNumberOfAvailableAudioOutputDevices; ++i) {
        KLANG_LOG("           %i: %s", i, SDL_GetAudioDeviceName(i, AUDIO_OUTPUT));
    }
}

inline float sanitize_sample(float f) {
    if (isinf(f)) {
        return 0.0;
    }
    if (isnan(f)) {
        return 0.0;
    }
    if (f != f) {
        return 0.0;
    }
    if (f > 1.0) {
        return 1.0;
    }
    if (f < -1.0) {
        return -1.0;
    }
    return f;
}

void update_audiobuffer() {
    // NOTE the code below is NOT very elegant, however it seems as if this is the only way to pass a 2D array to a function.
    // the option to pass the arrays by performing a C-style cast to a double pointer ( e.g `(float **)fOutputBuffers` ) is not
    // allowed and may produce undefined behavior or a segmantation fault.
    // it might be a better idea to use dynamic memory allocation after all …
    float *mInputBuffer[KLANG_INPUT_CHANNELS];
    float *mOutputBuffer[KLANG_OUTPUT_CHANNELS];
    for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
        mInputBuffer[j] = fInputBuffers[mCurrentInputBufferID][j];
    }
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        mOutputBuffer[j] = fOutputBuffers[j];
    }

    call_audioblock(mInputBuffer, mOutputBuffer);

    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        for (size_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            fInternalAudioOutputBuffers[j][i + mBufferOffset] = sanitize_sample(fOutputBuffers[j][i]);
        }
    }
}

void audio_output_callback(void *userdata, Uint8 *pStream, int pBufferLength) {
    // @TODO this is fixed to 2 channels
    /* for `AUDIO_F32` the stride is 8 = 4 Bytes LEFT + 4 Bytes RIGHT */
#define LEFT_POS  0
#define RIGHT_POS 4
    uint16_t mBufferPos = 0;
    for (int i = 0; i < pBufferLength; i += 8) {
        auto *mLeftF  = (uint8_t *)&fInternalAudioOutputBuffers[LEFT][mBufferPos + mBufferOffset];
        auto *mRightF = (uint8_t *)&fInternalAudioOutputBuffers[RIGHT][mBufferPos + mBufferOffset];
        for (uint8_t j = 0; j < 4; j++) {
            pStream[i + LEFT_POS + j]  = mLeftF[j];
            pStream[i + RIGHT_POS + j] = mRightF[j];
        }
        mBufferPos++;
    }
    /* update audio with next block */
    if (mBufferOffset == 0) {
        mBufferOffset = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    } else {
        mBufferOffset = 0;
    }
    update_audiobuffer();
}

Uint32 beat_callback(Uint32 interval, void *param) {
    beat(mBeatCounter++);
    return interval;
}

void klangstrom_arduino_beats_per_minute(float BPM) {
    if (BPM > 0) {
        const float    mMilliSeconds = 1000.0f / (BPM / 60.0f);
        const uint32_t pMicroSeconds = (uint32_t)(mMilliSeconds * 1000);
        klangstrom_arduino_beats_per_minute_ms(pMicroSeconds);
    } else {
        klangstrom_arduino_beats_per_minute_ms(0);
    }
}

void klangstrom_arduino_beats_per_minute_ms(uint32_t micro_seconds) {
    if (mTimerID > 0) {
        bool mOK = SDL_RemoveTimer(mTimerID);
        if (!mOK) {
            KLANG_LOG_ERR("+++ (ERROR) could not remove beat timer");
        }
    }
    if (micro_seconds > 0) {
        const float mMilliSeconds = micro_seconds / 1000.0f;
        mTimerID                  = SDL_AddTimer(mMilliSeconds, beat_callback, nullptr);
        if (!mTimerID) {
            KLANG_LOG_ERR("+++ (ERROR) could not start beat");
        }
        mTimerDisabled = false;
    } else {
        mTimerDisabled = true;
    }
}

void init_audio_output() {
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK * 2; i++) {
            fInternalAudioOutputBuffers[j][i] = 0.0;
        }
    }

    if (SDL_GetNumAudioDevices(AUDIO_OUTPUT) < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio output device! Error: %s", SDL_GetError());
    } else {
        SDL_AudioSpec mRequestedAudioOutputSpecs;  // https://wiki.libsdl.org/SDL_AudioSpec
        SDL_zero(mRequestedAudioOutputSpecs);
        mRequestedAudioOutputSpecs.freq     = KLANG_AUDIO_RATE;
        mRequestedAudioOutputSpecs.format   = AUDIO_F32;
        mRequestedAudioOutputSpecs.channels = 2;
        mRequestedAudioOutputSpecs.samples  = KLANG_SAMPLES_PER_AUDIO_BLOCK;
        mRequestedAudioOutputSpecs.callback = audio_output_callback;

        SDL_AudioSpec mAudioOutputDeviceSpecs;
        mAudioOutputDeviceInternalID = SDL_OpenAudioDevice(
            mAudioOutputDeviceID == AUDIO_DEFAULT_DEVICE ? NULL : SDL_GetAudioDeviceName(mAudioOutputDeviceID, AUDIO_OUTPUT),
            AUDIO_OUTPUT,
            &mRequestedAudioOutputSpecs,
            &mAudioOutputDeviceSpecs,
            SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        //        mAudioOutputDeviceInternalID = SDL_OpenAudioDevice(
        //                SDL_GetAudioDeviceName(mAudioOutputDeviceID, AUDIO_OUTPUT),
        //                AUDIO_OUTPUT,
        //                &mRequestedAudioOutputSpecs,
        //                &mAudioOutputDeviceSpecs,
        //                SDL_AUDIO_ALLOW_FORMAT_CHANGE);

#ifdef DEBUG_AUDIO_DEVICE
        KLANG_LOG(
            "+++ (INFO) audio output device configuration initialized:");
        KLANG_LOG("           frequency ... : %i", mAudioOutputDeviceSpecs.freq);
        KLANG_LOG("           channels .... : %i", mAudioOutputDeviceSpecs.channels);
        KLANG_LOG("           samples ..... : %i", mAudioOutputDeviceSpecs.samples);
#endif

        if (mAudioOutputDeviceInternalID == 0) {
            KLANG_LOG_ERR(
                "+++ (ERROR) opening audio output device: %i",
                mAudioOutputDeviceID);
        }
#ifdef DEBUG_AUDIO_DEVICE
        KLANG_LOG("+++ (INFO) selecting audio output device: %s",
                  mAudioOutputDeviceID == AUDIO_DEFAULT_DEVICE ? "(system default)" : SDL_GetAudioDeviceName(mAudioOutputDeviceID, AUDIO_OUTPUT));
#endif
    }
}

#ifdef KLST_SDL_USE_AUDIO_INPUT
void audio_input_callback(void *userdata, Uint8 *pStream, int pBufferLength) {
    mCurrentInputBufferID++;
    mCurrentInputBufferID %= NUM_INPUT_SWAP_BUFFERS;
    for (int i = 0; i < pBufferLength; i += KLANG_INPUT_CHANNELS * 4) {
        for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
            fInputBuffers[mCurrentInputBufferID][j][i / 8] = FLOAT_32(pStream, i + j * 4);
        }
    }
}

void init_audio_input() {
    if (SDL_GetNumAudioDevices(AUDIO_INPUT) < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio input device! Error: %s", SDL_GetError());
    } else {
        // audio specs
        SDL_AudioSpec mRequestedAudioInputSpecs;
        SDL_zero(mRequestedAudioInputSpecs);
        mRequestedAudioInputSpecs.freq     = KLANG_AUDIO_RATE;
        mRequestedAudioInputSpecs.format   = AUDIO_F32;
        mRequestedAudioInputSpecs.channels = 2;
        mRequestedAudioInputSpecs.samples  = KLANG_SAMPLES_PER_AUDIO_BLOCK;
        mRequestedAudioInputSpecs.callback = audio_input_callback;

        SDL_AudioSpec mAudioInputDeviceSpecs;
        mAudioInputDeviceInternalID = SDL_OpenAudioDevice(
            mAudioInputDeviceID == AUDIO_DEFAULT_DEVICE ? NULL : SDL_GetAudioDeviceName(mAudioInputDeviceID, AUDIO_INPUT),
            //                SDL_GetAudioDeviceName(mAudioInputDeviceID, AUDIO_INPUT),
            AUDIO_INPUT,
            &mRequestedAudioInputSpecs,
            &mAudioInputDeviceSpecs,
            SDL_AUDIO_ALLOW_ANY_CHANGE);

#ifdef DEBUG_AUDIO_DEVICE
        KLANG_LOG(
            "+++ (INFO) audio input device configuration initialized:");
        KLANG_LOG("           frequency ... : %i", mAudioInputDeviceSpecs.freq);
        KLANG_LOG("           channels .... : %i", mAudioInputDeviceSpecs.channels);
        KLANG_LOG("           samples ..... : %i", mAudioInputDeviceSpecs.samples);
#endif

        if (mAudioInputDeviceInternalID == 0) {
            KLANG_LOG_ERR(
                "+++ (ERROR) opening audio input device: %i",
                mAudioInputDeviceID);
        }
#ifdef DEBUG_AUDIO_DEVICE
        KLANG_LOG("+++ (INFO) selecting audio input device: %s",
                  mAudioInputDeviceID == AUDIO_DEFAULT_DEVICE ? "(system default)" : SDL_GetAudioDeviceName(mAudioInputDeviceID, AUDIO_INPUT));
#endif
    }
    //    //Lock callback
    //    SDL_LockAudioDevice( mAudioInputDeviceID );
    //
    //    //Finished input
    //    if( gBufferBytePosition > gBufferByteMaxPosition ) {
    //        //Stop input audio
    //        SDL_PauseAudioDevice( mAudioInputDeviceID, SDL_TRUE );
    //    }
    //    // Unlock callback
    //    SDL_UnlockAudioDevice( mAudioInputDeviceID );
}
#endif

void init_app() {
    update_audiobuffer();
}

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
void init_renderer() {
    auto               t  = std::time(nullptr);
    auto               tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::string str         = oss.str();
    std::string mWindowName = "Klangstrom ";
    mWindowName.append(str);

    mWindow = SDL_CreateWindow(
        mWindowName.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_SOFTWARE
        // DL_RENDERER_ACCELERATED
    );
}

// void init_card() {
//     CardPtr = new KlangstromCardBSP_SDL();
// }

const char *get_board_name(uint8_t pBoardID) {
    switch (pBoardID) {
        case KLANG_BOARD_EMULATOR_KLST_CORE:
            return "KLST_CORE ";
            break;
        case KLANG_BOARD_EMULATOR_KLST_TINY:
            return "KLST_TINY ";
            break;
        case KLANG_BOARD_EMULATOR_KLST_SHEEP:
            return "KLST_SHEEP";
            break;
        default:
            return "";
            break;
    }
}

void init_SDL() {
    KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");
    KLANG_LOG("+++                                            +++");
    KLANG_LOG("+++ @Klang_SDL                                 +++");
    KLANG_LOG("+++ KLANG_AUDIO_RATE              : %i      +++", KLANG_AUDIO_RATE);
#ifdef KLANG_AUDIO_BLOCKS
    KLANG_LOG("+++ KLANG_AUDIO_BLOCKS            : %i         +++", KLANG_AUDIO_BLOCKS);
#endif
    KLANG_LOG("+++ KLANG_SAMPLES_PER_AUDIO_BLOCK : %i       +++", KLANG_SAMPLES_PER_AUDIO_BLOCK);
#ifdef KLST_SDL_USE_OSC
    KLANG_LOG("+++ KLANG_OSC_TRANSMIT_ADDRESS    : %s", KLANG_OSC_TRANSMIT_ADDRESS);
    KLANG_LOG("+++ KLANG_OSC_TRANSMIT_PORT       : %i", KLANG_OSC_TRANSMIT_PORT);
    KLANG_LOG("+++ KLANG_OSC_RECEIVE_PORT        : %i", KLANG_OSC_RECEIVE_PORT);
#endif
    KLANG_LOG("+++ KLANG_BOARD_EMULATOR          : %s +++", get_board_name(KLANG_BOARD_EMULATOR));
    KLANG_LOG("+++                                            +++");
    KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        KLANG_LOG_ERR("@klangstrom_arduino unable to init SDL: %s", SDL_GetError());
    }
}

void shutdown_main() {
    KLANG_LOG("+++ shutting down gracefully");
    SDL_AudioQuit();
    SDL_CloseAudio();
    SDL_Quit();
}

void klangstrom_arduino_emu_transmit_serial(int pPort, int pData) {
#ifdef KLST_SDL_USE_OSC
#ifdef DEBUG_OSC
    KLANG_LOG("+++ send OSC: %i, %i", pPort, pData);
#endif
    char                      buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate
      << osc::BeginMessage(KLANG_OSC_SERIAL)
      << pPort
      << pData
      << osc::EndMessage
      << osc::EndBundle;
    mTransmitSocket->Send(p.Data(), p.Size());
#endif
}

#ifdef KLST_SDL_USE_OSC

void process_KLANGSTROM_OSC_SERIAL(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp KLANG_OSC_SERIAL: %s, %d (%s)", msg.AddressPattern(), msg.ArgumentCount(), msg.TypeTags());
#endif
    osc::ReceivedMessageArgumentStream args = msg.ArgumentStream();
    int                                mSerialPort;
    int                                mSerialData;
    args >> mSerialPort >> mSerialData >> osc::EndMessage;
    Serial.recv(mSerialData);
    serialEvent();
}

void process_KLANG_OSC_CMD(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp onReceive *KLANG_OSC_CMD*     : %s", KLANG_OSC_CMD);
#endif
    uint8_t mData[msg.ArgumentCount()];
    uint8_t i = 0;
    for (osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
         arg != msg.ArgumentsEnd(); ++arg) {
        mData[i] = (uint8_t)arg->AsInt32();
        i++;
    }
    data_receive(ALL_PERIPHERALS, mData, msg.ArgumentCount());
}

void process_KLANG_OSC_DATA(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp onReceive *KLANG_OSC_DATA* : %s", KLANG_OSC_DATA);
#endif
    if (msg.ArgumentCount() > 1) {
        const uint8_t                        mLength = msg.ArgumentCount() - 1;
        osc::ReceivedMessage::const_iterator arg     = msg.ArgumentsBegin();
        const uint8_t                        mSender = (uint8_t)arg->AsInt32();
        ++arg;
        uint8_t mData[mLength];
        uint8_t i = 0;
        while (arg != msg.ArgumentsEnd()) {
            mData[i] = (uint8_t)arg->AsInt32();
            ++arg;
            ++i;
        }
        data_receive(mSender, mData, mLength);
    }
}

void process_KLANG_OSC_MIDI_IN(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp onReceive *KLANG_OSC_MIDI_IN* : %s", KLANG_OSC_MIDI_IN);
#endif
    EVENT_TYPE mMidiInEvent;
    if (msg.ArgumentCount() > 0) {
        osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
        mMidiInEvent                             = (EVENT_TYPE)arg->AsInt32();
        if (msg.ArgumentCount() > 1) {
            uint8_t mData[msg.ArgumentCount() - 1];
            uint8_t i = 0;
            for (osc::ReceivedMessage::const_iterator args = msg.ArgumentsBegin();
                 args != msg.ArgumentsEnd(); ++args) {
                if (i == 0) {
                    args++;
                }
                mData[i] = args->AsInt32();
                i++;
            }
            event_receive(mMidiInEvent, mData);
        } else {
            event_receive(mMidiInEvent, nullptr);
        }
    }
}

class MOscPacketListener : public osc::OscPacketListener {
private:
    bool addr_pattern_equals(const osc::ReceivedMessage &msg, const char *pAddrPatter) {
        return (strcmp(msg.AddressPattern(), pAddrPatter) == 0);
    }

protected:
    void ProcessMessage(const osc::ReceivedMessage &msg,
                        const IpEndpointName       &remoteEndpoint) override {
        (void)remoteEndpoint;  // suppress unused parameter warning
        try {
            if (addr_pattern_equals(msg, KLANG_OSC_CMD)) {
                process_KLANG_OSC_CMD(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_DATA)) {
                process_KLANG_OSC_DATA(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_MIDI_IN)) {
                process_KLANG_OSC_MIDI_IN(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_SERIAL)) {
                process_KLANGSTROM_OSC_SERIAL(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_SIM)) {
                mEmulator.process_receive(msg);
#ifdef DEBUG_OSC
            } else {
                KLANG_LOG("@sdlApp onReceive: %s, %d", msg.AddressPattern(), msg.ArgumentCount());
#endif
            }
            //            for (osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg) {
            //                if (arg->IsMidiMessage()) {
            //                    arg->AsMidiMessageUnchecked();
            //                } else if (arg->IsFloat()) {
            //                    float f = arg->AsFloatUnchecked();
            //                    float g = arg->AsFloat();
            //                }
            //            }
            //
            //            if (strcmp(m.AddressPattern(), "/test1") == 0) {
            //                // example #1 -- argument stream interface
            //                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            //                bool a1;
            //                osc::int32 a2;
            //                float a3;
            //                const char *a4;
            //                args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;
            //
            //                cout << "received '/test1' message with arguments: "
            //                          << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            //
            //            } else if (strcmp(m.AddressPattern(), "/test2") == 0) {
            //                // example #2 -- argument iterator interface, supports
            //                // reflection for overloaded messages (eg you can call
            //                // (*arg)->IsBool() to check if a bool was passed etc).
            //                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
            //                bool a1 = (arg++)->AsBool();
            //                int a2 = (arg++)->AsInt32();
            //                float a3 = (arg++)->AsFloat();
            //                const char *a4 = (arg++)->AsString();
            //                if (arg != m.ArgumentsEnd())
            //                    throw osc::ExcessArgumentException();
            //
            //                cout << "received '/test2' message with arguments: "
            //                          << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            //            }
        } catch (osc::Exception &e) {
            KLANG_LOG("@klangstrom_arduino error in OSC listener: %s - %s", msg.AddressPattern(), e.what());
        }
    }
};

void osc_thread() {
    try {
#define USE_UDP_MULTICAST
#ifdef USE_UDP_MULTICAST
        MOscPacketListener mOscListener;
        PacketListener    *listener_       = &mOscListener;
        IpEndpointName     mIpEndpointName = IpEndpointName(KLANG_OSC_TRANSMIT_ADDRESS, KLANG_OSC_RECEIVE_PORT);
        if (mIpEndpointName.IsMulticastAddress()) {
#ifdef DEBUG_OSC
            KLANG_LOG("@klangstrom_arduino using UDP Multicast");
#endif
            UdpSocket mUdpSocket;
            mUdpSocket.SetAllowReuse(true);
            mUdpSocket.Bind(mIpEndpointName);
            SocketReceiveMultiplexer mux_;
            mux_.AttachSocketListener(&mUdpSocket, listener_);
            mux_.Run();
        } else {
            UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, KLANG_OSC_RECEIVE_PORT),
                                        &mOscListener);
            s.Run();
        }
#else
        MOscPacketListener        mOscListener;
        UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, KLANG_OSC_RECEIVE_PORT), &mOscListener);
        s.Run();
#endif
    } catch (exception &e) {
        string mError = e.what();
        KLANG_LOG("@klangstrom_arduino osc_thread %s", mError.c_str());
    }
}
#endif

void klangstrom_arduino_event_transmit(EVENT_TYPE pEvent, float *data) {
#ifdef KLST_SDL_USE_OSC
    // @todo(implement all events)
    char                      buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    if (pEvent == EVENT_MIDI_IN_NOTE_ON) {
        p << osc::BeginBundleImmediate
          << osc::BeginMessage(KLANG_OSC_MIDI_OUT)
          << data[CHANNEL]
          << data[NOTE]
          << data[VELOCITY]
          << osc::EndMessage
          << osc::EndBundle;
        mTransmitSocket->Send(p.Data(), p.Size());
    }
#endif
}

void klangstrom_arduino_data_transmit(const uint8_t pSender, uint8_t *pData, uint16_t pDataLength) {
#ifdef KLST_SDL_USE_OSC
    // @todo(implement all events)
    char                      buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate
      << osc::BeginMessage(KLANG_OSC_DATA);
    p << pSender;
    for (uint8_t i = 0; i < pDataLength; i++) {
        p << pData[i];
    }
    p << osc::EndMessage
      << osc::EndBundle;
    mTransmitSocket->Send(p.Data(), p.Size());
#endif
}

void klangstrom_arduino_sim_transmit(std::vector<float> &pData) {
#ifdef KLST_SDL_USE_OSC
    char                      buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate
      << osc::BeginMessage(KLANG_OSC_SIM);
    for (int n : pData) {
        p << n;
    }
    p << osc::EndMessage
      << osc::EndBundle;
    mTransmitSocket->Send(p.Data(), p.Size());
#endif
}

#ifdef KLST_SDL_USE_OSC
void init_osc() {
    mOSCThread                   = thread(osc_thread);
    IpEndpointName mEndpointName = IpEndpointName(KLANG_OSC_TRANSMIT_ADDRESS, KLANG_OSC_TRANSMIT_PORT);
    mTransmitSocket              = new UdpTransmitSocket(mEndpointName);

    /* --- OSC send test --- */
    // //@todo(remove this … it s just a test)
    // float data[3] = {0, 36, 100};
    // klangstrom_arduino_event_transmit(EVENT_MIDI_IN_NOTE_ON, data);
}

void shutdown_osc() {
    mOSCThread.detach();
}
#endif

void initVariant() {
// int main(int argc, char **argv) {
#ifdef KLST_SDL_PRINT_SDL_VERSION
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("+++ SDL VERION: \n");
    printf("+++ compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
    printf("+++   linked against SDL version %d.%d.%d\n", linked.major, linked.minor, linked.patch);
#endif

#ifdef KLST_SDL_USE_OSC
    init_osc();
#endif
    std::cout << std::flush;
    // init_card();
    init_main();
    std::cout << std::flush;
    loop_main();
    shutdown_main();
#ifdef KLST_SDL_USE_OSC
    shutdown_osc();
#endif
}

#endif  // (KLST_ARCH==KLST_ARCH_DESKTOP)
