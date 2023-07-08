// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// bool is_alt_tab_active = false;
// uint16_t alt_gui_timer = 0;

// hey there! this is in the linux machine's dev branch of my qmk userspace!
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0
     * ┌───┬───┬───────┐
     * │ 1 │ 2 │encoder│
     * ├───┼───┼───────┤
     * │ 3 │ 4 │ mo(1) │
     * └───┴───┴───────┘
     */
    [0] = LAYOUT_ortho_2x3(
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
    [1] = LAYOUT_ortho_2x3(
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
