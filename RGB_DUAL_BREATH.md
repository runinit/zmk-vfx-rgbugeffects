# Dual-Color Breathing RGB Effect

This ZMK module adds a custom dual-color breathing effect to your keyboard's RGB underglow.

## Features

- Alternates between two configurable colors while breathing
- Smooth fade in/out animation
- Configurable hue values for both colors
- Configurable saturation level

## Configuration

Add these options to your `.conf` file:

```
CONFIG_ZMK_RGB_DUAL_BREATH=y
CONFIG_ZMK_RGB_DUAL_BREATH_COLOR1_HUE=120    # First color (0-359, default: green)
CONFIG_ZMK_RGB_DUAL_BREATH_COLOR2_HUE=240    # Second color (0-359, default: blue)  
CONFIG_ZMK_RGB_DUAL_BREATH_SATURATION=100   # Saturation (0-100, default: 100)
```

## Usage

Add to your keymap:

```c
#include <dt-bindings/zmk/rgb_dual_breath.h>

/ {
    rgb_dual_breath: rgb_dual_breath {
        compatible = "zmk,behavior-rgb-dual-breath";
        label = "RGB_DUAL_BREATH";
        #binding-cells = <1>;
    };
};
```

Then use in your key bindings:
- `&rgb_dual_breath RGB_DUAL_BREATH_TOG` - Toggle effect on/off
- `&rgb_dual_breath RGB_DUAL_BREATH_ON` - Turn effect on
- `&rgb_dual_breath RGB_DUAL_BREATH_OFF` - Turn effect off

## Color Values

Common hue values:
- Red: 0
- Orange: 30
- Yellow: 60
- Green: 120
- Cyan: 180
- Blue: 240
- Purple: 270
- Pink: 330