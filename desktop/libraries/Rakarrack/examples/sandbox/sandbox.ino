#include "Klangstrom.h"
#include "Rakarrack.h"

float buffer_left[KLANG_SAMPLES_PER_AUDIO_BLOCK];
float buffer_right[KLANG_SAMPLES_PER_AUDIO_BLOCK];

void audioblock(float** input_buffer, float** output_buffer) {
    for (size_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_buffer[LEFT][i]  = input_buffer[LEFT][i];
        output_buffer[RIGHT][i] = input_buffer[RIGHT][i];
    }
}

void setup() {}

void loop() {}
