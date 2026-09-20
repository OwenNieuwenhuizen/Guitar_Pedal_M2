# Firmware work journal

## Project

Bare-metal firmware for an STM32F767ZI Nucleo-144 board. Source files are in
`Nucleo Code/Src`; headers are in `Nucleo Code/Inc`. The board audio path is
intended to run at 48 kHz using ADC1 on PA0 and DAC1 channel 1 on PA4.

## Current source state (reviewed 2026-09-20)

- `main.c` has two 256-sample ADC ping/pong buffers and starts ADC DMA2 Stream
  0 in double-buffered circular mode.
- `TIM2_SampleClock_Init()` creates a 48 kHz update-event TRGO.
- `adc_dac.c` configures ADC1 for TIM2 TRGO but configures DAC1 for direct
  writes only; DAC DMA and triggering are not currently enabled.
- `audio_process.c` declares `process_audio_frame(in_buffer, out_buffer,
  length)`, but `main.c` incorrectly calls it with only an input buffer and
  length. This is a build error.
- `tremolo_process_samp()` returns `int16_t` in `tremolo.c` and `tremolo.h`,
  despite its float DSP input/output. This truncates its signal result and
  should be `float`.

## Intended implementation

Implement DAC1 output DMA with independent ping/pong buffers:

1. Add two `uint16_t[BLOCK_SIZE]` DAC buffers in `main.c`, initialize both to
   2048, and leave DAC's holding register at 2048 initially.
2. Configure DAC1 channel 1 for TIM2 TRGO (`TSEL1 = 100`, `TEN1 = 1`) and DAC
   DMA requests (`DMAEN1 = 1`). The DAC target register is `DAC->DHR12R1`.
3. Add `DMA1_Stream5_DAC_Init()` and its IRQ handler in `dma.c`/`dma.h`.
   DMA1 Stream 5, Channel 7 is DAC1 channel 1's mapping. Use memory-to-
   peripheral, halfword memory/peripheral widths, memory increment, circular,
   double-buffered operation, and transfer-complete interrupts. Stream 5 uses
   DMA1 high interrupt status/clear registers; TCIF5 is bit 11. Its NVIC IRQ is
   16.
4. At DAC transfer completion, determine the inactive buffer from the `CT`
   bit and make that completed buffer available for DSP refill. ADC does the
   analogous handoff for the completed input buffer.
5. When both a completed ADC frame and a completed DAC frame are ready, call
   `process_audio_frame(adc_buffer, dac_buffer, BLOCK_SIZE)`.
6. Change `tremolo_process_samp()` and its header declaration to return
   `float`.

The intended sequencing starts ADC DMA and DAC DMA before enabling TIM2, so
the shared trigger begins both sample paths together.

## Validation

There is no checked-in command-line build system. Validate in STM32CubeIDE by
building the Debug configuration, then test the 48 kHz ADC-to-DAC DMA path on
the board. Confirm output begins at midscale and that tremolo is audible.

## Worktree and tooling note

At the last inspection, the only untracked files were `AGENTS.md` and
`README.md`; no firmware sources were changed.

Earlier attempts to patch `Nucleo Code/Inc` and `Nucleo Code/Src` stalled due
to the Windows sandbox error `cannot reopen writable descendants under
read-only carveouts directly`. Write access was later corrected. A temporary
`Src/codex_write_test.txt` was created to confirm access and then removed.

## Implementation completed (2026-09-20)

- `adc_dac.c` now selects TIM2 TRGO as the DAC1 trigger, enables DAC DMA
  requests, initializes the DAC holding register to 2048, and masks direct
  writes to 12 bits.
- `dma.c`/`dma.h` now configure DMA1 Stream 5, Channel 7 for DAC1 channel 1
  memory-to-peripheral circular double buffering, with a transfer-complete
  callback for the inactive buffer.
- `main.c` now owns initialized DAC ping/pong buffers, tracks completed ADC and
  DAC frames independently, and calls
  `process_audio_frame(input, output, BLOCK_SIZE)` with safe inactive buffers.
- `tremolo.c`/`tremolo.h` now return `float` from `tremolo_process_samp()`;
  this prevents truncation of the float DSP result.

## Validation completed

The configured ARM GCC toolchain passed a syntax-only compilation over every
source in `Nucleo Code/Src`. The only diagnostic was the existing FPU warning
emitted by `main.c`; there were no syntax errors. On-board 48 kHz ADC-to-DAC
verification has not yet been performed.
