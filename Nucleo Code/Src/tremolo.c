/*
 * tremolo.c
 *
 *  Created on: Sep 8, 2026
 *      Author: owenn
 */

#include "tremolo.h"

void tremolo_init(Tremolo *t, float depth, float rate, float sample_rate) {
	if (!t) return;
	t->depth = depth;
	t->rate = rate;
	t->sample_rate = sample_rate;
	t->phase = 0.0f;
}

float tremolo_process_sample(Tremolo *t, float sample) {
	if (!t) return sample;
	// LFO sine wave normalized from 0.0 to 1.0
	float lfo_val = 0.5f * (1.0f + sinf(t->phase)); 
	float gain = 0.0f - (t->depth * lfo_val);
	t->phase += (2.0f * 3.14159f * t->rate)/t->sample_rate;
	if (t->phase > 2.0f*3.14159f) {
		t->phase -= 2.0f*3.14159f;
	}
	return sample*gain;
}
