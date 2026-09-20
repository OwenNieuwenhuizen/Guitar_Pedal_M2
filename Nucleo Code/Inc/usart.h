/*
 * usart.h
 *
 *  Created on: Aug 28, 2026
 *      Author: owenn
 */

#ifndef USART_H_
#define USART_H_

#include "registers.h"
#include "gpio.h"

#define RING_BUF_SIZE 128

typedef struct {
	volatile uint8_t buffer[RING_BUF_SIZE];
	volatile uint16_t head;
	volatile uint16_t tail;
} RingBuffer_t;

/* TX Utilities */
void USART3_Init();
void USART3_WriteChar(char c);
void USART3_WriteByte(uint8_t byte);
void USART3_WriteStr(const char *str);

/* -- Ring Buffer RX -- */
uint8_t USART3_ReadByte(uint8_t *data);
void USART3_IRQHandler(void);

/* -- DMA Telemetry Transmission API -- */
void DMA1_Stream3_Init(void);
void USART3_TransmitDMA(const char *buffer, uint16_t length);
uint8_t USART3_DMABusy(void);
#endif /* USART_H_ */
