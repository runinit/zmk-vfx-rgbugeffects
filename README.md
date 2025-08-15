# ZMK RGB Dual Breath Module

A ZMK module that provides a dual-color breathing RGB underglow effect.

## Features

- Custom dual-color breathing animation
- Configurable color hues and saturation
- Smooth transitions between colors
- Toggle on/off functionality

## Configuration

Add this module to your `config/west.yml`:

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: runinit
      url-base: https://github.com/runinit
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: zmk-rgb-dual-breath
      remote: runinit
      revision: main
  self:
    path: config
```

## Usage

In your keymap file, add:

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

Then use in your keymap:
```c
&rgb_dual_breath RGB_DUAL_BREATH_TOG
```

## Configuration Options

- `CONFIG_ZMK_RGB_DUAL_BREATH_COLOR1_HUE`: First color hue (0-359, default: 120)
- `CONFIG_ZMK_RGB_DUAL_BREATH_COLOR2_HUE`: Second color hue (0-359, default: 240) 
- `CONFIG_ZMK_RGB_DUAL_BREATH_SATURATION`: Color saturation (0-100, default: 100)

## Commands

- `RGB_DUAL_BREATH_TOG`: Toggle the dual breath effect
- `RGB_DUAL_BREATH_ON`: Enable the dual breath effect
- `RGB_DUAL_BREATH_OFF`: Disable the dual breath effect