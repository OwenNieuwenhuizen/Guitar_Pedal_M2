/*
 * registers.h
 *
 *  Created on: Aug 26, 2026
 *      Author: owenn
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

/* -- RCC Register Layout -- */
typedef struct {
	volatile uint32_t CR; // Clock control
	volatile uint32_t PLLCFGR; // PLL config
	volatile uint32_t CFGR; // Clock config
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR; // AHB1 enable periph clock
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	uint32_t RESERVED2;
	volatile uint32_t APB1ENR; // APB1 enable periph clock
	volatile uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR1;
	volatile uint32_t DCKCFGR2;
} RCC_TypeDef;

/* -- FLASH Register Layout -- */
typedef struct {
	volatile uint32_t ACR; // Access control
} FLASH_TypeDef;

/* -- SysTick Register Layout -- */
typedef struct {
	volatile uint32_t CSR; // Control and status
	volatile uint32_t RVR; // Reload value
	volatile uint32_t CVR; // Current value
} SYST_TypeDef;

/* -- GPIO Register Layout -- */
typedef struct {
	volatile uint32_t MODER; // Mode
	volatile uint32_t OTYPER; // Output type
	volatile uint32_t OSPEEDR; // Output speed
	volatile uint32_t PUPDR; // Pulldown pullup
	volatile uint32_t IDR; // Input data
	volatile uint32_t ODR; // Output data
	volatile uint32_t BSRR; // Bit set reset
	volatile uint32_t LCKR; // Lock config
	volatile uint32_t AFRL; // Alternate funct low
	volatile uint32_t AFRH; // Alternate funct high
} GPIO_TypeDef;

/* -- USART Register Layout -- */
typedef struct {
	volatile uint32_t CR1; // Control 1
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t BRR; // Baud rate
	volatile uint32_t GTPR;
	volatile uint32_t RTOR;
	volatile uint32_t RQR;
	volatile uint32_t ISR; // Interrupt and status
	volatile uint32_t ICR;
	volatile uint32_t RDR;
	volatile uint32_t TDR; // Transmit data
} USART_TypeDef;

/* -- NVIC Registers -- */
typedef struct {
	volatile uint32_t ISER[8];
	uint32_t RESERVED0[24];
	volatile uint32_t ICER[8];
	uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];
	uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];
	uint32_t RESERVED3[24];
	volatile uint32_t IABR[8];
	uint32_t RESERVED4[56];
	volatile uint32_t IPR[60];
	uint32_t RESERVED5[644];
	volatile uint32_t STIR;
} NVIC_TypeDef;

/* -- DMA Registers -- */
typedef struct {
	volatile uint32_t CR; // Configuration
	volatile uint32_t NDTR; // Number of data
	volatile uint32_t PAR; // Peripheral address
	volatile uint32_t M0AR; // Memory 0 address
	volatile uint32_t M1AR; // Memory 1 address
	volatile uint32_t FCR; // FIFO control
} DMA_Stream_TypeDef;

typedef struct {
	volatile uint32_t LISR; // Low interrupt status
	volatile uint32_t HISR; // High interrupt status
	volatile uint32_t LIFCR; // Low interrupt flag clear
	volatile uint32_t HIFCR; // High interrupt flag clear
	DMA_Stream_TypeDef Stream[8];
} DMA_TypeDef;

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	uint32_t RESERVED;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;
} TIM_TypeDef;

typedef struct {
	volatile uint32_t CPACR;
	uint32_t RESERVED0[106];
	volatile uint32_t FPCCR;
	volatile uint32_t FPCAR;
	volatile uint32_t FPDSCR;
} FPU_TypeDef;

typedef struct {
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	volatile uint32_t JOFR1;
	volatile uint32_t JOFR2;
	volatile uint32_t JOFR3;
	volatile uint32_t JOFR4;
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t JSQR;
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	volatile uint32_t DR;
} ADCN_TypeDef;

typedef struct {
	ADCN_TypeDef ADC1;
	uint32_t RESERVED0 [44];
	ADCN_TypeDef ADC2;
	uint32_t RESERVED1 [44];
	ADCN_TypeDef ADC3;
	uint32_t RESERVED2 [44];
	volatile uint32_t CSR;
	volatile uint32_t CCR;
	volatile uint32_t CDR;
} ADC_TypeDef;

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t SWTRIGR;
	volatile uint32_t DHR12R1;
	volatile uint32_t DHR12L1;
	volatile uint32_t DHR8R1;
	volatile uint32_t DHR12R2;
	volatile uint32_t DHR12L2;
	volatile uint32_t DHR8R2;
	volatile uint32_t DHR12RD;
	volatile uint32_t DHR12LD;
	volatile uint32_t DHR8RD;
	volatile uint32_t DOR1;
	volatile uint32_t DOR2;
	volatile uint32_t SR;
} DAC_TypeDef;

/* -- Base Addresses -- */
#define RCC_BASE 0x40023800U
#define FLASH_BASE 0x40023C00U
#define SYST_BASE 0xE000E010U
#define GPIOA_BASE 0x40020000U
#define GPIOB_BASE 0x40020400U
#define GPIOC_BASE 0x40020800U
#define GPIOD_BASE 0x40020C00U
#define GPIOE_BASE 0x40021000U
#define GPIOF_BASE 0x40021400U
#define GPIOG_BASE 0x40021800U
#define GPIOH_BASE 0x40021C00U
#define USART3_BASE 0x40004800U
#define NVIC_BASE 0xE000E100U
#define DMA1_BASE 0x40026000U
#define DMA2_BASE 0x40026400U
#define TIM2_BASE 0x40000000U
#define FPU_BASE 0xE000ED88U
#define ADC_BASE 0x40012000U
#define DAC_BASE 0x40007400U

/* -- Peripheral Instance Pointers -- */
#define RCC ((RCC_TypeDef *) RCC_BASE)
#define FLASH ((FLASH_TypeDef *) FLASH_BASE)
#define SYST ((SYST_TypeDef *) SYST_BASE)
#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH ((GPIO_TypeDef *) GPIOH_BASE)
#define USART3 ((USART_TypeDef *) USART3_BASE)
#define NVIC ((NVIC_TypeDef *) NVIC_BASE)
#define DMA1 ((DMA_TypeDef *) DMA1_BASE)
#define DMA2 ((DMA_TypeDef *) DMA2_BASE)
#define TIM2 ((TIM_TypeDef *) TIM2_BASE)
#define FPU ((FPU_TypeDef *) FPU_BASE)
#define ADC ((ADC_TypeDef *) ADC_BASE)
#define DAC ((DAC_TypeDef *) DAC_BASE)

#endif /* REGISTERS_H_ */
