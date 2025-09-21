#ifdef SKETCH_AUDIO_OSCILLATOR_PASSTHROUGH
#include "Umfeld.h"

/**
* this example demonstrates how to set up the audio code and generate a sine wave sound.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "MWavetable.h"

float      wavetable[512];
MWavetable oscillator_left{wavetable, 512, 48000};
MWavetable oscillator_right{wavetable, 512, 48000};

AudioDevice* audiodevice;

float osc_frequency     = 440.0f;
bool  audiocodec_paused = false;

void setup() {
    system_init();

    MWavetable::fill(wavetable, 512, MWavetable::WAVEFORM_SINE);
    oscillator_left.set_amplitude(0.1f);
    oscillator_right.set_amplitude(0.15f);

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

    audiodevice_pause(audiodevice);
    audiodevice_resume(audiodevice);
}

void loop() {
    // osc_frequency += 0.0001f;
    osc_frequency += 10.0f;
    if (osc_frequency > 880.0f) {
        osc_frequency = 220.0f;
    }
    oscillator_left.set_frequency(osc_frequency);
    oscillator_right.set_frequency(osc_frequency * 0.495f);
    // console_println("frequency: %f", osc_frequency);

    if ((audiocodec_paused = !audiocodec_paused)) {
        audiodevice_pause(audiodevice);
    } else {
        audiodevice_resume(audiodevice);
    }

    delay(500);
}

void audioblock(AudioBlock* audio_block) {
    // console_once("audioblock block size     : ", audio_block->block_size);
    // console_once("audioblock input channels : ", audio_block->input_channels);
    // console_once("audioblock output channels: ", audio_block->output_channels);
    // console_once("system block size         : ", umfeld::a->buffer_size);
    // console_once("system input channels     : ", umfeld::a->input_channels);
    // console_once("system output channels    : ", umfeld::a->output_channels);

    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = oscillator_left.process();
        audio_block->output[1][i] = oscillator_right.process();
        // audio_block->output[0][i] = audio_block->input[0][i] + oscillator_left.process();
        // audio_block->output[1][i] = audio_block->input[0][i] + oscillator_right.process();
    }
}
#endif