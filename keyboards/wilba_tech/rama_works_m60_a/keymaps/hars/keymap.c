/*
   ┳┏ ┳━┓┳━┓┳━┓
   ┣┻┓┣━ ┣━ ┃━┃
   ┇ ┛┻━┛┻━┛┇━┛
*/

#include QMK_KEYBOARD_H

#include "../../../wt_rgb_backlight.h"

enum layer_names {
    QWER,
    GAMING,
    HHKB,
    UTIL,
    LIGHTING,
};

enum custom_keycodes { FN_HHKB = SAFE_RANGE, FN_UTIL, EFFECT, EFFECT_END = EFFECT + 20 };

extern backlight_config g_config;
static uint8_t          last_effect;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FN_HHKB:
            if (record->event.pressed) {
                layer_on(2);
                update_tri_layer(2, 3, 4);
            } else {
                layer_off(2);
                update_tri_layer(2, 3, 4);
            }
            return false;
            break;
        case FN_UTIL:
            if (record->event.pressed) {
                layer_on(3);
                update_tri_layer(2, 3, 4);
            } else {
                layer_off(3);
                update_tri_layer(2, 3, 4);
            }
            return false;
            break;
        case BL_TOGG:
            if (record->event.pressed) {
                if (g_config.effect) {
                    last_effect     = g_config.effect;
                    g_config.effect = 0;
                } else {
                    g_config.effect = last_effect;
                }
            }
            return false;
            break;
        case EFFECT ... EFFECT_END:
            if (record->event.pressed) {
                uint8_t effect = keycode - EFFECT;
                if (effect == g_config.effect) effect = 0; // Toggle effect on second press
                g_config.effect = effect;
                backlight_config_save();
            }
            return false;
            break;
        case TAB_FN:
            if (record->tap.count && record->event.pressed) {
                tap_code(KC_TAB);
                return false;
            } else if (record->event.pressed) {
                register_code(KC_CAPS);
                /* layer_on(2); */
            } else {
                unregister_code(KC_CAPS);
                /* layer_off(2); */
            }
            return false;
            break;
        default:
            return true;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TAB_FN:
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

void keyboard_post_init_user(void) {
    g_config.effect                    = 8;
    g_config.effect_speed              = 1;
    g_config.disable_after_timeout     = 15;
    g_config.layer_1_indicator.color.h = 21;
    g_config.layer_1_indicator.color.s = 255;
    g_config.layer_1_indicator.index   = 254;
    g_config.layer_2_indicator.color.h = 120;
    g_config.layer_2_indicator.color.s = 255;
    g_config.layer_2_indicator.index   = 254;
    g_config.layer_3_indicator.color.h = 204;
    g_config.layer_3_indicator.color.s = 255;
    g_config.layer_3_indicator.index   = 254;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Layer 0
[QWER] = LAYOUT_60_hhkb(
	QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, KC_BSPC,
	 TAB_FN, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
	KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
	KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, FN_UTIL,
	         KC_LALT, KC_LGUI,                            LT(UTIL, KC_SPC),                   FN_HHKB, KC_HYPR),
// Layer 1
[GAMING] = LAYOUT_60_hhkb(
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______,
	 KC_TAB, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	         KC_LGUI, KC_LALT,                             KC_SPC,                            _______, _______),
// Layer 2
[HHKB] = LAYOUT_60_hhkb(
	KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL,
	KC_CAPS, _______, KC_UP,   _______, _______, _______, KC_INS , KC_EJCT, KC_PSCR, KC_SCRL, KC_PAUS, _______,   _______, _______,
	_______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, _______, _______, _______,
	_______, _______, _______, _______, _______, _______, KC_PPLS, KC_PMNS, KC_END,  KC_PGDN, _______, _______, _______,
	         _______, _______,                            _______,                            _______, _______),
// Layer 3
[UTIL] = LAYOUT_60_hhkb(
	KC_MUTE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SLEP,
	_______, KC_MPLY, KC_VOLU, KC_MUTE, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______, _______, _______,
	_______, KC_MRWD, KC_VOLD, KC_MFFD, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	         _______, _______,                            _______,                            _______, _______),
// Layer 4
[LIGHTING] = LAYOUT_60_hhkb(
	QK_BOOT,   EF_DEC,   EF_INC,  H1_DEC,  H1_INC,  H2_DEC,  H2_INC, _______, _______, _______, _______, BR_DEC,  BR_INC,  _______, _______,
	TG_GAME, _______,  BL_TOGG,  S1_DEC,  S1_INC,  S2_DEC,  S2_INC, _______, _______, _______, _______, ES_DEC,  ES_INC,  _______,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
	         _______, _______,                            _______,                            _______, _______),
};
