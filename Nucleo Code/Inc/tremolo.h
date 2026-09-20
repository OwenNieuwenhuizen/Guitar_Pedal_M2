/*
 * tremolo.h
 *
 *  Created on: Sep 8, 2026
 *      Author: owenn
 */

#ifndef TREMOLO_H_
#define TREMOLO_H_

#include <math.h>
#include <stdint.h>

typedef struct {
	float depth;
	float rate;
	float sample_rate;
	float phase;
} Tremolo;

void tremolo_init(Tremolo *t, float depth, float rate, float sample_rate);
float tremolo_process_sample(Tremolo *t, float samp);

#endif /* TREMOLO_H_ */
