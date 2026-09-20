/*
 * adc_dac.c
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#include "adc_dac.h"

/* adc_dac.c */
#include "registers.h"

void Audio_ADC_DAC_Init(void) {
    /* -------------------------------------------------------------------------
     * 1. Enable Peripheral Clocks
     * ------------------------------------------------------------------------- */
    RCC->APB2ENR |= (1U << 8);   /* ADC1EN (Bit 8)  */
    RCC->APB1ENR |= (1U << 29);  /* DACEN  (Bit 29) */

    /* -------------------------------------------------------------------------
     * 2. Configure Pins PA0 (ADC) and PA4 (DAC) as Analog
     * ------------------------------------------------------------------------- */
    GPIO_Init(GPIOA, P4, ANALOG);
    GPIO_Init(GPIOA, P0, ANALOG);
    GPIOA->PUPDR &= ~((3U << (0 * 2)) | (3U << (4 * 2)));

    /* -------------------------------------------------------------------------
     * 3. Configure ADC1 for TIM2 TRGO Hardware Triggering
     * ------------------------------------------------------------------------- */
    /* Set ADC Prescaler to PCLK2 / 4 in Common Control Register */
    ADC->CCR &= ~(3U << 16);
    ADC->CCR |=  (1U << 16);

    /* 12-bit resolution (CR1 [25:24] = 00) */
    ADC->ADC1.CR1 &= ~(3U << 24);

    /* Select TIM2 TRGO as External Trigger Source (EXTSEL [27:24] = 0110) */
    ADC->ADC1.CR2 &= ~(0xFU << 24);
    ADC->ADC1.CR2 |=  (6U << 24);

    /* External Trigger Enable: Trigger on Rising Edge (EXTEN [29:28] = 01) */
    ADC->ADC1.CR2 &= ~(3U << 28);
    ADC->ADC1.CR2 |=  (1U << 28);

    /* Enable DMA mode and Continuous DMA Requests (DMA = Bit 8, DDS = Bit 9) */
    ADC->ADC1.CR2 |= (1U << 8) | (1U << 9);

    /* Sample Time: 15 cycles on Channel 0 (SMPR2 [2:0] = 001) */
    ADC->ADC1.SMPR2 &= ~(7U << 0);
    ADC->ADC1.SMPR2 |=  (1U << 0);

    /* Sequence Length = 1 conversion (SQR1 [23:20] = 0000) */
    ADC->ADC1.SQR1 &= ~(0xFU << 20);

    /* Channel Selection: 1st conversion in sequence = Channel 0 (PA0) */
    ADC->ADC1.SQR3 &= ~(0x1FU << 0);

    /* Turn on ADC1 */
    ADC->ADC1.CR2 |= (1U << 0);

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
