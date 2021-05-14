//
//  klangstrom_arduino_sdl.cpp 
//  Klang – a node+text-based synthesizer library
//
//
//

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <cstring>
#include <vector>

#include "SDL.h"
#include "SDL_audio.h"
#include "SDL_render.h"

#include "Arduino.h"
#include "klangstrom_arduino_sdl.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include <thread>
#include <chrono>

extern KLST_Simulator mSimulator;

#define Q(x) #x
#define QUOTE(x) Q(x)
#define APP_HEADER QUOTE(KLANG_APP_CLASS.hpp)

/* KLST -------------------------------------------------------------------------------- */

//@todo(move this to `KlangstromDefines`)

#if defined(KLANG_TARGET_MCU)
#define KLANG_NL "\n\r"
#else
#define KLANG_NL "\n"
#endif

#if (KLANG_DEBUG_LEVEL == 0)
#define  KLANG_LOG_ERR(...)
#define  KLANG_LOG(...)
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 1)
#define  KLANG_LOG_ERR(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 2)
#define  KLANG_LOG_ERR(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL >= 3)
#define  KLANG_LOG_ERR(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG_SIM(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#endif

/* KLST -------------------------------------------------------------------------------- */

#include "KLST_SDL-adapter.h"
#include "KlangstromApplicationArduino.h"

float FLOAT_32(uint8_t* pBytes, uint32_t pOffset) {
    float output;

    *((uint8_t*)(&output) + 0) = pBytes[0 + pOffset];
    *((uint8_t*)(&output) + 1) = pBytes[1 + pOffset];
    *((uint8_t*)(&output) + 2) = pBytes[2 + pOffset];
    *((uint8_t*)(&output) + 3) = pBytes[3 + pOffset];

    return output;
}

void delay(uint32_t pMS) { 
    this_thread::sleep_for(chrono::milliseconds(pMS));
}

/* KLST -------------------------------------------------------------------------------- */

using namespace klangstrom;
using namespace std;

thread mOSCThread;
UdpTransmitSocket *mTransmitSocket = nullptr;

thread mLoopThread;

float mInternalAudioOutputBufferLeft[KLANG_SAMPLES_PER_AUDIO_BLOCK * 2];
float mInternalAudioOutputBufferRight[KLANG_SAMPLES_PER_AUDIO_BLOCK * 2];
int mBufferOffset = 0;
uint32_t mBeatCounter = 0;
SDL_TimerID mTimerID = 0;
SDL_Window *mWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
bool mQuitFlag = false;

const static int8_t AUDIO_DEFAULT_DEVICE = -1;
int8_t mAudioInputDeviceID = AUDIO_DEFAULT_DEVICE;
int8_t mAudioOutputDeviceID = AUDIO_DEFAULT_DEVICE;

SDL_AudioDeviceID mAudioInputDeviceInternalID = 0;
SDL_AudioDeviceID mAudioOutputDeviceInternalID = 0;

const static int8_t NUM_OF_RX_BUFFER = 2;
uint8_t mCurrentInputBufferID = 0;
SIGNAL_TYPE mInputBufferLeft[NUM_OF_RX_BUFFER][KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mInputBufferRight[NUM_OF_RX_BUFFER][KLANG_SAMPLES_PER_AUDIO_BLOCK];

#define DEBUG_AUDIO_DEVICE
#define AUDIO_INPUT SDL_TRUE
#define AUDIO_OUTPUT SDL_FALSE

void init_main() {
    init_SDL();
    init_renderer();
    init_app();

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
    init_audio_input();
    start_audio();
}

void loop_thread() {
    while (!mQuitFlag) {
        loop();
//         this_thread::sleep_for(chrono::milliseconds(get_delay()));
    }
}

void loop_main() {
    setup();
    mLoopThread = thread(loop_thread);
    while (!mQuitFlag) {
        loop_event();
        loop_renderer();
        // @note(moved `loop()` to own thread)
    }
    mLoopThread.detach();
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
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            float data[3] = {(float) x / SCREEN_WIDTH, (float) y / SCREEN_HEIGHT,
                             (float) LEFT};
             event_receive(EVENT_MOUSE_PRESSED, data);
        }
        if (e.type == SDL_MOUSEBUTTONUP) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            float data[3] = {(float) x / SCREEN_WIDTH, (float) y / SCREEN_HEIGHT,
                             (float) LEFT};
             event_receive(EVENT_MOUSE_RELEASED, data);
        }
        if (e.type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                float data[3] = {(float) x / SCREEN_WIDTH, (float) y / SCREEN_HEIGHT,
                                 (float) LEFT};
                 event_receive(EVENT_MOUSE_DRAGGED, data);
            } else {
                float data[2] = {(float) x / SCREEN_WIDTH, (float) y / SCREEN_HEIGHT};
                 event_receive(EVENT_MOUSE_MOVED, data);
            }
        }
        if (e.type == SDL_KEYDOWN) {
            const char mKey = e.key.keysym.scancode;
            const float data[1] = {(float) (SDL_GetScancodeName(
                    (SDL_Scancode) mKey)[0])}; /* @TODO("dirty hack … this needs to be handle differently") */
             event_receive(EVENT_KEY_PRESSED, data);
#ifdef DEBUG_PRINT_EVENTS
            KLANG_LOG("key pressed: %c", SDL_GetScancodeName((SDL_Scancode) mKey)[0]);
#endif
        }
        if (e.type == SDL_KEYUP) {
            const char mKey = e.key.keysym.scancode;
            const float data[1] = {(float) (SDL_GetScancodeName(
                    (SDL_Scancode) mKey)[0])}; /* @TODO("dirty hack … this needs to be handle differently") */
             event_receive(EVENT_KEY_RELEASED, data);
#ifdef DEBUG_PRINT_EVENTS
            KLANG_LOG("key released: %c", SDL_GetScancodeName((SDL_Scancode) mKey)[0]);
#endif
        }
    }
}

void loop_renderer() {
    SDL_SetRenderDrawColor(gRenderer, 0x20, 0x20, 0x20, 0xFF);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int j = (i - 1) + mBufferOffset;
        int k = i + mBufferOffset;
        int x0 = SCREEN_WIDTH * (float) (i - 1) / (float) KLANG_SAMPLES_PER_AUDIO_BLOCK;
        int x1 = SCREEN_WIDTH * (float) i / (float) KLANG_SAMPLES_PER_AUDIO_BLOCK;
        int y0_left = mInternalAudioOutputBufferRight[j] * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4;
        int y1_left = mInternalAudioOutputBufferRight[k] * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4;
        int y0_right = mInternalAudioOutputBufferLeft[j] * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4 * 3;
        int y1_right = mInternalAudioOutputBufferLeft[k] * SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 4 * 3;
        SDL_RenderDrawLine(gRenderer, x0, y0_left, x1, y1_left);
        SDL_RenderDrawLine(gRenderer, x0, y0_right, x1, y1_right);
    }

    /* draw LEDs */
    static const uint8_t mWidth = 16;
    static const uint8_t mHeight = 32;
    static const uint8_t mSpacing = 20;
    for (uint8_t i=0; i < NUMBER_OF_LEDS; i++) {
        SDL_Rect r;
        r.x = 20 + i * mSpacing;
        r.y = 20;
        r.w = mWidth;
        r.h = mHeight;
        if (mSimulator.getLEDs()[i]) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawRect(gRenderer, &r);
        }
    }

    SDL_RenderPresent(gRenderer);
}

void start_audio() {
    SDL_PauseAudioDevice(mAudioInputDeviceInternalID, SDL_FALSE);
    SDL_PauseAudioDevice(mAudioOutputDeviceInternalID, SDL_FALSE);
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

void update_audiobuffer() {
    auto *mOutputBufferLeft = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    auto *mOutputBufferRight = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    // mCurrentInputBufferID
    const uint8_t _mCurrentInputBufferID = mCurrentInputBufferID;
    audioblock(mOutputBufferLeft, mOutputBufferRight, mInputBufferLeft[_mCurrentInputBufferID], mInputBufferRight[_mCurrentInputBufferID]);
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        mInternalAudioOutputBufferLeft[i + mBufferOffset] = mOutputBufferLeft[i];
        mInternalAudioOutputBufferRight[i + mBufferOffset] = mOutputBufferRight[i];
    }
    delete[] mOutputBufferLeft;
    delete[] mOutputBufferRight;
}

void audio_output_callback(void *userdata, Uint8 *pStream, int pBufferLength) {
    /* for `AUDIO_F32` the stride is 8 = 4 Bytes LEFT + 4 Bytes RIGHT */
#define LEFT_POS  0
#define RIGHT_POS 4
    uint16_t mBufferPos = 0;
    for (int i = 0; i < pBufferLength; i += 8) {
        auto *mLeftF = (uint8_t *) &mInternalAudioOutputBufferLeft[mBufferPos + mBufferOffset];
        auto *mRightF = (uint8_t *) &mInternalAudioOutputBufferRight[mBufferPos + mBufferOffset];
        for (uint8_t j = 0; j < 4; j++) {
            pStream[i + LEFT_POS + j] = mLeftF[j];
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

void klangstrom_arduino_beats_per_minute(float pBPM) {
    if (pBPM > 0) {
        const float mMilliSeconds = 1000.0f / (pBPM / 60.0f);
        const uint32_t pMicroSeconds = (uint32_t)(mMilliSeconds * 1000);
        klangstrom_arduino_beats_per_minute_ms(pMicroSeconds);
    }
}

// void klangstrom_arduino_beats_per_minute(float pBPM) {
//     if (mTimerID > 0) {
//         bool mOK = SDL_RemoveTimer(mTimerID);
//         if (!mOK) {
//             KLANG_LOG_ERR("+++ (ERROR) could not remove beat timer");
//         }
//     }
//     if (pBPM > 0) {
//         const float mMilliSeconds = 1000.0f / (pBPM / 60.0f);
//         mTimerID = SDL_AddTimer(mMilliSeconds, beat_callback, nullptr);
//         if (!mTimerID) {
//             KLANG_LOG_ERR("+++ (ERROR) could not start beat");
//         }
//     }
// }

void klangstrom_arduino_beats_per_minute_ms(uint32_t pMicroSeconds) {
    if (mTimerID > 0) {
        bool mOK = SDL_RemoveTimer(mTimerID);
        if (!mOK) {
            KLANG_LOG_ERR("+++ (ERROR) could not remove beat timer");
        }
    }
    if (pMicroSeconds > 0) {
        const float mMilliSeconds = pMicroSeconds / 1000.0f;
        mTimerID = SDL_AddTimer(mMilliSeconds, beat_callback, nullptr);
        if (!mTimerID) {
            KLANG_LOG_ERR("+++ (ERROR) could not start beat");
        }
    }
}

void init_audio_output() {
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK * 2; i++) {
        mInternalAudioOutputBufferLeft[i] = 0.0;
        mInternalAudioOutputBufferRight[i] = 0.0;
    }

    if (SDL_GetNumAudioDevices(AUDIO_OUTPUT) < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio output device! Error: %s", SDL_GetError());
    } else {
        SDL_AudioSpec mRequestedAudioOutputSpecs; // https://wiki.libsdl.org/SDL_AudioSpec
        SDL_zero(mRequestedAudioOutputSpecs);
        mRequestedAudioOutputSpecs.freq = KLANG_AUDIO_RATE;
        mRequestedAudioOutputSpecs.format = AUDIO_F32;
        mRequestedAudioOutputSpecs.channels = 2;
        mRequestedAudioOutputSpecs.samples = KLANG_SAMPLES_PER_AUDIO_BLOCK;
        mRequestedAudioOutputSpecs.callback = audio_output_callback;

        SDL_AudioSpec mAudioOutputDeviceSpecs;
        mAudioOutputDeviceInternalID = SDL_OpenAudioDevice(
                mAudioOutputDeviceID == AUDIO_DEFAULT_DEVICE ? NULL : SDL_GetAudioDeviceName(mAudioOutputDeviceID,
                                                                                             AUDIO_OUTPUT),
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
                  mAudioOutputDeviceID == AUDIO_DEFAULT_DEVICE ? "(system default)" : SDL_GetAudioDeviceName(
                          mAudioOutputDeviceID, AUDIO_OUTPUT));
#endif
    }
}

void audio_input_callback(void *userdata, Uint8 *pStream, int pBufferLength) {
    mCurrentInputBufferID++;
    mCurrentInputBufferID %= NUM_OF_RX_BUFFER;
    for (int i = 0; i < pBufferLength; i += 8) {
        mInputBufferLeft[mCurrentInputBufferID][i / 8] = FLOAT_32(pStream, i + 0);
        mInputBufferRight[mCurrentInputBufferID][i / 8] = FLOAT_32(pStream, i + 4);
    }
}

void init_audio_input() {
    if (SDL_GetNumAudioDevices(AUDIO_INPUT) < 1) {
        KLANG_LOG_ERR("+++ (ERROR) unable to get audio input device! Error: %s", SDL_GetError());
    } else {
        // audio specs
        SDL_AudioSpec mRequestedAudioInputSpecs;
        SDL_zero(mRequestedAudioInputSpecs);
        mRequestedAudioInputSpecs.freq = KLANG_AUDIO_RATE;
        mRequestedAudioInputSpecs.format = AUDIO_F32;
        mRequestedAudioInputSpecs.channels = 2;
        mRequestedAudioInputSpecs.samples = KLANG_SAMPLES_PER_AUDIO_BLOCK;
        mRequestedAudioInputSpecs.callback = audio_input_callback;

        SDL_AudioSpec mAudioInputDeviceSpecs;
        mAudioInputDeviceInternalID = SDL_OpenAudioDevice(
                mAudioInputDeviceID == AUDIO_DEFAULT_DEVICE ? NULL : SDL_GetAudioDeviceName(mAudioInputDeviceID,
                                                                                            AUDIO_INPUT),
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
                  mAudioInputDeviceID == AUDIO_DEFAULT_DEVICE ? "(system default)" : SDL_GetAudioDeviceName(
                          mAudioInputDeviceID, AUDIO_INPUT));
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

void init_app() {
//     mApp = new KLANG_APP_CLASS();
    update_audiobuffer();
}

void init_renderer() {
    mWindow = SDL_CreateWindow("klangstrom_arduino", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}


void init_SDL() {
    KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");
    KLANG_LOG("+++ @Klang_SDL                                 +++");
    KLANG_LOG("+++ KLANG_AUDIO_RATE              : %i", KLANG_AUDIO_RATE);
#ifdef KLANG_AUDIO_BLOCKS
    KLANG_LOG("+++ KLANG_AUDIO_BLOCKS            : %i", KLANG_AUDIO_BLOCKS);
#endif
    KLANG_LOG("+++ KLANG_SAMPLES_PER_AUDIO_BLOCK : %i", KLANG_SAMPLES_PER_AUDIO_BLOCK);
#ifdef KLANG_AUDIO_BLOCKS
    KLANG_LOG("+++ KLANG_SIGNAL_TYPE             : %i", KLANG_SIGNAL_TYPE);
#endif
    KLANG_LOG("+++ KLANG_OSC_TRANSMIT_ADDRESS    : %s", KLANG_OSC_TRANSMIT_ADDRESS);
    KLANG_LOG("+++ KLANG_OSC_TRANSMIT_PORT       : %i", KLANG_OSC_TRANSMIT_PORT);
    KLANG_LOG("+++ KLANG_OSC_RECEIVE_PORT        : %i", KLANG_OSC_RECEIVE_PORT);
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        KLANG_LOG_ERR("@klangstrom_arduino unable to init SDL: %s", SDL_GetError());
    }
}

void shutdown_main() {
    SDL_AudioQuit();
    SDL_CloseAudio();
    SDL_Quit();
}

void process_KLANG_OSC_CMD(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp onReceive *KLANG_OSC_CMD*     : %s", KLANG_OSC_CMD);
#endif
    uint8_t mData[msg.ArgumentCount()];
    uint8_t i = 0;
    for (osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
            arg != msg.ArgumentsEnd(); ++arg) {
        mData[i] = (uint8_t) arg->AsInt32();
        i++;
    }
    data_receive(ALL_PERIPHERALS, mData, msg.ArgumentCount());
}

void process_KLANG_OSC_DATA(const osc::ReceivedMessage &msg) {
#ifdef DEBUG_OSC
    KLANG_LOG("@sdlApp onReceive *KLANG_OSC_DATA* : %s", KLANG_OSC_DATA);
#endif
    if (msg.ArgumentCount() > 1) {
        const uint8_t mLength = msg.ArgumentCount() - 1;
        osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
        const uint8_t mSender = (uint8_t)arg->AsInt32();
        ++arg;
        uint8_t mData[mLength];
        uint8_t i = 0;
        while(arg != msg.ArgumentsEnd()) {
            mData[i] = (uint8_t) arg->AsInt32();
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
        mMidiInEvent = (EVENT_TYPE) arg->AsInt32();
        if (msg.ArgumentCount() > 1) {
            float mData[msg.ArgumentCount() - 1];
            uint8_t i = 0;
            for (osc::ReceivedMessage::const_iterator args = msg.ArgumentsBegin();
                    args != msg.ArgumentsEnd(); ++args) {
                if (i == 0) { args++; }
                mData[i] = (float) args->AsInt32();
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
                        const IpEndpointName &remoteEndpoint) override {
        (void) remoteEndpoint; // suppress unused parameter warning
        try {
            if (addr_pattern_equals(msg, KLANG_OSC_CMD)) {
                process_KLANG_OSC_CMD(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_DATA)) {
                process_KLANG_OSC_DATA(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_MIDI_IN)) {
                process_KLANG_OSC_MIDI_IN(msg);
            } else if (addr_pattern_equals(msg, KLANG_OSC_SIM)) {
                mSimulator.process_receive(msg);
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
        PacketListener *listener_ = &mOscListener;
        IpEndpointName mIpEndpointName = IpEndpointName(KLANG_OSC_TRANSMIT_ADDRESS, KLANG_OSC_RECEIVE_PORT);
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
        MOscPacketListener mOscListener;
        UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, KLANG_OSC_RECEIVE_PORT), &mOscListener);
        s.Run();
#endif
    } catch (exception &e) {
        string mError = e.what();
        KLANG_LOG("@klangstrom_arduino osc_thread %s", mError.c_str());
    }
}

void klangstrom_arduino_event_transmit(EVENT_TYPE pEvent, float *data) {
    // @todo(implement all events)
    char buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
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
}

void klangstrom_arduino_data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength) {
    // @todo(implement all events)
    char buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate
      << osc::BeginMessage(KLANG_OSC_DATA);
    p << pSender;
    for (uint8_t i=0; i<pDataLength; i++) {
        p << pData[i];
    }
    p <<  osc::EndMessage
        << osc::EndBundle;
    mTransmitSocket->Send(p.Data(), p.Size());
}

void klangstrom_arduino_sim_transmit(std::vector<float> &pData) {
    char buffer[OSC_TRANSMIT_OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OSC_TRANSMIT_OUTPUT_BUFFER_SIZE);
    p << osc::BeginBundleImmediate
      << osc::BeginMessage(KLANG_OSC_SIM);
    for (int n : pData) {
        p << n;
    }
    p << osc::EndMessage
      << osc::EndBundle;
    mTransmitSocket->Send(p.Data(), p.Size());
}

void init_osc() {
    mOSCThread = thread(osc_thread);
    IpEndpointName mEndpointName = IpEndpointName(KLANG_OSC_TRANSMIT_ADDRESS, KLANG_OSC_TRANSMIT_PORT);
    mTransmitSocket = new UdpTransmitSocket(mEndpointName);

    /* --- OSC send test --- */
    // //@todo(remove this … it s just a test)
    // float data[3] = {0, 36, 100};
    // klangstrom_arduino_event_transmit(EVENT_MIDI_IN_NOTE_ON, data);
}

void shutdown_osc() {
    mOSCThread.detach();
}

int main(int argc, char **argv) {
    init_osc();
    std::cout << std::flush;
    init_main();
    std::cout << std::flush;
    loop_main();
    shutdown_main();
    shutdown_osc();
    return 0;
}
