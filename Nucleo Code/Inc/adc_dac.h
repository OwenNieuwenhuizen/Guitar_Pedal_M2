/*
 * adc_dac.h
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#ifndef ADC_DAC_H_
#define ADC_DAC_H_

#include "registers.h"
#include "gpio.h"

void Audio_ADC_Init(void);
void Audio_DAC_Init(void);
void DAC1_WriteSample(uint16_t sample);

#endif /* ADC_DAC_H_ */
