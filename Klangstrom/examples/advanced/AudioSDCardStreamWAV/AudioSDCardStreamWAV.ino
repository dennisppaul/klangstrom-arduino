/**
 * this example demonstrates load and play a WAV file from an SD card.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"
#include "SDCard.h"
#include "WAV.h"
#include "Key.h"

AudioDevice* audiodevice = nullptr;
Key*         key_left    = nullptr;
// int                sample_buffer_size              = 0;
// float*             sample_buffer                   = nullptr;
// int                sample_buffer_counter           = 0;
bool               update_audio_block              = false;
constexpr uint16_t sample_buffer_multiple          = 64;
uint16_t           sample_buffer_read_counter      = 0;
float*             sample_audio_stream_buffer      = nullptr;
uint32_t           sample_audio_stream_buffer_size = 0;

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

void fill_audio_stream_buffer() {
    // TODO could apply double buffering
    wav_load_sample_frames(sample_audio_stream_buffer, sample_audio_stream_buffer_size);
}

void initialize_audio_stream_buffer() {
    sample_audio_stream_buffer_size = audiodevice->audioinfo->block_size * sample_buffer_multiple;
    console_println("sample_audio_stream_buffer_size %i", sample_audio_stream_buffer_size);
    sample_audio_stream_buffer = new float[sample_audio_stream_buffer_size];
    fill_audio_stream_buffer();
}

void setup() {
    system_init();
    console_init();

    key_left = key_create(KEY_LEFT);

    audiodevice = system_init_audiocodec();
    audiodevice_pause(audiodevice);

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
        // console_println("loading first WAV file: %s", wav_files[0].c_str());
        // load_header(wav_files[0]);
        open_file_and_load_header("TEILCHEN.WAV");
    }
    wav_looping(true);

    initialize_audio_stream_buffer();

    audiodevice_resume(audiodevice);
}

void loop() {
    if (update_audio_block) {
        update_audio_block = false;
        if (wav_is_open()) {
            console_println("update stream buffer");
            fill_audio_stream_buffer();
        }
    }
}

void audioblock(AudioBlock* audio_block) {
    if (sample_audio_stream_buffer == nullptr) {
        return;
    }
    // at 48000 Hz audiorate a data rate of 48000 * 4 = 192000 bytes or 187.5KB per second is required by the SD card
    const int mBufferOffset = audio_block->block_size * sample_buffer_read_counter;
    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = sample_audio_stream_buffer[i + mBufferOffset];
        audio_block->output[1][i] = sample_audio_stream_buffer[i + mBufferOffset];
    }
    sample_buffer_read_counter++;
    if (sample_buffer_read_counter >= sample_buffer_multiple) {
        sample_buffer_read_counter = 0;
        update_audio_block         = true;
    }
}

void key_event(const Key* key) {
    if (key->device_id == key_left->device_id) {
        if (key->pressed) {
            sample_buffer_read_counter = 0;
        }
    }
}
