// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0
     * ┌───┬───┬───────┐
     * │ 1 │ 2 │encoder│
     * ├───┼───┼───────┤
     * │ 3 │ 4 │ mo(1) │
     * └───┴───┴───────┘
     */
    [0] = LAYOUT_ortho_2x3(
        KC_1,    KC_2,    KC_MUTE,
        KC_3,    KC_4,    TG(sel)
    ),
    /* sel
     * ┌─────┬─────┬─────┐
     * │tg(1)│tg(2)│ ENC │
     * ├─────┼─────┼─────┤
     * │ tg(3│tg(4)│tg(0)│
     * └─────┴─────┴─────┘
     */
    [sel] = LAYOUT_ortho_2x3(
        KC_1,    KC_2,    KC_MUTE,
        KC_3,    KC_4,    KC_TRANS
    ),
    /* 1
     * ┌───┬───┬───┐
     * │ 1 │ 2 │ENC│
     * ├───┼───┼───┤
     * │ 3 │ 4 │ 5 │
     * └───┴───┴───┘
     */
    [1] = LAYOUT_ortho_2x3(
        KC_1,    KC_1,    KC_MUTE,
        KC_1,    KC_1,    TG(0)
    )
};
