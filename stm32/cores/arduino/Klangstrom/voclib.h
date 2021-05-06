/* Vocoder Library
* Voclib version 1.1 - 2019-02-16
*
* Philip Bennefall - philip@blastbay.com
*
* See the end of this file for licensing terms.
* The filter implementation was derived from public domain code found on musicdsp.org (see the section called "Filters" for more details).
*
* USAGE
*
* This is a single-file library. To use it, do something like the following in one .c file.
* #define VOCLIB_IMPLEMENTATION
* #include "voclib.h"
*
* You can then #include this file in other parts of the program as you would with any other header file.
*/

#ifndef VOCLIB_H
#define VOCLIB_H

#ifdef __cplusplus
extern "C" {
#endif

    /* COMPILE-TIME OPTIONS */

    /* The maximum number of bands that the vocoder can be initialized with (lower this number to save memory). */
#define VOCLIB_MAX_BANDS 96

    /* The maximum number of filters per vocoder band (lower this number to save memory). */
#define VOCLIB_MAX_FILTERS_PER_BAND 8

    /* PUBLIC API */

    typedef struct voclib_instance voclib_instance;

    /* Initialize a voclib_instance structure.
    *
    * Call this function to initialize the voclib_instance structure.
    * bands is the number of bands that the vocoder should use; recommended values are between 12 and 64.
    * bands must be between 4 and VOCLIB_MAX_BANDS (inclusive).
    * filters_per_band determines the steapness with which the filterbank divides the signal; a value of 6 is recommended.
    * filters_per_band must be between 1 and VOCLIB_MAX_FILTERS_PER_BAND (inclusive).
    * sample_rate is the number of samples per second in hertz, and should be between 8000 and 192000 (inclusive).
    * carrier_channels is the number of channels that the carrier has, and should be between 1 and 2 (inclusive).
    * Note: The modulator must always have only one channel.
    * Returns nonzero (true) on success or 0 (false) on failure.
    * The function will only fail if one or more of the parameters are invalid.
    */
    int voclib_initialize ( voclib_instance* instance, unsigned char bands, unsigned char filters_per_band, unsigned int sample_rate, unsigned char carrier_channels );

    /* Run the vocoder.
    *
    * Call this function continuously to generate your output.
    * carrier_buffer and modulator_buffer should contain the carrier and modulator signals respectively.
    * The modulator must always have one channel.
    * If the carrier has two channels, the samples in carrier_buffer must be interleaved.
    * output_buffer will be filled with the result, and must be able to hold as many channels as the carrier.
    * If the carrier has two channels, the output buffer will be filled with interleaved samples.
    * output_buffer may be the same pointer as either carrier_buffer or modulator_buffer as long as it can hold the same number of channels as the carrier.
    * The processing is performed in place.
    * frames specifies the number of sample frames that should be processed.
    * Returns nonzero (true) on success or 0 (false) on failure.
    * The function will only fail if one or more of the parameters are invalid.
    */
    int voclib_process ( voclib_instance* instance, const float* carrier_buffer, const float* modulator_buffer, float* output_buffer, unsigned int frames );

    /* Reset the vocoder sample history.
    *
    * In order to run smoothly, the vocoder needs to store a few recent samples internally.
    * This function resets that internal history. This should only be done if you are processing a new stream.
    * Resetting the history in the middle of a stream will cause clicks.
    */
    void voclib_reset_history ( voclib_instance* instance );

    /* Set the reaction time of the vocoder in seconds.
    *
    * The reaction time is the time it takes for the vocoder to respond to a volume change in the modulator.
    * A value of 0.03 (AKA 30 milliseconds) is recommended for intelligible speech.
    * Values lower than about 0.02 will make the output sound raspy and unpleasant.
    * Values above 0.2 or so will make the speech hard to understand, but can be used for special effects.
    * The value must be between 0.002 and 2.0 (inclusive).
    * Returns nonzero (true) on success or 0 (false) on failure.
    * The function will only fail if the parameter is invalid.
    */
    int voclib_set_reaction_time ( voclib_instance* instance, float reaction_time );

    /* Get the current reaction time of the vocoder in seconds. */
    float voclib_get_reaction_time ( const voclib_instance* instance );

    /* Set the formant shift of the vocoder in octaves.
    *
    * Formant shifting changes the size of the speaker's head.
    * A value of 1.0 leaves the head size unmodified.
    * Values lower than 1.0 make the head larger, and values above 1.0 make it smaller.
    * The value must be between 0.25 and 4.0 (inclusive).
    * Returns nonzero (true) on success or 0 (false) on failure.
    * The function will only fail if the parameter is invalid.
    */
    int voclib_set_formant_shift ( voclib_instance* instance, float formant_shift );

    /* Get the current formant shift of the vocoder in octaves. */
    float voclib_get_formant_shift ( const voclib_instance* instance );

    /* INTERNAL STRUCTURES */

    /* this holds the data required to update samples thru a filter. */
    typedef struct
    {
        float a0, a1, a2, a3, a4;
        float x1, x2, y1, y2;
    } voclib_biquad;

    /* Stores the state required for our envelope follower. */
    typedef struct
    {
        float coef;
        float history[4];
    } voclib_envelope;

    /* Holds a set of filters required for one vocoder band. */
    typedef struct
    {
        voclib_biquad filters[VOCLIB_MAX_FILTERS_PER_BAND];
    } voclib_band;

    /* The main instance structure. This is the structure that you will create an instance of when using the vocoder. */
    struct voclib_instance
    {
        voclib_band analysis_bands[VOCLIB_MAX_BANDS]; /* The filterbank used for analysis (these are applied to the modulator). */
        voclib_envelope analysis_envelopes[VOCLIB_MAX_BANDS]; /* The envelopes used to smooth the analysis bands. */
        voclib_band synthesis_bands[VOCLIB_MAX_BANDS * 2]; /* The filterbank used for synthesis (these are applied to the carrier). The second half of the array is only used for stereo carriers. */
        float reaction_time; /* In seconds. Higher values make the vocoder respond more slowly to changes in the modulator. */
        float formant_shift; /* In octaves. 1.0 is unchanged. */
        unsigned int sample_rate; /* In hertz. */
        unsigned char bands;
        unsigned char filters_per_band;
        unsigned char carrier_channels;
    };

#ifdef __cplusplus
}
#endif
#endif  /* VOCLIB_H */
