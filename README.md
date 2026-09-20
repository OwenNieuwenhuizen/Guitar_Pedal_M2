# STM32F767ZI Audio Firmware

Bare-metal firmware for an STM32F767ZI Nucleo-144 board. The project samples an analog audio signal, processes it in real time, and writes the processed signal to the on-chip DAC. The current processing path includes a tremolo effect and gain/soft-clipping stage.

## What it does

- Samples ADC1 channel 0 on `PA0` at 48 kHz.
- Uses DMA ping-pong buffers to move 256-sample audio frames with minimal CPU overhead.
- Applies audio processing in the main loop using the Cortex-M7 floating-point unit.
- Sends 12-bit processed samples to DAC channel 1 on `PA4`.

The application entry point is `Nucleo Code/Src/main.c`. Peripheral configuration lives in modules such as `adc_dac.c`, `dma.c`, and `tim.c`; DSP code is in `audio_process.c` and `tremolo.c`.

## Requirements

- STM32 Nucleo-144 board with an STM32F767ZI MCU
- ST-LINK connection for programming and debugging
- STM32CubeIDE with the STM32 ARM toolchain
- An audio input conditioned for the ADC's 0–3.3 V range and biased near mid-rail (approximately 1.65 V)

> **Hardware note:** `PA0` is configured as the ADC input and `PA4` as the DAC output. Do not connect an audio signal outside the MCU analog voltage range.

## Build and flash

1. In STM32CubeIDE, choose **File > Import > Existing Projects into Workspace**.
2. Select the `Nucleo Code` directory.
3. Build the `Debug` configuration with **Project > Build Project** (or `Ctrl+B`).
4. Connect the board over ST-LINK and start the configured debug/launch session to program it.

Generated files, including the ELF, map file, and object files, are written to `Nucleo Code/Debug/` and are intentionally ignored by Git.

## Repository layout

```text
Nucleo Code/
  Inc/       Module headers and register definitions
  Src/       Application, peripheral drivers, and audio DSP
  Startup/   Cortex-M startup assembly
  *.ld       Flash and RAM linker scripts
STM32F7ZI Documentation/  Board and MCU reference documents
```

## Validation

There is no automated test suite. For each change, complete a clean Debug build and validate the affected behavior on the target board. For audio changes, confirm the expected signal path and effect using suitable measurement equipment or listening hardware.

## Contributing

See [AGENTS.md](AGENTS.md) for module conventions, validation expectations, and commit/pull-request guidance.
