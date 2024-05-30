/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

// Disable bright red controller light.
void keyboard_pre_init_user(void) {
  // Set our LED pin as output
  setPinOutput(24);
  // Turn the LED off
  // (Due to technical reasons, high is off and low is on)
  writePinHigh(24);
}

enum layers {
    _ENGRAM = 0,
    _NAV,
    _OS,
    _SYM,
    _HOTKEY,
};

//-------------------------------------------------------------
// Tap dances.

enum tap_dance {
    TD_CTRL_ALT,
    TD_NAV_OS,
    TD_SYM_HOT,
};

// Ctrl/Alt tap dance.
void td_ctrl_alt_press(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            register_code(KC_LCTL);
            break;
        case 2:
            register_code(KC_LALT);
            break;
        case 3:
            register_code(KC_LCTL);
            register_code(KC_LALT);
            break;
    }
}
void td_ctrl_alt_release(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            unregister_code(KC_LCTL);
            break;
        case 2:
            unregister_code(KC_LALT);
            break;
        case 3:
            unregister_code(KC_LALT);
            unregister_code(KC_LCTL);
            break;
    }
}

// Navigation / OS layer tap dance.
void td_nav_os_press(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_on(_NAV);
            break;
        case 2:
            layer_on(_OS);
            break;
    }
}
void td_nav_os_release(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_off(_NAV);
            break;
        case 2:
            layer_off(_OS);
            break;
    }
}

// Symbol / number layer tap dance.
void td_sym_hot_press(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_on(_SYM);
            break;
        case 2:
            layer_on(_HOTKEY);
            break;
    }
}
void td_sym_hot_release(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_off(_SYM);
            break;
        case 2:
            layer_off(_HOTKEY);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CTRL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_ctrl_alt_press, td_ctrl_alt_release, NULL, NULL),
    [TD_NAV_OS]   = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_nav_os_press,   td_nav_os_release,   NULL, NULL),
    [TD_SYM_HOT]  = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_sym_hot_press,  td_sym_hot_release,  NULL, NULL),
};

//-------------------------------------------------------------

// Aliases for readability
#define SYM_HOT     TD(TD_SYM_HOT)
#define NAV_OS      TD(TD_NAV_OS)
#define CTRL_ALT    TD(TD_CTRL_ALT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: Engram (but with custom punctuation layout)
 *
 * ,------------------------------------------.                                        ,------------------------------------------.
 * | Tab |  B  |  Y  |   O  |   U  |   / ?    |                                        |   , <    |   L  |   D  |  W  |  V  |  Z  |
 * |-----+-----+-----+------+------+----------|                                        |----------+------+------+-----+-----+-----|
 * | Esc |  C  |  I  |   E  |   A  |   ' "    |                                        |   . >    |   H  |   T  |  S  |  N  |  Q  |
 * |-----+-----+-----+------+------+----------+------------------.  ,------------------+----------+------+------+-----+-----+-----|
 * |     |  G  |  X  |   J  |   K  |   - _    | Enter |   Sup    |  |  Sup     | Bksp  |   ; :    |   R  |   M  |  F  |  P  |     |
 * `-----------------+------+------+----------+-------+----------|  |----------+-------+----------+------+------+-----------------'
 *                   |      |      | Sym/Num  | Shift | Ctrl/Alt |  | Ctrl/Alt | Space | Nav/OS   |      |      |
 *                   |      |      |          |       |          |  |          |       |          |      |      |
 *                   `-------------------------------------------'  `-------------------------------------------'
 */
    [_ENGRAM] = LAYOUT(
     KC_TAB  ,  KC_B , KC_Y , KC_O    , KC_U    , KC_SLSH ,                                            KC_COMM, KC_L    , KC_D    , KC_W , KC_V , KC_Z    ,
     KC_ESC  ,  KC_C , KC_I , KC_E    , KC_A    , KC_QUOT ,                                            KC_DOT  , KC_H    , KC_T    , KC_S , KC_N , KC_Q    ,
     XXXXXXX ,  KC_G , KC_X , KC_J    , KC_K    , KC_MINS , KC_ENT  , KC_LGUI ,     KC_LGUI , KC_BSPC, KC_SCLN, KC_R    , KC_M    , KC_F , KC_P , XXXXXXX ,
                              _______ , _______ , SYM_HOT , KC_LSFT , CTRL_ALT,     CTRL_ALT, KC_SPC , NAV_OS  , _______ , _______
    ),

/*
 * Nav Layer: navigation and F-keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  |  F10 |  F11 |  F12 |      |                              | PgUp | Home |   ↑  | End  |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              | PgDn |  ←   |   ↓  |   →  |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      XXXXXXX, KC_F9  , KC_F10 , KC_F11 , KC_F12 , XXXXXXX,                                     KC_PGUP, KC_HOME, KC_UP  , KC_END , XXXXXXX, XXXXXXX,
      XXXXXXX, KC_F5  , KC_F6  , KC_F7  , KC_F8  , XXXXXXX,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_F1  , KC_F2  , KC_F3  , KC_F4  , XXXXXXX, _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    
/*
 * OS Layer: window management, media keys.
 *
 * ,--------------------------------------------------------.                              ,------------------------------------------------------------------.
 * |         |      |      |       | Sup+Spc | Sup+Shft+Del |                              | Sup+Shft+↑ | Sup+Shft+← | Sup+↑ | Sup+Shft+→ |      |            |
 * |---------+------+------+-------+---------+--------------|                              |------------+------------+-------+------------+------+------------|
 * | Sup+Esc |      |      | Sup+E | Sup+R   | Sup+T        |                              | Sup+Shft+↓ | Sup+←      | Sup+↓ | Sup+→      |      | Sup+Bckspc |
 * |---------+------+------+-------+---------+--------------+-------------.  ,-------------+------------+------------+-------+------------+------+------------|
 * |         |      |      |       |         |              |      |      |  |      |      |            |            |       |            |      |            |
 * `-----------------------+-------+---------+--------------+------+------|  |------+------+------------+------------+-------+--------------------------------'
 *                         |       |         |              |      |      |  |      |      |            |            |       |
 *                         |       |         |              |      |      |  |      |      |            |            |       |
 *                         `----------------------------------------------'  `-----------------------------------------------'
 */
    [_OS] = LAYOUT(
      XXXXXXX  , XXXXXXX, XXXXXXX, XXXXXXX, G(KC_SPC), C(A(KC_DEL)),                                     G(S(KC_UP))  , G(S(KC_LEFT)), G(KC_UP)  , G(S(KC_RGHT)), XXXXXXX, XXXXXXX   ,
      G(KC_ESC), XXXXXXX, XXXXXXX, G(KC_E), G(KC_R)  , G(KC_T)     ,                                     G(S(KC_DOWN)), G(KC_LEFT)   , G(KC_DOWN), G(KC_RGHT)   , XXXXXXX, G(KC_BSPC),
      XXXXXXX  , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX  , XXXXXXX     , _______, _______, _______, _______, _______      , XXXXXXX      , XXXXXXX   , XXXXXXX      , XXXXXXX, XXXXXXX   ,
                                   _______, _______  , _______     , _______, _______, _______, _______, _______      , _______      , _______
    ),

/*
 * Sym Layer: symbols and numbers
 *   
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  ~   |  &   |  [   |  ]   |  *   |                              |  `   |  7   |  8   |  9   |  @   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  %   |  !   |  (   |  )   |  =   |                              |  0   |  4   |  5   |  6   |  #   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  ^   |  |   |  {   |  }   |  +   |      |      |  |      | Del  |  \   |  1   |  2   |  3   |  $   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |  _   |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
     XXXXXXX , KC_TILD, KC_AMPR, KC_LBRC, KC_RBRC, KC_ASTR,                                     KC_GRV , KC_7   , KC_8   , KC_9   , KC_AT  , XXXXXXX,
     XXXXXXX , KC_PERC, KC_EXLM, KC_LPRN, KC_RPRN, KC_EQL ,                                     KC_0   , KC_4   , KC_5   , KC_6   , KC_HASH, XXXXXXX,
     XXXXXXX , KC_CIRC, KC_PIPE, KC_LCBR, KC_RCBR, KC_PLUS, _______, _______, _______, KC_DEL , KC_BSLS, KC_1   , KC_2   , KC_3   , KC_DLR , XXXXXXX,
                                 _______, _______, _______, _______, _______, _______, KC_UNDS, _______, _______, _______
    ),

/*
 * Hotkeys: easy access to common application hotkeys
 *
 * ,------------------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | Shft-Copy | Shft-Paste |      |                              |      |      |      |      |      |        |
 * |--------+------+------+-----------+------------+------|                              |------+------+------+------+------+--------|
 * |        |      | Cut  | Copy      | Paste      | Undo |                              |      |      |      |      |      |        |
 * |--------+------+------+-----------+------------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |           |            |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+-----------+------------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |           |            |      |      |      |  |      |      |      |      |      |
 *                        |           |            |      |      |      |  |      |      |      |      |      |
 *                        `---------------------------------------------'  `----------------------------------'
 */
    [_HOTKEY] = LAYOUT(
      XXXXXXX, XXXXXXX, XXXXXXX, C(S(KC_C)), C(S(KC_V)), XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, C(KC_X), C(KC_C)   , C(KC_V)   , C(KC_Z),                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX   , XXXXXXX   , XXXXXXX, _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                 _______   , _______   , _______, _______, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};
