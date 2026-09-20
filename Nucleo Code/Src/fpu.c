/*
 * fpu.c
 *
 *  Created on: Sep 11, 2026
 *      Author: owenn
 */

#include "fpu.h"

void FPU_Init(void) {
	FPU->CPACR |= 0x0FU<<20;
}
