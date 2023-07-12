// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
bool is_alt_tab_active = false;
uint16_t alt_gui_timer = 0;

enum layer_names {
    _BASE,
    _NAV,
    _ADJUST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* _BASE
     * ┌───┬───┬───────┐
     * │ 1 │ 2 │encoder│
     * ├───┼───┼───────┤
     * │ 3 │ 4 │ mo(1) │
     * └───┴───┴───────┘
     */
    [_BASE] = LAYOUT_ortho_2x3(
        (LGUI(KC_1)),    (LGUI(KC_2)),    TG(_NAV),
        (LGUI(KC_3)),    (LGUI(KC_4)),    KC_LSFT
    ),
    /* _NAV
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [_NAV] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    TG(_ADJUST),
        KC_7,    KC_8,    KC_2
    ),
        /* _ADJUST
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [_ADJUST] = LAYOUT_ortho_2x3(
        KC_9,    KC_0,    TO(_BASE),
        KC_1,    KC_2,    TO(_BASE)
    ),
};

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (get_highest_layer(layer_state|default_layer_state) == _BASE) {
            if (clockwise) {
                tap_code16(LGUI(KC_RIGHT));
            } else {
                tap_code16(LGUI(KC_LEFT));
            }
        } else if (get_highest_layer(layer_state|default_layer_state) == _NAV) {
            if (clockwise) {
                tap_code16(LCTL(KC_TAB));
            } else {
                tap_code16(LCTL(LSFT(KC_TAB)));
            }
        } else if (get_highest_layer(layer_state|default_layer_state) == _ADJUST) {
            if (clockwise) {
                tap_code(KC_MS_WH_DOWN);
            } else {
                tap_code(KC_MS_WH_UP);
            }
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
