/*
 * audio_process.c
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#include "audio_process.h"
#include <stdint.h>

void audio_pipeline_init(AudioPipeline *pipeline, float sample_rate) {
	if (!pipeline) return;
	pipeline->master_gain = 1.0f;
	tremolo_init(&pipeline->tremolo, 0.8f, 7.0f, sample_rate);
}

/* Float-native processing for direct Python streaming & testing */
void audio_pipeline_process_float(AudioPipeline *pipeline, const float *in_buf, float *out_buf, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        float sample = in_buf[i];

        /* Effect Chain Execution */
        sample = tremolo_process_sample(&pipeline->tremolo, sample);
        sample = sample * pipeline->master_gain;

        /* Hard-clipping protection [-1.0, +1.0] */
        if (sample > 1.0f)  sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;

        out_buf[i] = sample;
    }
}

/* 12-bit ADC/DAC conversion wrapper (used by STM32 bare-metal main) */
void audio_pipeline_process_12bit(AudioPipeline *pipeline, const uint16_t *in_buf, uint16_t *out_buf, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        /* Convert 12-bit unsigned ADC (0-4095) to float [-1.0, +1.0] */
        float float_in = ((float)in_buf[i] - 2048.0f) / 2048.0f;
        float float_out = 0.0f;

        audio_pipeline_process_float(pipeline, &float_in, &float_out, 1);

        /* Convert float back to 12-bit DAC (0-4095) */
        out_buf[i] = (uint16_t)((float_out * 2048.0f) + 2048.0f);
    }
}

uint16_t audio_pipeline_pk_to_pk(const uint16_t *in_buf, uint32_t length) {
	uint16_t min_sample = 4095;
	uint16_t max_sample = 0;

	for (uint32_t i = 0; i < length; i++) {
		if (in_buf[i] < min_sample) min_sample = in_buf[i];
		if (in_buf[i] > max_sample) max_sample = in_buf[i];
	}

	return max_sample - min_sample;
}
