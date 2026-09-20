/*
 * audio_process.c
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#include "audio_process.h"
#include <stdint.h>

static float audio_gain = 1.0f;

void process_audio_frame(uint16_t *in_buffer, uint16_t *out_buffer, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        /* 1. Extract 12-bit raw sample (0 to 4095) */
        uint16_t raw_adc = in_buffer[i];

        /* 2. Convert to normalized float range [-1.0f, +1.0f]
         *    assuming a mid-rail 1.65V DC bias (~2048 counts) */
        float float_sample = ((float)raw_adc - 2048.0f) / 2048.0f;

        float_sample = tremolo_process_samp(float_sample);

        /* 3. Execute DSP Algorithm (Hardware FPU acceleration) */
        float_sample = float_sample * audio_gain;

        /* Soft-clipping protection */
        if (float_sample > 1.0f)  float_sample = 1.0f;
        if (float_sample < -1.0f) float_sample = -1.0f;

        /* 4. Convert float back to 12-bit unsigned output (0 to 4095) */
        out_buffer[i] = (uint16_t)((float_sample * 2048.0f) + 2048.0f);
    }
}
