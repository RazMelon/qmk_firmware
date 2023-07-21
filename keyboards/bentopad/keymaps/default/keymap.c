// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// bool is_alt_tab_active = false;
// uint16_t alt_gui_timer = 0;

/*
Linux

	layer 0 home
keys: ws1 ws2 ws3 ws4
encoder: move workspaces
layer key: tap = layer 1 hold = shift

	Layer 1
keys: trans
encoder: ctrl tab/ctrl shift tab
layer key:

	Layer 2
keys: trans
encoder: scroll up n down
layer key:

	Layer 3 media
keys: playpause, prev, next, idk?
encoder: volume
layer key: play/pause
encoder switch - home layer
*/
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0
     * ┌───┬───┬───────┐
     * │ 1 │ 2 │encoder│
     * ├───┼───┼───────┤
     * │ 3 │ 4 │ mo(1) │
     * └───┴───┴───────┘
     */
    [0] = LAYOUT_ortho_2x3(
        (LGUI(KC_1)),    (LGUI(KC_2)),    KC_ESCAPE,
        (LGUI(KC_3)),    (LGUI(KC_4)),    TG(1)
    ),
    /* 1
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [1] = LAYOUT_ortho_2x3(
        KC_TRNS,    KC_TRNS,    TG(0),
        KC_TRNS,    KC_TRNS,    TG(2)
    ),
        /* 2
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [2] = LAYOUT_ortho_2x3(
        KC_5,    KC_6,    TG(0),
        KC_7,    KC_8,    KC_ESCAPE
    ),
        /* 3
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [3] = LAYOUT_ortho_2x3(
        (LALT(KC_TAB)),    (LCTL(KC_TAB)),    TG(0),
        KC_MEDIA_PREV_TRACK,    KC_MEDIA_NEXT_TRACK, KC_MEDIA_PLAY_PAUSE
    ),
};

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if(get_highest_layer(layer_state|default_layer_state) == 0){
            if (clockwise) {
                tap_code16(LGUI(KC_RIGHT));
            } else {
                tap_code16(LGUI(KC_LEFT));
            }
        } else if(get_highest_layer(layer_state|default_layer_state) == 1){
            if (clockwise) {
                tap_code16(LGUI(KC_RIGHT));
            } else {
                tap_code16(LGUI(KC_LEFT));
            }
        } else if(get_highest_layer(layer_state|default_layer_state) == 2) {
            if (clockwise) {
                tap_code16(LGUI(KC_RIGHT));
            } else {
                tap_code16(LGUI(KC_LEFT));
            }
        } else if(get_highest_layer(layer_state|default_layer_state) == 3) {
            if (clockwise) {
                tap_code16(LGUI(KC_RIGHT));
            } else {
                tap_code16(LGUI(KC_LEFT));
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

/*
Linux

	layer 0 home
keys: ws1 ws2 ws3 ws4
encoder: move workspaces
layer key: tap = layer 1 hold = shift

	Layer 1
keys: trans
encoder: ctrl tab/ctrl shift tab
layer key:

	Layer 2
keys: trans
encoder: scroll up n down
layer key:

	Layer 3 media
keys: playpause, prev, next, idk?
encoder: volume
layer key: go to home
*/


/*
features I want
windows

windows

	layer 0
keys:
encoder: alt tab
layer key:

	Layer 1
keys:
encoder: ctrl tab
layer key:

	Layer 2
keys:
encoder:
layer key:

*/
