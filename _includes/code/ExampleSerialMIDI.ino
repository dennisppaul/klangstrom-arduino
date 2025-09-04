#include "Klangstrom.h"
#include "MIDIParser.h"

using namespace klangstrom;

MIDIParser fMIDIParser00;
MIDIParser fMIDIParser01;

void configure() {
    option(KLST_OPTION_SERIAL_00_BAUD_RATE, 31250);
    option(KLST_OPTION_SERIAL_01_BAUD_RATE, 31250);
}

void setup() {
    Serial.begin(115200);
    Serial.println("---");
    Serial.println("Example Serial MIDI");
    Serial.println("---");
    Serial.print("Serial 00: ");
    Serial.println(get_option(KLST_OPTION_SERIAL_00_BAUD_RATE));
    Serial.print("Serial 01: ");
    Serial.println(get_option(KLST_OPTION_SERIAL_01_BAUD_RATE));
    Serial.println();

    /* test pre-recorded data */
    MIDIParser    mMIDIParserTEST;
    const uint8_t DATA_LENGTH       = 29;
    uint8_t       data[DATA_LENGTH] = {
        248,
        144,
        60,
        64,
        248,
        248,
        248,
        128,
        60,
        64,
        248,
        248,
        248,
        144,
        60,
        64,
        248,
        248,
        248,
        128,
        60,
        64,
        252,
        240,
        127,
        127,
        6,
        1,
        247};
    parse_data(mMIDIParserTEST, data, DATA_LENGTH);
    event_receive(2, (void*)data);

    beats_per_minute(120);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    Serial.print("event_receive: ");
    Serial.println(event);
}

void beat(uint32_t beat) {
    LED(LED_07, LED_TOGGLE);
    LED(LED_08, LED_TOGGLE);
    uint8_t data[3] = {0x90, 0x3c, 0x40};
    data_transmit(SERIAL_00, data, 3);
}

void loop() {
    delay(10);
}

void parse_data(MIDIParser& pMIDIParser, uint8_t* data, const uint8_t DATA_LENGTH) {
    // For commands of 0xf0 and greater (Both SYS and RS names)
    char const midi_sysmsg_names[][7] = {
        "SYSEX ",
        "TMCODE",
        "SNGPOS",
        "SNGSEL",
        "      ",
        "      ",
        "TUNE  ",
        "ENDEX ",
        "CLOCK ",
        "      ",
        "START ",
        "CONT  ",
        "STOP  ",
        "      ",
        "SENSE ",
        "RESET ",
    };

    // Otherwise, these are channel messages
    char const midi_msg_names[][8] = {
        "NOTOFF ",
        "NOTEON ",
        "ATOUCH ",
        "CC CHG ",
        "PRGCHG ",
        "CTOUCH ",
        "P BEND ",
    };

    for (int i = 0; i < DATA_LENGTH; i++) {
        uint8_t msg = pMIDIParser.parse(data[i]);
        if (msg != 0) {
            if (msg < MIDIParser::SYSEX) {
                Serial.print((int)msg, HEX);
                Serial.print(" CH ");
                Serial.print(pMIDIParser.msg_channel(msg));
                Serial.print(": ");
                Serial.print(midi_msg_names[(msg >> 4) & 0x07]);
                Serial.print(' ');
                if (pMIDIParser.msg_value(msg) == MIDIParser::PITCH_BEND) {
                    Serial.print(pMIDIParser.get_int14(), HEX);
                } else {
                    Serial.print(pMIDIParser.get_dataA(), HEX);
                    Serial.print(", ");
                    Serial.print(pMIDIParser.get_dataB(), HEX);
                }
                Serial.println("");
            } else if (msg != MIDIParser::RT_CLOCK) {
                Serial.print("SYS: ");
                Serial.print(midi_sysmsg_names[msg & 0x0f]);
                Serial.println("");
            }
        }
    }
}

void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {
    Serial.print("+++ data_receive: ");
    if (receiver == SERIAL_00) {
        Serial.print("SERIAL_00: ");
        parse_data(fMIDIParser00, data, length);
        for (int i = 0; i < length; i++) {
            LED(LED_00, LED_TOGGLE);
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println();
    }
    if (receiver == SERIAL_01) {
        Serial.print("SERIAL_01: ");
        parse_data(fMIDIParser01, data, length);
        for (int i = 0; i < length; i++) {
            LED(LED_15, LED_TOGGLE);
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println();
    }
}
