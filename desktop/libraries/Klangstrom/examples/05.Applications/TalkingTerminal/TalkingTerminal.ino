/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */
// klst-sketch -u --usb=HOST -b=KLST_SHEEP /Users/dennisppaul/Documents/dennisppaul/projects/klangstrom/git/klangstrom/klangstrom/platforms/klangstrom-arduino/stm32/libraries/Klangstrom/examples/05.Applications/TalkingTerminal/TalkingTerminal.ino

#define KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM

#include "Arduino.h"
#include "KlangNodes.hpp"
#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayCLI.h"
#include "KlangstromDisplayFont_7x10.h"
#include "KlangstromDisplayFont_8x12.h"
#include "KlangstromDisplayFont_8x8.h"
#include "USBHost.h"

using namespace klang;
using namespace klangstrom;

KlangstromDisplay    &Display = KlangstromDisplay::create();
KlangstromDisplayCLI &CLI     = KlangstromDisplayCLI::create(&Display, &Font_8x12);

NodeDAC             mDAC;
NodeTextToSpeechSAM mTTS(46160);

void setup() {
    Serial.begin(115200);
    Serial.println("----------------");
    Serial.println("Talking Terminal");
    Serial.println("----------------");

    USBHost.init();
    USBHost.register_key_pressed(key_pressed);
    USBHost.register_event_receive(event_receive);

    Display.begin();
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    Display.clear();

    CLI.set_write_char_callback(writeChar);
    CLI.set_on_command_callback(onCommand);
    CLI.add_binding({"clear", "clear screen", true, (void *)"", onClear});
    CLI.add_binding({"sing", "sing a text", true, (void *)"", onSing});
    CLI.color(255, 255, 255);
    CLI.background(0, 0, 0);
    CLI.clear();

    Klang::connect(mTTS, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
}

void loop() {
    USBHost.process();
    LED(LED_01, USBHost.is_keyboard_connected());
    CLI.process();
}

void beat(uint32_t beat) {
    LED(LED_00, LED_TOGGLE);
    if (beat % 2 == 0) {
        mTTS.speak_from_buffer();
    }
    LED(LED_01, LED_TOGGLE);
}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void key_pressed(uint8_t key) {
    if (key != KEY_BACKSPACE && key != KEY_TAB) {
        CLI.receive_char(key);
        LED(LED_02, true);
    }
}

void event_receive(EVENT_TYPE event, const void *data) {
    if (event == EVENT_KEYBOARD) {
        const uint8_t keycode = keyboard_event(data).keycodes[0];
        switch (keycode) {
            case KEYCODE_ARROW_DOWN:
                CLI.send_history_forward();
                break;
            case KEYCODE_ARROW_UP:
                CLI.send_history_back();
                break;
            case KEYCODE_BACKSPACE:
                CLI.send_delete_character();
                break;
            case KEYCODE_TAB:
                CLI.send_complete_command();
                break;
            default:
                if (keycode > KEYCODE_NONE) {
                    Serial.print("EVENT_KEYBOARD  :");
                    Serial.print(" CODE(");
                    Serial.print(keycode);
                    Serial.print(")");
                    Serial.println();
                }
                break;
        }
        // Serial.print(")");
        // Serial.print(" SHIFT_LEFT: ");
        // Serial.print(keyboard_event(data).SHIFT_LEFT);
        // Serial.print(" GUI_LEFT: ");
        // Serial.print(keyboard_event(data).GUI_LEFT);
    }
}

/* --- CLI --- */

void onClear(EmbeddedCli *cli, char *args, void *context) {
    CLI.clear();
}

void onSing(EmbeddedCli *cli, char *args, void *context) {
    if (CLI.get_token_count(args) > 0) {
        const char *mText = CLI.get_token(args, 1);
        CLI.print("SING:");
        CLI.println(mText);

        mTTS.set_pitch(64);
        mTTS.set_throat(128);
        mTTS.set_speed(100);
        mTTS.set_pitch(96);
        mTTS.speak_buffer(mText);
    }
}

void writeChar(EmbeddedCli *embeddedCli, char c) {
    CLI.print(c);
}

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command) {
    const char *mText = command->name;
    CLI.print("SAY :");
    CLI.println(mText);

    mTTS.set_pitch(64);
    mTTS.set_throat(128);
    mTTS.set_speed(72);
    mTTS.set_mouth(128);
    mTTS.speak_buffer(mText);
}
