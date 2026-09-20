/*
 * usart.c
 *
 *  Created on: Aug 28, 2026
 *      Author: owenn
 */

#include "usart.h"
#include <stdint.h>

static volatile RingBuffer_t rx_ring_buffer = {.head=0, .tail=0};

void USART3_Init(void) {
	uint8_t PTX, PRX;
	PTX = P8;
	PRX = P9;
	RCC->APB1ENR |= 1U<<18;
//	RCC->AHB1ENR |= 1U<<21;

	GPIO_Init(GPIOD, PTX, ALT);
	GPIO_SetFunct(GPIOD, PTX, AF7);

	GPIO_Init(GPIOD, PRX, ALT);
	GPIO_SetFunct(GPIOD, PRX, AF7);

	USART3->BRR = 469U;
	USART3->CR1 = (1U<<3)|(1U<<2)|(1U<<5)|(1U);
	NVIC->ISER[39U>>5] |= 1U<<(39%32);
	USART3->CR3 |= 1U<<7;

//	DMA1_Stream3_Init();
}

void USART3_WriteChar(char c) {
	while(!(USART3->ISR&(1U<<7))){}
	USART3->TDR = (uint8_t)c;
}

void USART3_WriteByte(uint8_t byte) {
	while(!(USART3->ISR&(1U<<7))){}
	USART3->TDR = byte;
}

void USART3_WriteStr(const char *str) {
	while (*str) {
		USART3_WriteChar(*str++);
	}
}

void USART3_IRQHandler(void) {
	if (USART3->ISR&(1U<<3)) {
		USART3->ICR |= 1U<<3;
	}
	if (USART3->ISR&(1U<<5)) {
		uint8_t data = (uint8_t)(USART3->RDR&0xFFU);
		uint16_t next_head = (rx_ring_buffer.head+1)%RING_BUF_SIZE;
		if (next_head != rx_ring_buffer.tail) {
			rx_ring_buffer.buffer[rx_ring_buffer.head] = data;
			rx_ring_buffer.head = next_head;
		}
	}
}

uint8_t USART3_ReadByte(uint8_t *data) {
	if (rx_ring_buffer.head == rx_ring_buffer.tail) {
		return 0;
	}
	*data = rx_ring_buffer.buffer[rx_ring_buffer.tail];
	rx_ring_buffer.tail = (rx_ring_buffer.tail+1)%RING_BUF_SIZE;
	return 1;
}

void DMA1_Stream3_Init(void) {
	DMA1->Stream[3].CR &= ~(1U);
	while (DMA1->Stream[3].CR&1U) {}
	DMA1->LIFCR |= (0x0FU<<24)|(1U<<22);
	DMA1->Stream[3].PAR = (uint32_t)&(USART3->TDR);
	DMA1->Stream[3].CR = (4U<<25)|(1U<<10)|(1U<<6)|(1U<<4);
}

uint8_t USART3_DMABusy(void) {
	return (DMA1->Stream[3].CR&1U) ? 1: 0;
}

void USART3_TransmitDMA(const char *buffer, uint16_t length) {
	while(USART3_DMABusy()) {}
	DMA1->LIFCR |= (0x0FU<<24)|(1U<<22);
	DMA1->Stream[3].NDTR = length;
	DMA1->Stream[3].M0AR = (uint32_t)buffer;
	DMA1->Stream[3].CR |= 1U;
}
