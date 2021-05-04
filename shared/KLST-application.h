//
//  KLST-application.h
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#ifndef KLST_application_h
#define KLST_application_h

/* klangstrom application */

//@todo(reintroduce `SIGNAL_TYPE` aka `float` + `EVENT_TYPE` + `KLANG_SAMPLES_PER_AUDIO_BLOCK`)
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 512

#ifdef __cplusplus
namespace klangstrom{};

WEAK void setup() {}
WEAK void loop() {}
WEAK void beat(uint32_t pBeat) {}
WEAK void event_receive(const uint8_t event, const float* data) {} // @todo(as cool as it is to use *raw* arrays, consider using `std::array<float, ?>`)
WEAK void data_receive(uint8_t* data, uint8_t length) {}
WEAK void data_receive(const uint8_t sender, uint8_t* data, uint8_t length) {}

WEAK void audioblock(float* pLeftTX, float* pRightTX, float* pLeftRX, float* pRightRX) {
    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        pLeftTX[i] = 0.0;
        pRightTX[i] = 0.0;
    }
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* KLST_application_h */
