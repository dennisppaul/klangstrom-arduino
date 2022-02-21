/**
    ExampleDataSendAndReceive

    this example demonstrates how to transmit and receive data via the built in
    serial ports. the examples implements a simple protocol that transmits
    enocder events on serial port 00 and receives events on serial port 01.

    pressing any encoder button triggers a sound. rotating the encoders changes
    the frequency.

    note that this examles allows to control on board with another board. in
    order to do so a special IDC cable is required to connect the two boards.

*/

// @todo(not tested in desktop simulator)

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable    mVCO;
NodeADSR            mADSR;
NodeDAC             mDAC;

static const uint8_t M_ADSR_START   = 0x00;
static const uint8_t M_ADSR_STOP    = 0x01;
static const uint8_t M_FREQ_INC     = 0x02;
static const uint8_t M_FREQ_DEC     = 0x03;

static const uint8_t M_FREQ_COUNTER = 12;

float mBaseFreq = 220;
int mFreqCounter = 0;

void setup() {
    Klang::lock();
    Klang::connect(mVCO,    mADSR);
    Klang::connect(mADSR,   mDAC);
    mVCO.set_frequency(mBaseFreq);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    /*  note that the iteration time for each loop needs to be kept short because all
        events ( e.g encoder, serial ) are processed right after loop is finished.
    */
    delay(10);
}

void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {
    if (receiver == SERIAL_01) {
        for (int i = 0; i < length; i++) {
            parse_command(data[i]);
        }
        led_toggle(LED_01);
    }
}

void parse_command(uint8_t pCommand) {
    if (pCommand == M_ADSR_START) {
        mADSR.start();
    } else if (pCommand == M_ADSR_STOP) {
        mADSR.stop();
    } else if (pCommand == M_FREQ_INC) {
        change_frequency(true);
        update_frequency();
    } else if (pCommand == M_FREQ_DEC) {
        change_frequency(false);
        update_frequency();
    }
}

void change_frequency(bool pIncrease) {
    if (pIncrease) {
        mFreqCounter ++;
        mFreqCounter %= M_FREQ_COUNTER;
    } else {
        if (mFreqCounter == 0) {
            mFreqCounter = M_FREQ_COUNTER;
        }
        mFreqCounter--;
    }
}

void update_frequency() {
    const float mFreq = mBaseFreq + mBaseFreq * mFreqCounter * 0.5;
    mVCO.set_frequency(mFreq);
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            transmit_command(M_ADSR_START);
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            transmit_command(M_ADSR_STOP);
            led_toggle(LED_00);
            break;
        case EVENT_ENCODER_ROTATED:
            parse_encoder(data[TICK] - data[PREVIOUS_TICK]);
            led_toggle(LED_02);
            break;
    }
}

void parse_encoder(float pDirection) {
    if (pDirection > 0) {
        transmit_command(M_FREQ_INC);
    } else if (pDirection < 0) {
        transmit_command(M_FREQ_DEC);
    }
}

void transmit_command(uint8_t pCommand) {
    uint8_t mData[1] = {pCommand};
    data_transmit(SERIAL_00, mData, 1);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
