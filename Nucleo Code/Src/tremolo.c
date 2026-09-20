/*
 * tremolo.c
 *
 *  Created on: Sep 8, 2026
 *      Author: owenn
 */

#include "tremolo.h"

static float g_tremolo_depth = 0.8f;
static float g_tremolo_rate = 7.0f;
static float g_tremolo_freq = 48000.0f;
static float g_tremolo_phase = 0.0f;

int16_t tremolo_process_samp(float samp) {
	float lfo_f = 0.5f * (sinf(g_tremolo_phase)+1.0f);
	float gain = (1.0f-g_tremolo_depth)+(g_tremolo_depth*lfo_f);
	g_tremolo_phase += (2.0f * 3.14159f * g_tremolo_rate)/g_tremolo_freq;
	if (g_tremolo_phase > 2.0f*3.14159f) {
		g_tremolo_phase -= 2.0f*3.14159f;
	}
	return samp*gain;
}
