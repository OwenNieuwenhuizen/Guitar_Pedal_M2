# Repository Guidelines

## Project Structure & Module Organization

This repository contains bare-metal firmware for an STM32F767ZI Nucleo-144 board.

- `Nucleo Code/Src/` contains C implementations, including startup application logic in `main.c` and peripheral/audio modules such as `adc_dac.c`, `dma.c`, and `tremolo.c`.
- `Nucleo Code/Inc/` contains the matching public headers and register definitions. Keep declarations and include guards aligned with their source module.
- `Nucleo Code/Startup/` holds the MCU startup assembly; linker scripts (`STM32F767ZITX_*.ld`) define RAM and flash layouts.
- `Nucleo Code/Debug/` is generated STM32CubeIDE build output and must not be committed.
- `STM32F7ZI Documentation/` contains reference PDFs for the MCU and development board; do not edit vendor documentation.

## Build, Test, and Development Commands

Open `Nucleo Code/.project` in STM32CubeIDE, select the `Debug` configuration, then use **Project > Build Project** (or `Ctrl+B`). This produces the ELF and related artifacts under `Nucleo Code/Debug/`. Flash and debug through the configured ST-LINK launch configuration in the IDE.

There is no checked-in command-line build system or automated test suite. Before submitting changes, build the `Debug` configuration and exercise affected hardware paths on the board (for example, 48 kHz audio DMA after changing `audio_process.c`).

## Coding Style & Naming Conventions

Use C and preserve the surrounding module style: four-space indentation in new code, braces on the same line as control statements/functions, and standard fixed-width integer types from `<stdint.h>`. Name files after their module (`dma.c` / `dma.h`), public functions in `Pascal_Case` when following existing peripheral APIs (for example, `DMA2_Stream0_ADC_Init`), and internal state in `lower_snake_case`. Use `UPPER_SNAKE_CASE` for macros and include guards. Keep interrupt and DMA-shared state `volatile` where required.

## Testing Guidelines

No unit-test framework or coverage target is configured. Treat a clean firmware build plus targeted on-device verification as required validation. State the board setup, peripheral path tested, and observed result in the pull request.

## Commit & Pull Request Guidelines

The available history uses a concise imperative subject (for example, `Initial commit for guitar pedal M-2`). Follow that pattern: `Add ADC DMA buffer guard`. Keep commits focused and exclude generated binaries, IDE settings, and launch files. Pull requests should describe the hardware behavior changed, identify affected files/peripherals, link any issue, and include serial output, scope captures, or screenshots when they substantiate the result.
