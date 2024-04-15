#include "KlangWellen.h"
#include "Klangstrom.h"
#include "SAM.h"

using namespace klangstrom;
using namespace klangwellen;

int8_t fBuffer[48000];
SAM    fSAM_left(fBuffer, 48000);
SAM    fSAM_right(48000);

void setup() {
    Serial.begin(115200);
    Serial.println("------");
    Serial.println("06.SAM");
    Serial.println("------");

    fSAM_right.set_speed(120);
    fSAM_right.set_throat(100);
    beats_per_minute(60);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fSAM_left.speak(String(beat_counter).c_str());
    if (beat_counter % 2 == 0) {
        fSAM_right.speak_to_buffer(String(beat_counter).c_str());
    } else {
        fSAM_right.speak_from_buffer();
    }
}

void audioblock(float** input_signal, float** output_signal) {
    fSAM_right.process(output_signal[LEFT]);
    fSAM_left.process(output_signal[RIGHT]);
    KlangWellen::mult(output_signal[LEFT], 0.25);
    KlangWellen::mult(output_signal[RIGHT], 0.25);
}
