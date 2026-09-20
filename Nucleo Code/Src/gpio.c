/*
 * gpio.c
 *
 *  Created on: Aug 27, 2026
 *      Author: owenn
 */

#include "gpio.h"

void GPIO_ClockEnable(GPIO_TypeDef *GPIOx) {
	if (GPIOx == GPIOA) {RCC->AHB1ENR|=(1U);}
	else if (GPIOx == GPIOB) {RCC->AHB1ENR|=(1U<<1);}
	else if (GPIOx == GPIOC) {RCC->AHB1ENR|=(1U<<2);}
	else if (GPIOx == GPIOD) {RCC->AHB1ENR|=(1U<<3);}
	else if (GPIOx == GPIOE) {RCC->AHB1ENR|=(1U<<4);}
	else if (GPIOx == GPIOF) {RCC->AHB1ENR|=(1U<<5);}
	else if (GPIOx == GPIOG) {RCC->AHB1ENR|=(1U<<6);}
	else if (GPIOx == GPIOH) {RCC->AHB1ENR|=(1U<<7);}
}

void GPIO_Init(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode) {
	GPIO_ClockEnable(GPIOx);
	GPIOx->MODER &= ~(3U<<(pin<<1));
	GPIOx->MODER |= mode<<(pin<<1);
}

void GPIO_SetFunct(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t funct) {
	if (pin<8) {
		GPIOx->AFRL &= ~(0x0FU<<(pin<<2));
		GPIOx->AFRL |= funct<<(pin<<2);
	} else {
		pin-=8;
		GPIOx->AFRH &= ~(0x0FU<<(pin<<2));
		GPIOx->AFRH |= funct<<(pin<<2);
	}
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state) {
	if (state) {
		GPIOx->BSRR = 1U<<pin;
	} else {
		GPIOx->BSRR = 1U<<(pin+16U);
	}
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin) {
	return !!(GPIOx->ODR&1U<<pin);
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin) {
	if (GPIOx->ODR&(1U<<pin)) {
		GPIOx->BSRR = 1U<<(pin+16U);
	} else {
		GPIOx->BSRR = 1U<<pin;
	}
}
