#include QMK_KEYBOARD_H

#include "oneshot.h"
// #include "keymap_swiss_de.h"
// #include "keymap_german.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TABL G(S(KC_LBRC))
#define TABR G(S(KC_RBRC))
#define SPCL A(G(KC_LEFT))
#define SPC_R A(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

// Swiss things
#define CC_AE RALT(KC_Q)
#define CC_OE RALT(KC_P)
#define CC_UE RALT(KC_Y)

// Window manager
#define CC_WM_1 KC_NO
#define CC_WM_2 KC_NO
#define CC_WM_3 KC_NO
#define CC_CTRL_G LCTL(KC_G)


enum layers {
    DEF,
    SYM,
    NAV,
    NUM,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN,  // Switch to next window         (cmd-tab)
    SW_LANG, // Switch to next input language (ctl-spc)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // kh_k twice!!
    /* [DEF] = LAYOUT( */
    /*     KC_W,    KC_F,    KC_M,    KC_P,    KC_V,    KC_SLSH, KC_DOT,  KC_Q,    KC_Z,    KC_J, */
    /*     KC_R,    KC_S,    KC_N,    KC_T,    KC_B,    KC_COMM, KC_A,    KC_E,    KC_I,    KC_H, */
    /*     KC_X,    KC_C,    KC_L,    KC_D,    KC_G,    KC_K,    KC_U,    KC_O,    KC_Y,    KC_K, */
    /*                                LA_NAV,  KC_LSFT, KC_SPC,  LA_SYM */
    /* ), */
    // colemak-dh
    [DEF] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH,
                                   LA_NAV,  KC_LSFT, KC_SPC,  LA_SYM
    ),
    [SYM] = LAYOUT(
        KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_BSLS, KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,
        KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,  KC_HASH, OS_CMD,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_PLUS, KC_PIPE, KC_AT,   KC_TILD, KC_PERC, XXXXXXX, KC_AMPR, KC_QUOT, KC_DQUO, KC_EXLM,
                                   _______, KC_BSPC, XXXXXXX, _______
    ),

    [NAV] = LAYOUT(
        XXXXXXX, CC_WM_1, CC_WM_2, CC_WM_3, KC_VOLU, CC_AE,  HOME,    KC_UP,   END,     KC_DEL,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  KC_VOLD, CC_OE,  KC_LEFT, KC_DOWN, KC_RGHT, CC_CTRL_G,
        XXXXXXX, XXXXXXX, BACK,    FWD,     XXXXXXX, CC_UE,  KC_PGDN, KC_PGUP, KC_TAB,  KC_INS,
                                   _______, XXXXXXX, KC_ENT, _______
    ),

    [NUM] = LAYOUT(
        KC_7,    KC_5,    KC_3,    KC_1,    KC_9,    KC_8,    KC_0,    KC_2,    KC_4,    KC_6,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  KC_F11,  KC_F10,  OS_CMD,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_F7,   KC_F5,   KC_F3,   KC_F1,   KC_F9,   KC_F8,   KC_F12,  KC_F2,   KC_F4,   KC_F6,
                                   _______, XXXXXXX, XXXXXXX, _______
    ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}

