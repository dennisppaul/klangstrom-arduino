//
//  klangstrom_arduino_sdl.h
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef klangstrom_arduino_sdl_h
#define klangstrom_arduino_sdl_h

// #include "klangstrom_arduino_defines.h"
#include "KLST-defines.h"
#include "KLST-adapter.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//@todo maybe merge this into `KLST_SDL-adapter.h`
void klangstrom_arduino_beats_per_minute(float pBPM);
void klangstrom_arduino_beats_per_minute_ms(uint32_t pMicroSeconds);

void init_main();

void init_audio_output();

void init_audio_input();

void init_SDL();

void init_renderer();

void init_app();

void loop_renderer();

void loop_event();

void start_audio();

void list_audio_devices();

void update_audiobuffer();

void shutdown_main();

/* klang::KlangApplicationInterface */

void klang_sdl_event_transmit(EVENT_TYPE pEvent, float *data);

#endif /* klangstrom_arduino_sdl_h */
 