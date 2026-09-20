/*
 * gpio.h
 *
 *  Created on: Aug 27, 2026
 *      Author: owenn
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "registers.h"

/* -- GPIO Pin Definitions -- */
#define P0 0U
#define P1 1U
#define P2 2U
#define P3 3U
#define P4 4U
#define P5 5U
#define P6 6U
#define P7 7U
#define P8 8U
#define P9 9U
#define P10 10U
#define P11 11U
#define P12 12U
#define P13 13U
#define P14 14U
#define P15 15U

/* -- GPIO Modes -- */
#define INPUT 0U
#define GOUPUT 1U
#define ALT 2U
#define ANALOG 3U

/* -- ALT Functions -- */
#define AF0 0U
#define AF1 1U
#define AF2 2U
#define AF3 3U
#define AF4 4U
#define AF5 5U
#define AF6 6U
#define AF7 7U
#define AF8 8U
#define AF9 9U
#define AF10 10U
#define AF11 11U
#define AF12 12U
#define AF13 13U
#define AF14 14U
#define AF15 15U

/* -- Function Prototypes -- */
void GPIO_ClockEnable(GPIO_TypeDef *GPIOx);
void GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode);
void GPIO_SetFunct(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t funct);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif /* GPIO_H_ */
