/* Copyright 2021 Kyle McCreery
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap

enum layer_names {
    _BASE,
    _NAV,
    _MOUSE,
    _ADJUST
};

#define _BASE 0
#define _NAV 1
#define _MOUSE 2
#define _ADJUST 3
// variables for the alt tab timer

bool is_alt_gui_active = false;
bool is_alt_tab_active = false;
uint16_t alt_gui_timer = 0;

// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
                  KC_F1,   KC_F2,   KC_F3,   KC_F4,
				          KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
                  KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_MUTE,  KC_P4,   KC_P5,   KC_P6,   _______,
        TG(_MOUSE), KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        MO(_NAV),  KC_P0,   _______, KC_PDOT, _______,

				          KC_F5,   KC_F6,   KC_F7

    ),
    [_NAV] = LAYOUT(
                 _______,  _______, _______, _______,
                 KC_MEDIA_PREV_TRACK,  KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, KC_AUDIO_MUTE,
                 _______,   KC_UP,  KC_BSPC, _______,
        _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,
        TG(_ADJUST), _______,  _______, _______, _______,
        _______, _______,  _______, _______, _______,

                 _______,  _______, _______

    ),
    [_MOUSE] = LAYOUT(
                  _______, _______, _______, _______,
                  _______, KC_BSPC, _______, _______,
                  KC_MS_BTN1, KC_MS_UP, KC_MS_BTN2, KC_PAGE_UP,
        _______,  KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT, KC_PAGE_DOWN,
        _______,  _______, _______, _______, _______,
        KC_MS_ACCEL2,  TG(_BASE), _______, _______, _______,

                  _______, _______, _______
    ),
	  [_ADJUST] = LAYOUT(
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
                  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,
        TG(_BASE),  _______, _______, _______, _______,
        _______,  _______, _______, _______, _______,

                  _______, _______, _______

    )
};

// alt tab timer

void matrix_scan_user(void) {
  if (is_alt_gui_active) {
    if (timer_elapsed(alt_gui_timer) > 750) {
      unregister_code(KC_LGUI);
      is_alt_gui_active = false;
    }
  } else if (is_alt_tab_active) {
    if (timer_elapsed(alt_gui_timer) > 750) {
    unregister_code(KC_LALT);
    is_alt_tab_active = false;
  }
}
};


//layer switching for the encoder
bool encoder_update_user(uint8_t index, bool clockwise){
    if (index == 0 ) {
        if(get_highest_layer(layer_state|default_layer_state) == _BASE) {
            if (clockwise) {
                tap_code16(C(KC_TAB));
            } else {
                tap_code16(S(C(KC_TAB)));
            }
        } else if (get_highest_layer(layer_state|default_layer_state) == _NAV) {
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_gui_timer = timer_read();
                tap_code16(KC_TAB);
            } else if {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_gui_timer = timer_read();
                tap_code16(S(LALT(KC_TAB)));
            } else if (get_highest_layer(layer_state|default_layer_state) == _MOUSE) {
                if (clockwise) {
                    tap_code(KC_MS_WH_DOWN);
                } else {
                    tap_code(KC_MS_WH_UP);
                }
            } else if (get_highest_layer(layer_state|default_layer_state) == _ADJUST) {
                if (clockwise) {
                    tap_code(KC_MS_WH_DOWN);
                } else {
                    tap_code(KC_MS_WH_UP);
                    //I want this to actually end up controlling rgb brightness of the macropad but it is okay, we will make this work another time.
                    //Apparently I need special RGB codes to make that work, check out the docs later
                }
            }
        }
    }
return false;
};

//here is the rest of my encoder code, idk let's keep it around to be safe
/*    else if (index == 1) {
        bool encoder_update_user(bool clockwise){
            if(get_highest_layer(layer_state|default_layer_state) == _BASE) {
                if (clockwise) {
                    do clockwise shit
                } else {
                    do counterclockwise shit
                }
            }
        }
    }
}


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (get_highest_layer(layer_state|default_layer_state) == _BASE) {

// left room for an index check in case I want to utilize a second encoder and preserve multi-layer functionality
// I think that this will be more complicated in terms of raw lines of code but probably easier for me to parse through the logic/not have to fight through programming in the timer if I'm using the encoder map
        // Top rotary for Control + Tab (tab switching)
        if (clockwise) {
            tap_code16(C(KC_TAB));
        } else {
            tap_code16(S(C(KC_TAB)));
        }
    }
}

    else if (get_highest_layer(layer_state|default_layer_state) == _NAV) {
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_gui_timer = timer_read();
                tap_code16(KC_TAB);
            } else {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_gui_timer = timer_read();
                tap_code16(S(LALT(KC_TAB)));
            }

        }
    }

    else if (get_highest_layer(layer_state|default_layer_state) == _MOUSE) {
        if (index == 0 ) {


            if(clockwise) {
                tap_code(KC_MS_WH_DOWN);
            } else {
                tap_code(KC_MS_WH_UP);
            }

        }
    }

    else if (get_highest_layer(layer_state|default_layer_state) == _ADJUST) {
        if (index == 0 ) {


            if(clockwise) {
                tap_code(KC_PGDN);
            } else {
                tap_code(KC_PGUP);
            }

        }
    }
return false;*/


//oled cod(e LMAO forgot the e)
//first map defines MW logo
//second stuff is printing layer names

#ifdef OLED_ENABLE
	oled_rotation_t oled_init_user(oled_rotation_t rotation) {
		return OLED_ROTATION_270;       // flips the display 270 degrees
	}

	static void render_logo(void) {     // Render MechWild "MW" Logo
		static const char PROGMEM logo_1[] = {0x8A, 0x8B, 0x8C, 0x8D, 0x00};
		static const char PROGMEM logo_2[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0x00};
		static const char PROGMEM logo_3[] = {0xCA, 0xCB, 0xCC, 0xCD, 0x00};
		static const char PROGMEM logo_4[] = {0x20, 0x8E, 0x8F, 0x90, 0x00};
		oled_set_cursor(0,0);
		oled_write_P(logo_1, false);
		oled_set_cursor(0,1);
		oled_write_P(logo_2, false);
		oled_set_cursor(0,2);
		oled_write_P(logo_3, false);
		oled_set_cursor(0,3);
		oled_write_P(logo_4, false);
	}

	bool oled_task_user(void) {
		render_logo();
		oled_set_cursor(0,6);

		oled_write_ln_P(PSTR("Layer"), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case _NAV:
            oled_write_ln_P(PSTR("NAV"), false);
            break;
        case _MOUSE:
            oled_write_ln_P(PSTR("MOUSE"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("ADJUST"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
	oled_write_ln_P(PSTR(""), false);
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    return false;
	}
#endif

/*
encoder map, might be simpler than what I'm about to do but I don't know. I just want a good alt/tab lmao

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_BRID, KC_BRIU) },
    [_NAV]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_MOUSE]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_ADJUST]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

*/
