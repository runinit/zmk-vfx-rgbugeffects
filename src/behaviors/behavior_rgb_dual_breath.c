/*
 * Copyright (c) 2024 ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_rgb_dual_breath

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#include <zmk/behavior.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/rgb_underglow.h>

#include <dt-bindings/zmk/rgb_dual_breath.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_rgb_dual_breath_config {
    uint8_t index;
};

struct behavior_rgb_dual_breath_data {
    bool enabled;
    uint16_t color1_hue;
    uint16_t color2_hue;
    uint8_t saturation;
    uint8_t brightness;
    uint32_t animation_step;
};

static struct behavior_rgb_dual_breath_data dual_breath_state = {
    .enabled = false,
    .color1_hue = CONFIG_ZMK_RGB_DUAL_BREATH_COLOR1_HUE,
    .color2_hue = CONFIG_ZMK_RGB_DUAL_BREATH_COLOR2_HUE,
    .saturation = CONFIG_ZMK_RGB_DUAL_BREATH_SATURATION,
    .brightness = 50,
    .animation_step = 0,
};


// Breathing animation function
static void dual_breath_effect_step(void) {
    if (!dual_breath_state.enabled) {
        return;
    }

    // Create a breathing pattern (0-255)
    uint32_t step = dual_breath_state.animation_step % 512;
    uint8_t breath_intensity;
    
    if (step < 256) {
        breath_intensity = step;
    } else {
        breath_intensity = 511 - step;
    }

    // Determine which color to use based on breath cycle
    uint16_t current_hue;
    if (step < 256) {
        current_hue = dual_breath_state.color1_hue;
    } else {
        current_hue = dual_breath_state.color2_hue;
    }

    // Calculate final brightness (0-100 range)
    uint8_t final_brightness = (dual_breath_state.brightness * breath_intensity) / 255;

    // Apply color through ZMK's RGB underglow system
    struct zmk_periph_led_hsb color = {
        .h = current_hue,
        .s = dual_breath_state.saturation,
        .b = final_brightness
    };
    
    // Set the color for all LEDs
    zmk_rgb_underglow_set_hsb(color);

    dual_breath_state.animation_step++;
}

// Timer for animation
static struct k_timer dual_breath_timer;
static void dual_breath_timer_handler(struct k_timer *timer) {
    dual_breath_effect_step();
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                   struct zmk_behavior_binding_event event) {
    switch (binding->param1) {
    case RGB_DUAL_BREATH_TOG:
        dual_breath_state.enabled = !dual_breath_state.enabled;
        if (dual_breath_state.enabled) {
            k_timer_start(&dual_breath_timer, K_MSEC(50), K_MSEC(50));
        } else {
            k_timer_stop(&dual_breath_timer);
            // Turn off underglow
            zmk_rgb_underglow_off();
        }
        break;
    case RGB_DUAL_BREATH_ON:
        dual_breath_state.enabled = true;
        k_timer_start(&dual_breath_timer, K_MSEC(50), K_MSEC(50));
        break;
    case RGB_DUAL_BREATH_OFF:
        dual_breath_state.enabled = false;
        k_timer_stop(&dual_breath_timer);
        zmk_rgb_underglow_off();
        break;
    }
    
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_rgb_dual_breath_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

static int behavior_rgb_dual_breath_init(const struct device *dev) {
    k_timer_init(&dual_breath_timer, dual_breath_timer_handler, NULL);
    return 0;
}

#define RGB_DUAL_BREATH_INST(n)                                                                    \
    static struct behavior_rgb_dual_breath_data behavior_rgb_dual_breath_data_##n = {};           \
    static struct behavior_rgb_dual_breath_config behavior_rgb_dual_breath_config_##n = {         \
        .index = n,                                                                                \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_rgb_dual_breath_init, NULL,                              \
                           &behavior_rgb_dual_breath_data_##n,                                    \
                           &behavior_rgb_dual_breath_config_##n, POST_KERNEL,                    \
                           CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                  \
                           &behavior_rgb_dual_breath_driver_api);

DT_INST_FOREACH_STATUS_OKAY(RGB_DUAL_BREATH_INST)

#endif