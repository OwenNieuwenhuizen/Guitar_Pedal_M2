/*
 * dma.h
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#ifndef DMA_H_
#define DMA_H_

#include "registers.h"
#include <stddef.h>

void DMA2_Stream0_ADC_Init(uint16_t *buf0, uint16_t *buf1, uint16_t block_size, void (*cb)(uint16_t *));
void DMA2_Stream0_IRQHandler(void);

#endif /* DMA_H_ */
