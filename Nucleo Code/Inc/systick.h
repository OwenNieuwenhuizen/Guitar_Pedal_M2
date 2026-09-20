/*
 * systick.h
 *
 *  Created on: Aug 27, 2026
 *      Author: owenn
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "registers.h"

#define LOAD_VAL 215999U

extern volatile uint32_t g_ticks;

void SysTick_Init(void);
void SysTick_Handler(void);
void Delay(uint32_t ms);
uint32_t SysTick_GetTicks(void);

#endif /* SYSTICK_H_ */
