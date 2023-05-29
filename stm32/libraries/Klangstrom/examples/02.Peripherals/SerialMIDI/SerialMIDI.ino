#include "Klangstrom.h"
#include "KlangstromMIDIParser.h"

using namespace klangstrom;

KlangstromMIDIParser fMIDI;

void configure() {
    option(KLST_OPTION_SERIAL_00_BAUD_RATE, 31250);
}

void setup() {
    Serial.begin(115200);
    Serial.println("----------");
    Serial.println("SerialMIDI");
    Serial.println("----------");

    beats_per_minute(120);

    fMIDI.register_song_position_pointer(song_position_pointer);
    fMIDI.register_clock_tick(clock_tick);
    fMIDI.register_note_off(midi_note_off);
    fMIDI.register_note_on(midi_note_on);
    fMIDI.register_event_receive(event_receive);

    {
        uint8_t data[3] = {0xF2, 0x20, 0x00};
        data_receive(SERIAL_00, data, 3);
    }
    {
        uint8_t data[1] = {0xF8};
        data_receive(SERIAL_00, data, 1);
    }
    {
        uint8_t data[3] = {0x8A, 0x3C, 0x00};
        data_receive(SERIAL_00, data, 3);
    }
    {
        uint8_t data[3] = {0x92, 0x3C, 0x40};
        data_receive(SERIAL_00, data, 3);
    }
}

void song_position_pointer(uint16_t position) {
    Serial.println("song_position_pointer");
}

void clock_tick() {
    Serial.println("clock_tick");
}

void midi_note_off(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    Serial.println("midi_note_off");
}

void midi_note_on(uint8_t channel, uint8_t pitch, uint8_t velocity) {
    Serial.println("midi_note_on");
}

void event_receive(const EVENT_TYPE event, const void* data) {
    Serial.print("event_receive: ");
    Serial.println(event);
    if (event == EVENT_MIDI) {
        Serial.print("channel: ");
        Serial.println(midi_event(data).channel);
        Serial.print("command: ");
        Serial.println(midi_event(data).command);
        Serial.print("a: ");
        Serial.println(midi_event(data).a);
        Serial.print("b: ");
        Serial.println(midi_event(data).b);
    }
}

void beat(uint32_t beat) {
    LED(LED_07, LED_TOGGLE);
    LED(LED_08, LED_TOGGLE);
    // uint8_t data[3] = {0x90, 0x3c, 0x40};
    // data_transmit(SERIAL_00, data, 3);
}

void loop() {
    delay(10);
}

void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {
    Serial.print("+++ data_receive: ");

    if (receiver == SERIAL_00) {
        Serial.println("SERIAL_00");
    }
    if (receiver == SERIAL_01) {
        Serial.println("SERIAL_01");
    }

    if (receiver == SERIAL_00 || receiver == SERIAL_01) {
        for (int i = 0; i < length; i++) {
            EVENT_TYPE mMessage = fMIDI.parse(data[i]);
            if (mMessage > MIDI_NONE) {
                Serial.print("+++ ");
                Serial.println(fMIDI.message_str(mMessage));
            }
            LED(LED_00, LED_TOGGLE);
        }
        Serial.println();
    }
}
