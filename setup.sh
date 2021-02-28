#!/bin/bash
set -x
set -euo pipefail

for i in *.c; do
    gcc "$i" -o "$(basename "$i" .c)" -O2
done

sudo killall intercept mux uinput dual-function-keys || true
sudo killall xcape || true

# R-go
LEFT=/dev/input/by-id/usb-0911_2188-event-kbd
RIGHT=/dev/input/by-id/usb-Hantick_USB_Keyboard_HID_Composite_device-event-kbd

if test -e $LEFT && test -e $RIGHT; then
    sudo rm -f /dev/shm/Both || true
    mux -c Both

    sudo intercept -g $LEFT  | ./remap KEY_SPACE    KEY_YEN   | mux -o Both &
    sudo intercept -g $RIGHT | ./remap KEY_RIGHTALT KEY_ENTER | mux -o Both &
    mux -i Both |
        ./remap KEY_CAPSLOCK KEY_ESC      \
                KEY_ESC      KEY_CAPSLOCK |
        dual-function-keys -c <(echo '
            MAPPINGS:
              - KEY: KEY_YEN
                TAP: KEY_BACKSPACE
                HOLD: KEY_YEN
              - KEY: KEY_LEFTALT
                TAP: KEY_DELETE
                HOLD: KEY_LEFTALT
            ') | ./modifiers | sudo uinput -d $LEFT &
fi

Thinkpad=/dev/input/by-path/platform-i8042-serio-0-event-kbd

if test -e $Thinkpad; then
    sudo intercept -g $Thinkpad |
        ./rhand |
        ./remap KEY_CAPSLOCK KEY_ESC      \
                KEY_ESC      KEY_CAPSLOCK \
                KEY_SPACE    KEY_YEN      \
                KEY_RIGHTALT KEY_SPACE    \
                KEY_SYSRQ    KEY_ENTER    \
                KEY_LEFTCTRL KEY_KATAKANA \
                |
        dual-function-keys -c <(echo '
            MAPPINGS:
              - KEY: KEY_YEN
                TAP: KEY_BACKSPACE
                HOLD: KEY_LEFTCTRL
              - KEY: KEY_LEFTALT
                TAP: KEY_DELETE
                HOLD: KEY_LEFTALT
              - KEY: KEY_SLASH
                TAP: KEY_SLASH
                HOLD: KEY_RIGHTSHIFT
              - KEY: KEY_ENTER
                TAP: KEY_ENTER
                HOLD: KEY_RIGHTALT
        ') |
        ./swe_and_nav |
        dual-function-keys -c <(echo '
            MAPPINGS:
              - KEY: KEY_KP0
                TAP: KEY_Z
                HOLD: KEY_RIGHTSHIFT
        ') | sudo uinput -d $Thinkpad &
fi

sleep 0.4

setxkbmap dvorak -option "compose:102"

xmodmap <(echo '
! mode switch on AltGr
clear Mod1
clear Mod5
keycode 108 = Mode_switch
add Mod5 = Mode_switch
add Mod1 = Alt_L Alt_R

keycode 52 = colon semicolon

! Swe_mode uses these to map to åäö
keycode 87 = aring Aring
keycode 88 = adiaeresis Adiaeresis
keycode 89 = odiaeresis Odiaeresis
') || true

xset r rate 150 45
xset b off

python agr.py
