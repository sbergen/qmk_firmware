#include QMK_KEYBOARD_H
#include "deferred_exec.h"
#include "ploopyco.h"

static deferred_token drag_scroll_hold_timer;
// Indicates that we are in hold mode, and not tapping
static bool drag_scroll_held = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_BTN4, KC_BTN5, DRAG_SCROLL, KC_BTN2,
        KC_BTN1,                       KC_BTN3)
};
// clang-format on

uint32_t hold_drag_scroll(uint32_t trigger_time, void *cb_arg) {
    drag_scroll_held = true;
    set_drag_scroll(true);
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DRAG_SCROLL:
            if (record->event.pressed) {
                drag_scroll_hold_timer = defer_exec(TAPPING_TERM, hold_drag_scroll, NULL);
            } else {
                if (drag_scroll_held) {
                    drag_scroll_held = false;
                    set_drag_scroll(false);
                } else {
                    cancel_deferred_exec(drag_scroll_hold_timer);
                    toggle_drag_scroll();
                }
            }

            return false;

        default:
            return true;
    }
}
