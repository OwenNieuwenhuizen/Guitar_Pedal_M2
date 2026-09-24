/*
 * tim.h
 *
 *  Created on: Aug 30, 2026
 *      Author: owenn
 */

#ifndef TIM_H_
#define TIM_H_

#include "usart.h"
#include "registers.h"

void USART3_DMA1_TIM2_Telemetry_Init(const char *buffer, uint16_t length);
void TIM2_SampleClock_Init(uint32_t sampling_rate_hz);
void TIM2_IRQHandler(void);

#endif /* TIM_H_ */
