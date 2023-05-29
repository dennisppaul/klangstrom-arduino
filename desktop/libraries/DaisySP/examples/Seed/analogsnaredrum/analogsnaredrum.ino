#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

float sample_rate = KLANG_AUDIO_RATE;

AnalogSnareDrum sd;
Metro tick;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
  for (size_t i = 0; i < size; i++) {
    bool t = tick.Process();
    if (t) {
      sd.SetDecay(random() / (float)RAND_MAX);
      sd.SetSnappy(random() / (float)RAND_MAX);
      sd.SetTone(.8f * random() / (float)RAND_MAX);
    }
    out[0][i] = out[1][i] = sd.Process(t);
  }
}

void setup() {
  tick.Init(2.f, sample_rate);

  sd.Init(sample_rate);
}

void loop() {}
