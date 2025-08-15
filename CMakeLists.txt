# Copyright (c) 2024 ZMK Contributors
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_RGB_DUAL_BREATH)
  target_sources(zmk PRIVATE src/behaviors/behavior_rgb_dual_breath.c)
  target_include_directories(zmk PRIVATE include)
endif()