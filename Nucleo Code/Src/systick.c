/*
 * systick.c
 *
 *  Created on: Aug 27, 2026
 *      Author: owenn
 */

#include "systick.h"

volatile uint32_t g_ticks = 0;

void SysTick_Init(void) {
	SYST->RVR = LOAD_VAL;
	SYST->CVR = 0U;
	SYST->CSR |= 0x07U;
}

void SysTick_Handler(void) {
	g_ticks++;
}

void Delay(uint32_t ms) {
	uint32_t start_ticks = g_ticks;
	while ((g_ticks-start_ticks) < ms) {}
}

uint32_t SysTick_GetTicks(void) {
	return g_ticks;
}
