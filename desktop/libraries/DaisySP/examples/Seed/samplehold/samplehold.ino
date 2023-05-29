#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Oscillator osc;
static Metro      tick;
static WhiteNoise noise;
static AdEnv      env;
static SampleHold sh;

float osc_out = 0;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  osc_out, env_out;
    for (size_t i = 0; i < size; i++) {
        uint8_t trig = tick.Process();
        float   newsample =
            sh.Process(trig, noise.Process() * 500 + 500, sh.MODE_SAMPLE_HOLD);

        // When the metro ticks, trigger the envelope to start.
        if (trig) {
            env.Trigger();
            tick.SetFreq(newsample / 100 + 1);
            osc.SetFreq(newsample);
        }

        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process();
        osc.SetAmp(env_out);
        osc_out = osc.Process();

        out[0][i] = osc_out;
        out[1][i] = osc_out;
    }
}

void setup() {
    env.Init(sample_rate);
    osc.Init(sample_rate);
    tick.Init(1, sample_rate);

    // set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 0.01);
    env.SetTime(ADENV_SEG_DECAY, 0.1);
    env.SetMin(0.0);
    env.SetMax(0.25);
    env.SetCurve(0.5);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    noise.Init();
}

void loop() {}
