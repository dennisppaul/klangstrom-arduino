/**
 * this example demonstrates how to use an SD card to stream audio data to the audio codec.
 * it is less straightforward than expected. a few things need to be considered:
 * - the buffer must be updated from outside the audioblock callback
 * - the audio data must be read in big chunks ( e.g 512*32 bytes ) due to the SD card overhead
 *
 * on a *normal* SD card the data rate is about 1800 KB/s if the file is read in one big chunk
 * ( and even 10667 KB/s if fast internal RAM is used ). if the file  is read in chunks of
 * 512 bytes the data rate is about 232 KB/s. at an audio rate of 48000 Hz, the data rate for
 * a single 32-bit channel is 187.5 KB/s.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "SDCard.h"
#include "AudioDevice.h"
#include "AudioStreamFloat32.h"

AudioDevice*        audiodevice;
AudioStreamFloat32* audio_stream;

const uint16_t buffer_multiple     = 32;
uint16_t       buffer_read_counter = 0;

#define USE_FAST_INTERNAL_RAM
#ifdef USE_FAST_INTERNAL_RAM
__attribute__((section(".bss")))
#endif
uint8_t read_file_buffer[131072];

void setup() {
    system_init();
    console_init();
    sdcard_init();

    while (!sdcard_detected()) {
        console_println("SD card not detected");
        delay(1000);
    }

    sdcard_status();
    sdcard_mount();
    sdcard_file_open("TEILCHEN.RAW", FILE_READ_ONLY);

    /* read entire file */
    uint32_t time;
    console_println("start reading entire file from SD card ...");
    time = system_get_ticks();
    sdcard_file_read(read_file_buffer, 131072);
    console_println("... done in %i ms", (system_get_ticks() - time)); // 71 ms ≈ 1803 KB/s OR 12 ms ≈ 10667 KB/s
    sdcard_file_seek(0);

    /* read entire file in chunks */
    static const uint32_t buffer_size = 512;
    console_println("start reading entire file in chunks of %i from SD card ...", buffer_size);
    time = system_get_ticks();
    while (!sdcard_file_eof()) {
        uint8_t buffer[buffer_size];
        sdcard_file_read(buffer, buffer_size);
        console_print(".");
    }
    sdcard_file_seek(0);
    console_println("!");
    console_println("... done in %i ms", (system_get_ticks() - time)); // 552 ms ≈ 232 KB/s

    audiodevice = system_init_audiocodec();
    audiodevice_pause(audiodevice);

    audio_stream = new AudioStreamFloat32(audiodevice->audioinfo->block_size * buffer_multiple);
    audio_stream->update();

    audiodevice_resume(audiodevice);
}

bool mUpdateBuffer = false;

void loop() {
    if (mUpdateBuffer) {
        mUpdateBuffer = false;
        audio_stream->update();
    }
}

void audioblock(AudioBlock* audio_block) {
    // at 48000 Hz audiorate a data rate of 48000 * 4 = 192000 bytes or 187.5KB per second is required by the SD card
    const int mBufferOffset = audio_block->block_size * buffer_read_counter;
    for (int i = 0; i < audio_block->block_size; ++i) {
        audio_block->output[0][i] = audio_stream->buffer()[i + mBufferOffset];
        audio_block->output[1][i] = audio_stream->buffer()[i + mBufferOffset];
    }
    buffer_read_counter++;
    if (buffer_read_counter >= buffer_multiple) {
        buffer_read_counter = 0;
        mUpdateBuffer       = true;
    }
}
