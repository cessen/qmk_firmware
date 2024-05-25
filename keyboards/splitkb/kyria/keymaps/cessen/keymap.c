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
    _NUM,
    // _ADJUST,
};

//-------------------------------------------------------------
// Tap dances.

enum tap_dance {
    TD_CTRL_ALT,
    TD_NAV_OS,
    TD_SYM_NUM,
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
            unregister_code(KC_LCTL);
            unregister_code(KC_LALT);
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
void td_sym_num_press(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_on(_SYM);
            break;
        case 2:
            layer_on(_NUM);
            break;
    }
}
void td_sym_num_release(tap_dance_state_t *state, void *user_data)
{
    switch (state->count) {
        case 1:
            layer_off(_SYM);
            break;
        case 2:
            layer_off(_NUM);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_CTRL_ALT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_ctrl_alt_press, td_ctrl_alt_release, NULL, NULL),
    [TD_NAV_OS]   = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_nav_os_press,   td_nav_os_release,   NULL, NULL),
    [TD_SYM_NUM]  = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(td_sym_num_press,  td_sym_num_release,  NULL, NULL),
};

//-------------------------------------------------------------


// // Aliases for readability
// #define QWERTY   DF(_QWERTY)
// #define COLEMAK  DF(_COLEMAK_DH)
// #define DVORAK   DF(_DVORAK)

#define SYM_NUM     TD(TD_SYM_NUM)
#define NAV_OS      TD(TD_NAV_OS)
#define CTRL_ALT    TD(TD_CTRL_ALT)
// #define FKEYS    MO(_FUNCTION)
// #define ADJUST   MO(_ADJUST)

// #define CTL_ESC  MT(MOD_LCTL, KC_ESC)
// #define CTL_QUOT MT(MOD_RCTL, KC_QUOTE)
// #define CTL_MINS MT(MOD_RCTL, KC_MINUS)
// #define ALT_ENT  MT(MOD_LALT, KC_ENT)

// Note: LAlt/Enter (ALT_ENT) is not the same thing as the keyboard shortcut Alt+Enter.
// The notation `mod/tap` denotes a key that activates the modifier `mod` when held down, and
// produces the key `tap` when tapped (i.e. pressed and released).

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: ENGRAM
 *
 * ,------------------------------------------------.                                    ,-----------------------------------------------.
 * | Tab       |  B  |  Y  |   O  |   U  |   ' "    |                                    |   ; :    |   L  |   D  |  W  |  V  |  Z       |
 * |-----------+-----+-----+------+------+----------|                                    |----------+------+------+-----+-----+----------|
 * | Esc       |  C  |  I  |   E  |   A  |   , <    |                                    |   . >    |   H  |   T  |  S  |  N  |  Q       |
 * |-----------+-----+-----+------+------+----------+-----------------.  ,---------------+----------+------+------+-----+-----+----------|
 * | Ctrl/Alt  |  G  |  X  |   J  |   K  |   / ?    | Enter |   Sup   |  |  Sup   | Bksp |   - _    |   R  |   M  |  F  |  P  | Ctrl/Alt |
 * `-----------------------+------+------+----------+-------+---------|  |--------+------+----------+------+------+----------------------'
 *                         |      |      | Sym/Num  | Shift | Sym/Num |  | Nav/OS | Space| Nav/OS   |      |      |
 *                         |      |      |          |       |         |  |        |      |          |      |      |
 *                         `------------------------------------------'  `----------------------------------------'
 */
    [_ENGRAM] = LAYOUT(
     KC_TAB  ,  KC_B , KC_Y , KC_O    , KC_U    , KC_QUOT ,                                            KC_SCLN , KC_L    , KC_D    , KC_W , KC_V , KC_Z   ,
     KC_ESC  ,  KC_C , KC_I , KC_E    , KC_A    , KC_COMM ,                                            KC_DOT  , KC_H    , KC_T    , KC_S , KC_N , KC_Q   ,
     CTRL_ALT,  KC_G , KC_X , KC_J    , KC_K    , KC_SLSH , KC_ENT  , KC_LGUI ,     KC_LGUI , KC_BSPC, KC_MINS , KC_R    , KC_M    , KC_F , KC_P , CTRL_ALT,
                              _______ , _______ , SYM_NUM , KC_LSFT , SYM_NUM ,     NAV_OS  , KC_SPC , NAV_OS  , _______ , _______
    ),

/*
 * Nav Layer: navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home |   ↑  | End  |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | PgDn |  ←   |   ↓  |   →  |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_PGUP, KC_HOME, KC_UP  , KC_END , _______, _______,
      _______, _______, _______, _______, _______, _______,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
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
      _______     , _______, _______, _______   , LGUI(KC_SPC), LGUI(LALT(KC_DEL)),                                     LGUI(LSFT(KC_UP)),   LGUI(LSFT(KC_LEFT)), LGUI(KC_UP)  , LGUI(LSFT(KC_RGHT)), _______, _______      ,
      LGUI(KC_ESC), _______, _______, LGUI(KC_E), LGUI(KC_R)  , LGUI(KC_T)        ,                                     LGUI(LSFT(KC_DOWN)), LGUI(KC_LEFT)      , LGUI(KC_DOWN), LGUI(KC_RGHT)      , _______, LGUI(KC_BSPC),
      _______     , _______, _______, _______   , _______     , _______           , _______, _______, _______, _______, _______            , _______            , _______      , _______            , _______, _______      ,
                                      _______   , _______     , _______           , _______, _______, _______, _______, _______            , _______            , _______
    ),

/*
 * Sym Layer: symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |  %   |  ^   |  ~   |                              |   @  |  [   |  ]   |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |  $   |  *   |  +   |  !   |                              |   `  |  (   |  )   |  =   |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |  &   |  |   |  \   |      |      |  |      |      |   #  |  {   |  }   |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
     _______ , _______, _______, KC_PERC, KC_CIRC, KC_TILD,                                     KC_AT  , KC_LBRC, KC_RBRC, _______, _______, _______,
     _______ , _______, KC_DLR , KC_ASTR, KC_PLUS, KC_EXLM,                                     KC_GRV , KC_LPRN, KC_RPRN, KC_EQL , _______, _______,
     _______ , _______, _______, KC_AMPR, KC_PIPE, KC_BSLS, _______, _______, _______, _______, KC_HASH, KC_LCBR, KC_RCBR, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Number Layer: numbers, function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F9  | F10  | F11  | F12  |      |                              |   .  |  7   |  8   |  9   |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |                              |   0  |  4   |  5   |  6   |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |  1   |  2   |  3   |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUM] = LAYOUT(
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                                     KC_DOT , KC_7   , KC_8   , KC_9   , _______, _______,
      _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                                     KC_0   , KC_4   , KC_5   , KC_6   , _______, _______,
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, KC_1   , KC_2   , KC_3   , _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
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
