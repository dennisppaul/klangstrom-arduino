//
//  klangstrom_arduino_sdl.h
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef klangstrom_arduino_sdl_h
#define klangstrom_arduino_sdl_h

#include <vector>

#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"
#include "KlangstromApplicationInterfaceArduino.h"
#include "KLST_Simulator.h"

const uint16_t SCREEN_WIDTH = 1024;
const uint16_t SCREEN_HEIGHT = 768;
const uint16_t OSC_TRANSMIT_OUTPUT_BUFFER_SIZE = 1024;

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

void klangstrom_arduino_event_transmit(EVENT_TYPE pEvent, float *data);
void klangstrom_arduino_data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength);
void klangstrom_arduino_sim_transmit(std::vector<float> &pData);

#endif /* klangstrom_arduino_sdl_h */
 