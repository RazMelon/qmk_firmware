// Copyright 2023 RazMelon (@RazMelon)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#define DYNAMIC_KEYMAP_LAYER_COUNT 5
#define USE_I2C
#define PIMORONI_TRACKBALL_ADDRESS 0x0A
#define WS2812_DI_PIN E2
#ifdef WS2812_DI_PIN
  #define RGBLED_NUM 11
  #define RGBLIGHT_HUE_STEP 16
  #define RGBLIGHT_SAT_STEP 16
  #define RGBLIGHT_VAL_STEP 16
  #define RGBLIGHT_LIMIT_VAL 255 /* The maximum brightness level */
  #define RGBLIGHT_SLEEP
#endif
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
