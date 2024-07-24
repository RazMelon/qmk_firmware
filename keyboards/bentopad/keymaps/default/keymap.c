// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "pimoroni_trackball.h"
#include "pointing_device.h"
#include "i2c_master.h"
// bool is_alt_tab_active = false;
// uint16_t alt_gui_timer = 0;

enum layer_names {
    _BASE,
    _NUM,
	_SYM,
    _ADJUST,
    _MOUS
};

enum custom_keycodes {
  BALL_HUE, //hold + scroll ball up and down to cycle hue
  BALL_SAT,//hold + scroll ball up and down to cycle saturation
  BALL_VAL,//hold + scroll ball up and down to cycle value
  BALL_RCL,
  BALL_DRG,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0
     * ┌───┬───┬───────┐
     * │ 1 │ 2 │encoder│
     * ├───┼───┼───────┤
     * │ 3 │ 4 │ mo(1) │
     * └───┴───┴───────┘
     */
    [_BASE] = LAYOUT_ortho_2x3(
        (LCTL(KC_Z)),    (LCTL(LSFT(KC_Z))),    KC_8,
        (LCTL(KC_X)),    (LCTL(KC_C)),    (LCTL(KC_V))
    ),
    /* sel
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [_NUM] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    KC_MUTE,
        KC_7,    KC_8,    TG(0)
    ),
    [_SYM] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    KC_MUTE,
        KC_7,    KC_8,    TG(0)
    ),
    [_ADJUST] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    KC_MUTE,
        KC_7,    KC_8,    TG(0)
    ),
    [_MOUS] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    KC_MUTE,
        KC_7,    KC_8,    TG(0)
    ),
};

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code16(LGUI(KC_RIGHT));
        } else {
            tap_code16(LGUI(KC_LEFT));
        }
    }
    return true;
}
#endif
// bool encoder_update_user(uint8_t index, bool clockwise) {
//     if (index == 0) {
//         // Middle rotary for Alt + Tab (Windows window switching)
//         if (clockwise) {
//             if (!is_alt_tab_active) {
//                 is_alt_tab_active = true;
//                 register_code(KC_LALT);
//             }
//             alt_gui_timer = timer_read();
//             tap_code16(KC_TAB);
//         } else {
//             if (!is_alt_tab_active) {
//                 is_alt_tab_active = true;
//                 register_code(KC_LALT);
//             }
//             alt_gui_timer = timer_read();
//             tap_code16(S(LALT(KC_TAB)));
//         }
//     }
//     return true;
// }
// #endif

#include "timer.h"
#include "math.h"
#include "quantum/quantum.h"

static int16_t mouse_auto_layer_timer = 0;
static bool hue_mode_enabled = 0;
static bool saturation_mode_enabled = 0;
static bool value_mode_enabled = 0;
static bool right_click_mode_enabled = 0;
static bool drag_mode_enabled = 0;
#define MOUSE_TIMEOUT 1000
#define TRACKBALL_TIMEOUT 5

#define SIGN(x) ((x > 0) - (x < 0))

// user config EEPROM stuff {{{
typedef union {
  uint32_t raw;
  struct {
	uint8_t tb_hue :8;
    uint8_t tb_value :8;
	uint8_t tb_saturation :8;
  };
} user_config_t;

user_config_t user_config;

static uint8_t tb_hue = 1;
static uint8_t tb_saturation = 255;
static uint8_t tb_value = 255;


void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  tb_hue = user_config.tb_hue;
  tb_saturation = user_config.tb_saturation;
  tb_value = user_config.tb_value;
  trackball_set_hsv(tb_hue, tb_saturation,tb_value);
}

void eeconfig_init_user(void) {
  user_config.raw = 0;
  user_config.tb_hue = 80;
  user_config.tb_saturation = 80;
  user_config.tb_value = 80;
  eeconfig_update_user(user_config.raw);
}
// }}}

void matrix_init_user() {
    trackball_init();
}

void suspend_power_down_user(void) {
    trackball_set_brightness(0);
    /* trackball_sleep(); */
}

__attribute__((weak)) void pointing_device_init(void) { trackball_set_rgbw(0,0,0,tb_value); }

void update_member(int8_t* member, int16_t* offset) {//{{{
    if (*offset > 127) {
        *member = 127;
        *offset -= 127;
    } else if (*offset < -127) {
        *member = -127;
        *offset += 127;
    } else {
        *member = *offset;
        *offset = 0;
    }
}//}}}

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t v_offset = 0;
static int16_t h_offset = 0;
static int16_t tb_timer = 0;
void pointing_device_task() {
    report_mouse_t mouse = pointing_device_get_report();

    if (trackball_get_interrupt() && (!tb_timer || timer_elapsed(tb_timer) > TRACKBALL_TIMEOUT)) {
        tb_timer = timer_read() | 1;

        trackball_state_t state = trackball_get_state();

        uint8_t mods;
        if (state.x || state.y) {
            mods = get_mods();
        }

        if (state.button_triggered && (right_click_mode_enabled == 1)) {
            if(state.button_down) {
                mouse.buttons |= MOUSE_BTN2;
            } else {
                mouse.buttons &= ~MOUSE_BTN2;
            }
            pointing_device_set_report(mouse);
		pointing_device_send();
		} else if (state.button_triggered && (drag_mode_enabled == 1)) {
			if (state.button_down){
			mouse.buttons |= MOUSE_BTN1;
			pointing_device_set_report(mouse);
            pointing_device_send();}
		} else if (state.button_triggered) {
			if(state.button_down) {
				mouse.buttons |= MOUSE_BTN1;
			} else {
				mouse.buttons &= ~MOUSE_BTN1;
			}
			pointing_device_set_report(mouse);
            pointing_device_send();
        } else {
			// on the ADJUST layer with BALL_VAL held, roll ball downwards to change trackball value
            if (layer_state_is(_ADJUST) && value_mode_enabled == 1) {
                tb_value += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation, tb_value | 1);
			// on the ADJUST layer with BALL_SAT held, roll ball downwards to change trackball saturation
			} else if (layer_state_is(_ADJUST) && saturation_mode_enabled == 1) {
				tb_saturation += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation | 1, tb_value);
			// on the ADJUST layer, or with BALL_HUE held, roll ball downwards to change trackball hue
			} else if (layer_state_is(_ADJUST) || hue_mode_enabled == 1) {
				tb_hue += state.y;
                trackball_set_hsv(tb_hue | 1, tb_saturation, tb_value);
			// on the NUM layer, trackball behaves as vertical scroll
            } else if (layer_state_is(_NUM) || layer_state_is(_SYM)) {
                h_offset += (state.x);
                v_offset -= (state.y);
            } else if ((state.x || state.y) && !state.button_down) {

			if (!mouse_auto_layer_timer && !layer_state_is(_NUM)) {
                    layer_on(_MOUS);
                }
                mouse_auto_layer_timer = timer_read() | 1;

                uint8_t scale = 3;
                if (mods & MOD_MASK_CTRL) scale = 2;
                x_offset += state.x * state.x * SIGN(state.x) * scale;
                y_offset += state.y * state.y * SIGN(state.y) * scale;

            }
        }

        /* xprintf("%d, %d, %d, %d, %d\n", */
        /*         s[0], s[1], */
        /*         s[2], s[3], */
        /*         s[4]); */

    }

    while (x_offset || y_offset || h_offset || v_offset) {
        update_member(&mouse.x, &x_offset);
        update_member(&mouse.y, &y_offset);

        update_member(&mouse.v, &v_offset);
        update_member(&mouse.h, &h_offset);

        pointing_device_set_report(mouse);
        pointing_device_send();
    }
}

void matrix_scan_user(void) {/*{{{*/
    if (mouse_auto_layer_timer && timer_elapsed(mouse_auto_layer_timer) > MOUSE_TIMEOUT) {
        report_mouse_t rep = pointing_device_get_report();
        if (rep.buttons) { return; }
        layer_off(_MOUS);
        mouse_auto_layer_timer = 0;
    }
}/*}}}*/

bool process_record_user(uint16_t keycode, keyrecord_t *record) {/*{{{*/
    switch (keycode) {
    case BALL_HUE:
        if (record->event.pressed) {
        hue_mode_enabled = 1;
        } else {
        hue_mode_enabled = 0;
        }
        break;
	case BALL_SAT:
		if (record->event.pressed) {
        saturation_mode_enabled = 1;
        } else {
        saturation_mode_enabled = 0;
        }
        break;
	case BALL_VAL:
		if (record->event.pressed) {
        value_mode_enabled = 1;
        } else {
        value_mode_enabled = 0;
        }
        break;
	case BALL_RCL:
	if (record->event.pressed) {
        right_click_mode_enabled = 1;
        } else {
        right_click_mode_enabled = 0;
        }
        break;
	case BALL_DRG:
	if (record->event.pressed) {
        drag_mode_enabled = 1;
        } else {
        drag_mode_enabled = 0;
        }
        break;
/*   case BALL_NCL:
     record->event.pressed?register_code(KC_BTN1):unregister_code(KC_BTN1);
     break;
  case BALL_RCL:
      record->event.pressed?register_code(KC_BTN2):unregister_code(KC_BTN2);
      break;
  case BALL_MCL:
      record->event.pressed?register_code(KC_BTN3):unregister_code(KC_BTN3);
      break; */
    }

	if ((keycode < KC_BTN1 || ((keycode > KC_BTN5) && (keycode < SAFE_RANGE)))
			&& layer_state_is(_MOUS)
            && record->event.pressed) {
        layer_off(_MOUS);
        mouse_auto_layer_timer = 0;
    }
    return true;

}/*}}}*/


static uint8_t last_layer = _BASE;
layer_state_t layer_state_set_user(layer_state_t state) {/*{{{*/
    uint8_t layer = get_highest_layer(state);

    switch(layer) {
        case _NUM:
            trackball_set_hsv(tb_hue + 20, tb_saturation, ((tb_value-30) > 0 ? (tb_value-30) : 0));
            break;
		case _SYM:
			trackball_set_hsv(tb_hue - 20, tb_saturation, ((tb_value-30) > 0 ? (tb_value-30) : 0));
			break;
        case _ADJUST:
            trackball_set_hsv(tb_hue, ((tb_saturation+20) < 255 ? (tb_saturation+20) : 255), tb_value);
            break;
        case _MOUS:
            trackball_set_hsv(tb_hue, tb_saturation, tb_value);
            break;
        default:
            trackball_set_hsv(tb_hue, tb_saturation, ((tb_value-30) > 0 ? (tb_value-30) : 0));
            break;
    }

    // if layer *was* ADJUST, save eeprom
    if (
            last_layer == _ADJUST
            && last_layer != layer
            && (tb_value != user_config.tb_value || tb_hue != user_config.tb_hue || tb_saturation != user_config.tb_saturation)
       ) {
        user_config.tb_value = tb_value;
		user_config.tb_hue = tb_hue;
		user_config.tb_saturation = tb_saturation;
        eeconfig_update_user(user_config.raw);
    }

    last_layer = layer;
    return state;
}