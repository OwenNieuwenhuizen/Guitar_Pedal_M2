/*
 * adc_dac.c
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#include "adc_dac.h"

/* adc_dac.c */
#include "registers.h"

void Audio_ADC_Init(void) {
    /* -------------------------------------------------------------------------
     * 1. Enable Peripheral Clocks
     * ------------------------------------------------------------------------- */
    RCC->APB2ENR |= (1U << 8);   /* ADC1EN (Bit 8)  */

    /* -------------------------------------------------------------------------
     * 2. Configure Pins PA0 (ADC) and PA4 (DAC) as Analog
     * ------------------------------------------------------------------------- */
    GPIO_Init(GPIOA, P0, ANALOG);
    GPIOA->PUPDR &= ~(3U << (0 * 2));

    /* -------------------------------------------------------------------------
     * 3. Configure ADC1 for TIM2 TRGO Hardware Triggering
     * ------------------------------------------------------------------------- */
    /* Set ADC Prescaler to PCLK2 / 4 in Common Control Register */
    ADC->CCR &= ~(3U << 16);
    ADC->CCR |=  (1U << 16);

    ADC->ADC1.CR1 = 0; // 12-bit resolution (CR1 [25:24] = 00)
    ADC->ADC1.CR2 = 0;

    /* Sequence Length = 1 conversion (SQR1 = 0), Channel 0 first (SQR3 = 0) */
	ADC->ADC1.SQR1 = 0;
	ADC->ADC1.SQR3 = 0;

	/* Set ADC1 Channel 0 sample time to 480 cycles */
	ADC->ADC1.SMPR2 &= ~(7U << (0 * 3)); /* Clear SMP0 */
	ADC->ADC1.SMPR2 |=  (7U << (0 * 3)); /* 111 = 480 cycles */

	/* Turn on ADC1 core power first */
	ADC->ADC1.CR2 |= (1U << 0); // ADON = 1

	/* Small stabilization delay (~10us) */
	for (volatile int i = 0; i < 10000; i++);

	/* Clear any startup flags (OVR, EOC, STRT) */
	ADC->ADC1.SR = 0;

	/* -------------------------------------------------------------------------
	 * 4. Atomic Enable Triggering + DMA
	 * ------------------------------------------------------------------------- */
	/* Write CR2 atomically:
	 * Bit 0  = ADON (Keep power active)
	 * Bit 8  = DMA  (Enable DMA requests)
	 * Bit 9  = DDS  (Continuous DMA Requests)
	 * Bits 27:24 = EXTSEL (0110 = TIM2 TRGO)
	 * Bits 29:28 = EXTEN  (01 = Trigger on Rising Edge)
	 */
	ADC->ADC1.CR2 = (1U << 0)  | (1U << 8)  | (1U << 9) |
					(11U << 24) | (1U << 28);
}

void Audio_DAC_Init(void) {
	/* -------------------------------------------------------------------------
	 * 1. Enable Peripheral Clocks
	 * ------------------------------------------------------------------------- */
    RCC->APB1ENR |= (1U << 29);  /* DACEN  (Bit 29) */
    /* -------------------------------------------------------------------------
	 * 2. Configure Pins PA0 (ADC) and PA4 (DAC) as Analog
	 * ------------------------------------------------------------------------- */
	GPIO_Init(GPIOA, P4, ANALOG);
	GPIOA->PUPDR &= ~(3U << (4 * 2));

	/* -------------------------------------------------------------------------
	 * 4. Configure DAC Channel 1 (PA4 Output)
	 * ------------------------------------------------------------------------- */
	DAC->CR &= ~(1U << 0);          /* Disable DAC Ch1 during config */
	DAC->CR &= ~((7U << 3) | (1U << 12));
	DAC->CR |=  (4U << 3);          /* TSEL1: TIM2 TRGO */
	DAC->CR &= ~(1U << 1);          /* Enable output buffer (BOFF1 = 0) */
	DAC->CR |=  (1U << 2);          /* Enable trigger */
	DAC->CR |=  (1U << 12);         /* Enable DMA requests */
	DAC->DHR12R1 = 2048U;
	DAC->CR |=  (1U << 0);          /* Enable DAC Channel 1 */
}

void DAC1_WriteSample(uint16_t sample) {
    /* Write 12-bit right-aligned data directly to DAC Channel 1 */
    DAC->DHR12R1 = sample & 0x0FFFU;
}
