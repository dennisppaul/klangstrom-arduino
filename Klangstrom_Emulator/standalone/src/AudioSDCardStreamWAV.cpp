#ifdef SKETCH_AUDIO_WAV_STREAM
/**
 * this example demonstrates how to stream a WAV file from an SD card.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "SDCard.h"
#include "WAV.h"
#include "Key.h"

Key*   key_left              = nullptr;
int    sample_buffer_size    = 0;
float* sample_buffer         = nullptr;
int    sample_buffer_counter = 0;
bool   update_audio_block    = false;

void filter_wav_files(std::vector<std::string>& result_files) {
    std::vector<std::string> files;
    std::vector<std::string> directories;
    sdcard_list("", files, directories);

    for (std::string file: files) {
        if (file.substr(file.find_last_of(".") + 1) == "WAV") {
            // NOTE to also catch lower case add:
            //      || file.substr(file.find_last_of(".") + 1) == "wav"
            console_println("found WAV file: %s", file.c_str());
            result_files.push_back(file);
        } else {
            console_println("not a WAV file: %s", file.c_str());
        }
    }
}

void open_file_and_load_header(const std::string& filename) {
    if (wav_load_header(filename)) {
        console_println("%i samples in WAV file", wav_num_sample_frames());
        if (wav_is_open()) {
            console_println("WAV file is now ready to be played");
        }
    } else {
        console_println("failed to load WAV header");
    }
}

void load_all_samples() {
    sample_buffer_size = wav_num_sample_frames();
    sample_buffer      = new float[sample_buffer_size];
    wav_load_sample_frames(sample_buffer, WAV_ALL_SAMPLES);
}

void setup() {
    system_init();
    console_init();
    key_left = key_create(KEY_LEFT);
    sdcard_init();

    while (!sdcard_detected()) {
        console_println("SD card not detected");
        delay(1000);
    }

    sdcard_status();
    sdcard_mount();

    std::vector<std::string> wav_files;
    filter_wav_files(wav_files);

    if (wav_files.size() > 0) {
        console_println("found %i WAV files", wav_files.size());
        for (std::string file: wav_files) {
            console_println(" - %s", file.c_str());
        }
        console_println("loading first WAV file: %s", wav_files[0].c_str());
        open_file_and_load_header(wav_files[0]);
        load_all_samples();
    }

    system_init_audiocodec();
}

void loop() {
    if (update_audio_block) {
        update_audio_block = false;
        if (wav_is_open()) {
            // load_samples(...);
        }
    }
}

void audioblock(AudioBlock* audio_block) {
    if (sample_buffer == nullptr) {
        return;
    }
    for (int i = 0; i < audio_block->block_size; ++i) {
        float sample = 0.0f;
        if (sample_buffer_counter < sample_buffer_size) {
            sample_buffer_counter++;
            sample = sample_buffer[sample_buffer_counter];
        }
        for (int c = 0; c < audio_block->output_channels; ++c) {
            audio_block->output[c][i] = sample;
        }
    }
    update_audio_block = true;
}

void key_event(const Key* key) {
    if (key->device_id == key_left->device_id) {
        if (key->pressed) {
            sample_buffer_counter = 0;
        }
    }
}
#endif
