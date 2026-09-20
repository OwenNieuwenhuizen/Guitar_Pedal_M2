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

void process_audio_frame(uint16_t *in_buffer, uint16_t *out_buffer, uint32_t length);

#endif /* AUDIO_PROCESS_H_ */
