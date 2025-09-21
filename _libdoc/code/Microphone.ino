/**
 * this example demonstrates how to use the on-board microphone.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Microphone.h"

#include <algorithm>

float peak0 = 0.0;
float peak1 = 0.0;
float sum0  = 0.0;
float sum1  = 0.0;

void setup() {
    system_init();
    console_init();
    microphone_init();
}

void loop() {
    console_clear();
    console_println("ENERGY: %05.3f | %05.3f", sum0, sum1);
    console_println("PEAK  : %05.3f | %05.3f", peak0, peak1);
    delay(100);
}

void audioblock(AudioBlock* audio_block) {
    sum0 = 0.0;
    sum1 = 0.0;
    peak0 *= 0.995f;
    peak1 *= 0.995f;
    for (int i = 0; i < audio_block->block_size; ++i) {
        const float sample0 = audio_block->input[0][i];
        const float sample1 = audio_block->input[1][i];
        sum0 += abs(sample0);
        sum1 += abs(sample1);
        peak0 = std::max(peak0, abs(sample0));
        peak1 = std::max(peak1, abs(sample1));
    }
    sum0 /= audio_block->block_size;
    sum1 /= audio_block->block_size;
}
