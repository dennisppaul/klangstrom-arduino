#include "KlangWellen.h"

/* xorshift32 ( ref: https://en.wikipedia.org/wiki/Xorshift ) */
uint32_t klangwellen::KlangWellen::x32Seed = 23;
uint32_t klangwellen::KlangWellen::xorshift32() {
    x32Seed ^= x32Seed << 13;
    x32Seed ^= x32Seed >> 17;
    x32Seed ^= x32Seed << 5;
    return x32Seed;
}

/**
 * returns a random number between 0 ... 1
 */
float klangwellen::KlangWellen::random_normalized() {
    // TODO replace with mapping, without division
    return ((float)xorshift32() / UINT32_MAX);
}

float klangwellen::KlangWellen::random() {
    // TODO replace with mapping, without division
    return ((float)xorshift32() / UINT32_MAX) * 2 - 1;
}

uint32_t klangwellen::KlangWellen::millis_to_samples(float pMillis, float pSamplingRate) {
    return (uint32_t)(pMillis / 1000.0f * pSamplingRate);
}

uint32_t klangwellen::KlangWellen::millis_to_samples(float pMillis) {
    return (uint32_t)(pMillis / 1000.0f * (float)klangwellen::KlangWellen::DEFAULT_SAMPLING_RATE);
}
