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
    _NUM,
    _OS,
    _MISC,
};

//-------------------------------------------------------------
// Tap dances.

enum tap_dance {
    TD_CTRL_ALT,
    TD_NAV_OS,
    TD_NUM_MISC,
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

// Number layer tap dance.
void td_num_misc_press(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_on(_NUM);
            break;
        case 2:
            layer_on(_MISC);
            break;
    }
}
void td_num_misc_release(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_off(_NUM);
            break;
        case 2:
            layer_off(_MISC);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CTRL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_ctrl_alt_press, td_ctrl_alt_release, NULL, NULL),
    [TD_NAV_OS]   = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_nav_os_press,   td_nav_os_release,   NULL, NULL),
    [TD_NUM_MISC]  = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_num_misc_press,  td_num_misc_release,  NULL, NULL),
};

//-------------------------------------------------------------
// Key overrides.

const key_override_t comma_comma = ko_make_basic(MOD_BIT(KC_LSFT), KC_COMM, KC_COMM);
const key_override_t dot_dot = ko_make_basic(MOD_BIT(KC_LSFT), KC_DOT, KC_DOT);
const key_override_t quote_quote = ko_make_basic(MOD_BIT(KC_LSFT), KC_QUOT, KC_QUOT);
const key_override_t minus_minus = ko_make_basic(MOD_BIT(KC_LSFT), KC_MINS, KC_MINS);
const key_override_t slash_slash = ko_make_basic(MOD_BIT(KC_LSFT), KC_SLSH, KC_SLSH);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &comma_comma,
    &dot_dot,
    &quote_quote,
    &minus_minus,
    &slash_slash,
    NULL // Null terminate the array of overrides!
};
//-------------------------------------------------------------

// Aliases for readability
#define NUM_MISC    TD(TD_NUM_MISC)
#define NAV_OS      TD(TD_NAV_OS)
#define CTRL_ALT    TD(TD_CTRL_ALT)
#define OSM_LSFT    OSM(MOD_LSFT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: modified Engram
 *
 * ,---------------------------------------.                                        ,--------------------------------------.
 * |     |  Z  |  Y  |  O   |  U  |    .    |                                        |   ,    |  L  |  D  |  W  |  Q  | Del |
 * |-----+-----+-----+------+-----+---------|                                        |--------+-----+-----+-----+-----+-----|
 * | Esc |  C  |  I  |  E   |  A  |    V    |                                        |   B    |  H  |  T  |  S  |  N  | Tab |
 * |-----+-----+-----+------+-----+---------+------------------.  ,------------------+--------+-----+-----+-----+-----+-----|
 * |     |  G  |  X  |  J   |  K  |    '    | Enter |   Sup    |  |   Sup    | Bksp  |   _    |  R  |  M  |  F  |  P  |     |
 * `-----------------+------+-----+---------+-------+----------|  |----------+-------+--------+-----+-----+-----------------'
 *                   | ONE  |     | Sym/Num | Shift | Ctrl/Alt |  | Ctrl/Alt | Space | Nav/OS |     |     |
 *                   | SHOT |     |         |       |          |  |          |       |        |     |     |
 *                   `-----------------------------------------'  `---------------------------------------'
 */
    [_ENGRAM] = LAYOUT(
     _______ , KC_Z , KC_Y , KC_O    , KC_U    , KC_DOT  ,                                             KC_COMM , KC_L    , KC_D    , KC_W , KC_Q , KC_DEL  ,
     KC_ESC  , KC_C , KC_I , KC_E    , KC_A    , KC_V    ,                                             KC_B    , KC_H    , KC_T    , KC_S , KC_N , KC_TAB  ,
     _______ , KC_G , KC_X , KC_J    , KC_K    , KC_QUOT , KC_ENT  , KC_LGUI ,     KC_LGUI , KC_BSPC , KC_UNDS , KC_R    , KC_M    , KC_F , KC_P , _______ ,
                             OS_TOGG , _______ , NUM_MISC, KC_LSFT , CTRL_ALT,     CTRL_ALT, KC_SPC  , NAV_OS  , _______ , _______
    ),

/*
 * Nav Layer: navigation and left symbols.
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  *   |  /   |  ^   |  `   |                              | PgUp | Home |   ↑  | End  |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  :   |  +   |  -   |  =   |  &   |                              | PgDn |  ←   |   ↓  |   →  |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  ~   |  |   |  <   |  >   |  %   |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      | Caps |      |  |      |      |      |      |      |
 *                        |      |      |      | Word |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      XXXXXXX, XXXXXXX, KC_ASTR, KC_SLSH, KC_CIRC, KC_GRV ,                                     KC_PGUP, KC_HOME, KC_UP  , KC_END , XXXXXXX, XXXXXXX,
      XXXXXXX, KC_COLN, KC_PLUS, KC_MINS, KC_EQL , KC_AMPR,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_TILD, KC_PIPE, KC_LT  , KC_GT  , KC_PERC, _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Num Layer: numbers and right symbols.
 *   
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  5   |  4   |  3   |      |                              |  $   |  #   |  [   |  ]   |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  .   |  2   |  1   |  0   |  9   |                              |  !   |  "   |  (   |  )   |  ;   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  -   |  8   |  7   |  6   |      |      |      |  |      |      |  ?   |  @   |  {   |  }   |  \   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUM] = LAYOUT(
     XXXXXXX, XXXXXXX, KC_5   , KC_4   , KC_3   , XXXXXXX,                                     KC_DLR , KC_HASH, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX,
     XXXXXXX, KC_DOT , KC_2   , KC_1   , KC_0   , KC_9   ,                                     KC_EXLM, KC_DQUO, KC_LPRN, KC_RPRN, KC_SCLN, XXXXXXX,
     XXXXXXX, KC_MINS, KC_8   , KC_7   , KC_6   , XXXXXXX, _______, _______, _______, _______, KC_QUES, KC_AT  , KC_LCBR, KC_RCBR, KC_BSLS, XXXXXXX,
                                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
    
/*
 * OS Layer: window management, media keys, function keys.
 *
 * ,--------------------------------------------------------.                              ,------------------------------------------------------------------.
 * |         |      |      |       | Sup+Spc | Sup+Shft+Del |                              | Sup+Shft+↑ | Sup+Shft+← | Sup+↑ | Sup+Shft+→ |      |            |
 * |---------+------+------+-------+---------+--------------|                              |------------+------------+-------+------------+------+------------|
 * | Sup+Esc |      |      | Sup+E | Sup+R   | Sup+T        |                              | Sup+Shft+↓ | Sup+←      | Sup+↓ | Sup+→      |      | Sup+Bckspc |
 * |---------+------+------+-------+---------+--------------+-------------.  ,-------------+------------+------------+-------+------------+------+------------|
 * |   F1    |  F2  |  F3  |  F4   |   F5    |      F6      |      |      |  |      |      |    F12     |    F11     |  F10  |     F9     |  F8  |     F7     |
 * `-----------------------+-------+---------+--------------+------+------|  |------+------+------------+------------+-------+--------------------------------'
 *                         |       |         |              |      |      |  |      |      |            |            |       |
 *                         |       |         |              |      |      |  |      |      |            |            |       |
 *                         `----------------------------------------------'  `-----------------------------------------------'
 */
    [_OS] = LAYOUT(
      XXXXXXX  , XXXXXXX, XXXXXXX, XXXXXXX, G(KC_SPC), C(A(KC_DEL)),                                     G(S(KC_UP))  , G(S(KC_LEFT)), G(KC_UP)  , G(S(KC_RGHT)), XXXXXXX, XXXXXXX   ,
      G(KC_ESC), XXXXXXX, XXXXXXX, G(KC_E), G(KC_R)  , G(KC_T)     ,                                     G(S(KC_DOWN)), G(KC_LEFT)   , G(KC_DOWN), G(KC_RGHT)   , XXXXXXX, G(KC_BSPC),
      KC_F1    , KC_F2  , KC_F3  , KC_F4  , KC_F5    , KC_F6       , _______, _______, _______, _______, KC_F12       , KC_F11       , KC_F10    , KC_F9        , KC_F8  , KC_F7     ,
                                   _______, _______  , _______     , _______, _______, _______, _______, _______      , _______      , _______
    ),

/*
 * Misc Layer: various other keys and combos.
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
    [_MISC] = LAYOUT(
      XXXXXXX, XXXXXXX, XXXXXXX, C(S(KC_C)), C(S(KC_V)), XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
      XXXXXXX, XXXXXXX, C(KC_X), C(KC_C)   , C(KC_V)   , C(KC_Z),                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX   , XXXXXXX   , XXXXXXX, _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
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
