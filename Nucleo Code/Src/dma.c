/*
 * dma.c
 *
 *  Created on: Sep 12, 2026
 *      Author: owenn
 */

#include "dma.h"
#include <stdint.h>

static void (*dma_adc_callback)(uint16_t *buffer) = NULL;
static void (*dma_dac_callback)(uint16_t *buffer) = NULL;

void DMA2_Stream0_ADC_Init(uint16_t *buf0, uint16_t *buf1, uint16_t block_size, void (*cb)(uint16_t *)) {
    dma_adc_callback = cb;

    /* Enable DMA2 Clock (AHB1ENR Bit 22) */
    RCC->AHB1ENR |= (1U << 22);

    /* Disable Stream 0 */
    DMA2->Stream[0].CR &= ~1U;
    while (DMA2->Stream[0].CR & 1U);

    /* Clear Stream 0 Interrupt Flags in Low Interrupt Flag Clear Register */
    DMA2->LIFCR |= 0x3DU;

    /* Assign Addresses and Block Size */
    DMA2->Stream[0].PAR  = (uint32_t)&(ADC->ADC1.DR);
    DMA2->Stream[0].M0AR = (uint32_t)buf0;
    DMA2->Stream[0].M1AR = (uint32_t)buf1;
    DMA2->Stream[0].NDTR = block_size;

    /* Configuration:
     * - CHSEL [27:25] = 000 (Channel 0)
     * - DBM [18]      = 1   (Double Buffer Mode)
     * - MSIZE [14:13] = 01  (16-bit Memory size)
     * - PSIZE [12:11] = 01  (16-bit Peripheral size)
     * - MINC [10]     = 1   (Memory Increment)
     * - CIRC [8]      = 1   (Circular Mode)
     * - TCIE [4]      = 1   (Transfer Complete Interrupt Enable)
     */
    DMA2->Stream[0].CR = (1U << 18) | (1U << 14) | (1U << 11) |
                         (1U << 10) | (1U << 8)  | (1U << 4);

    /* Enable DMA2 Stream 0 IRQ (IRQ #56) in NVIC */
    /* Index: 56 >> 5 = 1 (ISER[1]), Bit: 56 % 32 = 24 */
    NVIC->ISER[56U >> 5] |= (1U << (56U % 32));

    /* Enable Stream 0 */
    DMA2->Stream[0].CR |= (1U << 0);
}

void DMA2_Stream0_IRQHandler(void) {
    /* Check TCIF0 (Bit 5 in LISR) */
//	Transfer complete, then clear flag and read data in to buf
    if (DMA2->LISR & (1U << 5)) {
    	DMA2->LIFCR = (0x3DU << 0);

        /* CT bit (Bit 19) in CR indicates which buffer DMA is currently targeting.
         * If CT == 1, DMA is actively writing to M1AR, so M0AR is ready for processing. */
        uint16_t *ready_buf = (DMA2->Stream[0].CR & (1U << 19)) ?
                              (uint16_t*)DMA2->Stream[0].M0AR :
                              (uint16_t*)DMA2->Stream[0].M1AR;

        if (dma_adc_callback) {
            dma_adc_callback(ready_buf);
        }
    }
}

void DMA1_Stream5_DAC_Init(uint16_t *buf0, uint16_t *buf1, uint16_t block_size, void (*cb)(uint16_t *)) {
    dma_dac_callback = cb;

    /* Enable DMA1 clock (AHB1ENR bit 21). */
    RCC->AHB1ENR |= (1U << 21);

    /* Disable Stream 5 before changing its configuration. */
    DMA1->Stream[5].CR &= ~(1U << 0);
    while (DMA1->Stream[5].CR & (1U << 0)) {}

    /* Clear all Stream 5 status flags in the high interrupt flag register. */
    DMA1->HIFCR = 0x0F40U;

    DMA1->Stream[5].PAR = (uint32_t)&DAC->DHR12R1;
    DMA1->Stream[5].M0AR = (uint32_t)buf0;
    DMA1->Stream[5].M1AR = (uint32_t)buf1;
    DMA1->Stream[5].NDTR = block_size;

    /*
     * CHSEL = 7, DIR = memory-to-peripheral, double-buffered circular mode,
     * and 16-bit peripheral/memory accesses for 12-bit DAC samples.
     */
    DMA1->Stream[5].CR = (7U << 25) | (1U << 18) | (1U << 14) |
                         (1U << 11) | (1U << 10) | (1U << 8) |
                         (1U << 6) | (1U << 4);

    /* DMA1 Stream 5 is IRQ 16. */
    NVIC->ISER[16U >> 5] |= (1U << (16U % 32));

    DMA1->Stream[5].CR |= (1U << 0);
}

void DMA1_Stream5_IRQHandler(void) {
    /* TCIF5 is bit 11 in DMA1_HISR/HIFCR. */
    if (DMA1->HISR & (1U << 11)) {
        DMA1->HIFCR = (1U << 11);

        /* CT identifies the active target; the other buffer is safe to refill. */
        uint16_t *ready_buf = (DMA1->Stream[5].CR & (1U << 19)) ?
                              (uint16_t *)DMA1->Stream[5].M0AR :
                              (uint16_t *)DMA1->Stream[5].M1AR;

        if (dma_dac_callback) {
            dma_dac_callback(ready_buf);
        }
    }
}
