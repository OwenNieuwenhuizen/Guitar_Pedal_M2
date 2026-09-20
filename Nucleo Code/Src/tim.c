/*
 * tim.c
 *
 *  Created on: Aug 30, 2026
 *      Author: owenn
 */

#include "tim.h"
#include <stddef.h>

static const char *telemetry_buf_ptr = NULL;
static uint16_t telemetry_len = 0;

void USART3_DMA1_TIM2_Telemetry_Init(const char *buffer, uint16_t length) {
	telemetry_buf_ptr = buffer;
	telemetry_len = length;
	RCC->APB1ENR |= (1U << 0);
	TIM2->CR1 &= ~1U;
	TIM2->PSC = 10800U - 1U;
	TIM2->ARR = 30000U - 1U;
	TIM2->DIER |= 1U;
	NVIC->ISER[28U>>5] |= (1U<<(28%32));
	TIM2->CR1 |= 1U<<0;
}

#include "registers.h"

void TIM2_SampleClock_Init(uint32_t sampling_rate_hz) {
    RCC->APB1ENR |= (1U << 0);
    TIM2->CR1 &= ~(1U << 0);
    TIM2->PSC = 0U;
    TIM2->ARR = (108000000U / sampling_rate_hz) - 1U;
    TIM2->CR2 &= ~(7U << 4);
    TIM2->CR2 |=  (2U << 4);
    TIM2->EGR |= (1U << 0);
    TIM2->SR = 0U;
    TIM2->CR1 |= (1U << 0);
}

void TIM2_IRQHandler(void) {
	if (TIM2->SR & (1U << 0)) {
		TIM2->SR &= ~(1U << 0);
		if (telemetry_buf_ptr && telemetry_len > 0) {
			USART3_TransmitDMA(telemetry_buf_ptr, telemetry_len);
		}
	}
}
