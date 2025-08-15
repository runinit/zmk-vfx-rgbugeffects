# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a ZMK (Zephyr Mechanical Keyboard) module that provides a dual-color breathing RGB underglow effect. The module creates a smooth breathing animation that alternates between two configurable colors.

## Architecture

### Core Components

- **Behavior Implementation** (`src/behaviors/behavior_rgb_dual_breath.c`): Main C implementation containing the animation logic, timer handling, and ZMK behavior interface
- **Device Tree Bindings** (`dts/bindings/behaviors/zmk,behavior-rgb-dual-breath.yaml`): Device tree binding definition for the behavior
- **Header Definitions** (`include/dt-bindings/zmk/rgb_dual_breath.h`): Command constants for toggle/on/off operations
- **Module Configuration** (`zephyr/module.yml`): Zephyr module definition and build settings

### Key Architecture Elements

- **Timer-based Animation**: Uses Zephyr's `k_timer` for 50ms interval updates to create smooth breathing effect
- **HSB Color System**: Works with hue (0-359), saturation (0-100), brightness (0-100) color model
- **ZMK Integration**: Integrates with ZMK's RGB underglow system via `zmk_rgb_underglow_set_hsb()`
- **Dual-Phase Breathing**: 512-step cycle where steps 0-255 show color1, steps 256-511 show color2

## Configuration System

The module uses ZMK's Kconfig system for compile-time configuration:

- `CONFIG_ZMK_RGB_DUAL_BREATH_COLOR1_HUE`: First color hue (0-359, default: 120)
- `CONFIG_ZMK_RGB_DUAL_BREATH_COLOR2_HUE`: Second color hue (0-359, default: 240)
- `CONFIG_ZMK_RGB_DUAL_BREATH_SATURATION`: Color saturation (0-100, default: 100)

## Usage Pattern

Users integrate this module by:
1. Adding the module to their `west.yml` manifest
2. Including the device tree binding in their keymap
3. Using behavior bindings: `&rgb_dual_breath RGB_DUAL_BREATH_TOG`

## Development Notes

- This is a Zephyr RTOS module following ZMK's behavior API patterns
- The breathing effect uses integer math for performance on microcontrollers
- Animation state is managed in a global singleton structure
- Timer cleanup is handled properly when effect is disabled