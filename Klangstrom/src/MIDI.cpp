/*
* Klangstrom
*
* This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdint>

#include "MIDI.h"
#include "Console.h"


#ifdef __cplusplus
extern "C" {
#endif

static Callback_1_SERIALDEVICEPTR callback_midi = nullptr;
static SerialDevice*              fSerialDevice = nullptr;
static KlangstromMIDIParser*      fMIDIParser;

/* -------------------------------------------------------------------------------------------------------------- */
/* --- DEFAULT CALLBACKS ---------------------------------------------------------------------------------------- */

WEAK void midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) { console_println("midi_note_on"); }
WEAK void midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity) { console_println("midi_note_off"); }

// TODO add other callbacks

WEAK void midi_event(SerialDevice* serial_device) {
    for (uint16_t i = 0; i < serial_device->length; i++) {
        fMIDIParser->parse(serial_device->data[i]);
        //        uint8_t msg = fMIDIParser->parse(serial_device->data[i]);
        //        if (msg != 0) {
        //            console_println("MIDI event: %i", msg);
        //        }
    }
    //        console_print("MIDI event: ");
    //        for (int i = 0; i < serial_device->length; ++i) {
    //            console_print("0x%02X, ", serial_device->data[i]);
    //        }
    //        console_println("");
}

/* -------------------------------------------------------------------------------------------------------------- */

KlangstromMIDIParser* midi_parser() { return fMIDIParser; }

static void serial_to_midi(SerialDevice* serial_device) {
    if (serial_device->device_id == fSerialDevice->device_id && // TODO might be a bit much checking
        serial_device == fSerialDevice &&
        callback_midi) {
        callback_midi(serial_device);
    }
}
bool midi_init() {
    fSerialDevice = serialdevice_create(SERIAL_DEVICE_TYPE_IDC_00);
    //    fSerialDevice                  = serialdevice_create(SERIAL_DEVICE_TYPE_MIDI);
    fSerialDevice->callback_serial = serial_to_midi;
    callback_midi                  = midi_event;
    fMIDIParser                    = new KlangstromMIDIParser();

    /* register default callbacks */
#ifndef KLST_DISABLE_REGISTER_DEFAULT_CALLBACKS
    fMIDIParser->register_note_on(midi_note_on);
    fMIDIParser->register_note_off(midi_note_off);
#endif // KLST_DISABLE_REGISTER_DEFAULT_CALLBACKS
    return true;
}

void midi_send(const uint8_t* data, uint16_t length) {
    serialdevice_send(fSerialDevice, data, length);
}

void midi_deinit() {
    serialdevice_delete(fSerialDevice);
    fSerialDevice = nullptr;
}

static void parse_data(KlangstromMIDIParser& pMIDIParser, uint8_t* data, const uint8_t DATA_LENGTH) {
    // For commands of 0xf0 and greater (Both SYS and RS names)
    constexpr char midi_sysmsg_names[][7] = {
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
    constexpr char midi_msg_names[][8] = {
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
                console_print("0x%02X", (int) msg);
                console_print(" CH ");
                console_print("%i", pMIDIParser.parser().msg_channel(msg));
                console_print(": ");
                console_print(midi_msg_names[(msg >> 4) & 0x07]);
                console_print(" ");
                if (pMIDIParser.parser().msg_value(msg) == MIDIParser::PITCH_BEND) {
                    console_println("0x%02X", pMIDIParser.parser().get_int14());
                } else {
                    console_print("0x%02X", pMIDIParser.get_dataA());
                    console_print(", ");
                    console_println("0x%02X", pMIDIParser.get_dataB());
                }
                console_println("");
            } else if (msg != MIDIParser::RT_CLOCK) {
                console_print("SYS: ");
                console_print(midi_sysmsg_names[msg & 0x0f]);
                console_println("");
            }
        }
    }
}

#ifdef __cplusplus
}
#endif
