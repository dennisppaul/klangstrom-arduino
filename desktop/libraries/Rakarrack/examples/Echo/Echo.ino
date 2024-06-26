#include "Klangstrom.h"
#include "Rakarrack.h"

float buffer_left[KLANG_SAMPLES_PER_AUDIO_BLOCK];
float buffer_right[KLANG_SAMPLES_PER_AUDIO_BLOCK];
Echo  echo{buffer_left, buffer_right};

void audioblock(float** input_buffer, float** output_buffer) {
    echo.out(input_buffer[LEFT], input_buffer[RIGHT]);
    for (size_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_buffer[LEFT][i]  = buffer_left[i];
        output_buffer[RIGHT][i] = buffer_right[i];
    }
}

void setup() {
    echo.setpreset(4);
}

void loop() {}
