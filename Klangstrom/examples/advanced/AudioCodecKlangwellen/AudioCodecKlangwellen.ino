/**
 * this example demonstrates how to setup the audio code and generate a sine wave sound with the klangwellen library.
 */

#define KLANGWELLEN_WAVETABLE_INTERPOLATE_SAMPLES 1

#include "FMSynthesis.h"
#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "Wavetable.h"
#include "OscillatorFunction.h"
#include "Noise.h"

using namespace klangwellen;

float              wavetable[512];
Wavetable          oscillator_left{wavetable, 512, 48000};
OscillatorFunction oscillator_right{};
FMSynthesis        fm_synthesis{512, 48000};
Noise              noise;
SimplexNoise       simplex_noise;
AudioDevice*       audiodevice;

float osc_frequency     = 440.0f;
bool  audiocodec_paused = false;

void setup() {
    system_init();

    Wavetable::fill(wavetable, 512, KlangWellen::WAVEFORM_SAWTOOTH);
    oscillator_left.set_amplitude(0.5f);

    oscillator_right.set_amplitude(0.5f);
    oscillator_right.set_waveform(KlangWellen::WAVEFORM_SINE);

    fm_synthesis.set_amplitude(0.5f);
    fm_synthesis.get_carrier()->set_frequency(110);
    fm_synthesis.get_modulator()->set_waveform(KlangWellen::WAVEFORM_TRIANGLE);
    fm_synthesis.get_modulator()->set_frequency(55);

    noise.set_type(KlangWellen::NOISE_WHITE);
    simplex_noise.set_step(0.01);

    // long init section ...
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    audiodevice               = audiodevice_init_audiocodec(&audioinfo);
    if (audiodevice->audioinfo->device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    // ... or for short with default values and auto start
    // audiodevice = system_init_audiocodec();
}

void loop() {
    // osc_frequency += 10.0f;
    // if (osc_frequency > 880.0f) {
    //     osc_frequency = 220.0f;
    // }
    // oscillator_left.set_frequency(osc_frequency + noise.process() * 100);
    // oscillator_right.set_frequency(osc_frequency * 0.49f);
    console_println("frequency: %f", osc_frequency);

    fm_synthesis.get_carrier()->set_frequency(osc_frequency);
    fm_synthesis.get_modulator()->set_frequency(sin(osc_frequency / 205.0f * TWO_PI) * 10 + 31);

    delay(250);
}

void audioblock(AudioBlock* audio_block) {
    oscillator_left.set_frequency(osc_frequency + simplex_noise.process() * 200);
    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = KlangWellen::clamp(oscillator_left.process(), -1, 1) * 0.5f;
        audio_block->output[1][i] = audio_block->output[0][i];
        // audio_block->output[1][i] = oscillator_right.process();
    }
}
