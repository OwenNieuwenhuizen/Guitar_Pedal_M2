/*
 * audio_process.h
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#ifndef AUDIO_PROCESS_H_
#define AUDIO_PROCESS_H_

#include "tremolo.h"
#include "adc_dac.h"

typedef struct {
	float master_gain;
	Tremolo tremolo;
} AudioPipeline;

void audio_pipeline_init(AudioPipeline *pipeline, float sample_rate);
void audio_pipeline_process_12bit(AudioPipeline *pipeline, const uint16_t *in_buf, uint16_t *out_buf, uint32_t length);
void audio_pipeline_process_float(AudioPipeline *pipeline, const float *in_buf, float *out_buf, uint32_t length);
uint16_t audio_pipeline_pk_to_pk(const uint16_t *in_buf, uint32_t length);

#endif /* AUDIO_PROCESS_H_ */
