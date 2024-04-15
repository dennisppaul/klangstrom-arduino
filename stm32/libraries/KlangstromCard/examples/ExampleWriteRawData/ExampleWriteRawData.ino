#include "KlangNodes.hpp"
#include "Klangstrom.h"
#include "KlangstromCard.h"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;
float           mBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
bool            fBufferUpdated = false;

void print_file_list(vector<String>& pFiles) {
    if (pFiles.size() > 0) {
        for (uint16_t i = 0; i < pFiles.size(); i++) {
            Serial.print(i);
            Serial.print("\t");
            Serial.println(pFiles[i]);
        }
        Serial.println();
    } else {
        Serial.println("--- no files found");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("-------------------");
    Serial.println("ExampleWriteRawData");
    Serial.println("-------------------");

    bool           mOpenCardError = Card.begin();
    vector<String> mFiles;
    if (!mOpenCardError) {
        Serial.println("--- reading file list");
        Card.get_file_list(mFiles);
        print_file_list(mFiles);
    } else {
        Serial.println("--- opening card failed");
        return;
    }

    Klang::lock();
    Klang::connect(mVCO, mDAC);
    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    mVCO.set_amplitude(0.5);
    Klang::unlock();

    bool mOpenFileError = Card.create_file("audiodata.raw");
    if (!mOpenFileError) {
        Serial.println("--- writing to file");
    } else {
        Serial.println("--- opening file failed");
        return;
    }
}

void beat(uint32_t pBeat) {
    mVCO.set_frequency(DEFAULT_FREQUENCY + (pBeat % 16) * 20);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
        case EVENT_KEY_PRESSED:
        case EVENT_MOUSE_PRESSED:
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
        case EVENT_KEY_RELEASED:
        case EVENT_MOUSE_RELEASED:
            Serial.println("--- closing file");
            Card.close();
            break;
    }
}

void loop() {
    if (fBufferUpdated) {
        Card.write((uint8_t*)mBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK * 4, true);
        fBufferUpdated = false;
    }
}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
    if (!fBufferUpdated) {
        KLANG_COPY_AUDIO_BUFFER(mBuffer, output_signal[LEFT]);
        fBufferUpdated = true;
    }
}
