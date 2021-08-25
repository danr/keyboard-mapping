#!/bin/bash
set -x
set -euo pipefail

for i in *.c; do
    gcc "$i" -o "$(basename "$i" .c)" -O2
done

sudo killall intercept mux uinput dual-function-keys || true
sudo killall xcape || true

Thinkpad=/dev/input/by-path/platform-i8042-serio-0-event-kbd
if test -e $Thinkpad; then
    sudo intercept -g $Thinkpad |
        ./rhand |
        ./remap KEY_CAPSLOCK  KEY_ESC       \
                KEY_ESC       KEY_CAPSLOCK  \
                KEY_SPACE     KEY_BACKSPACE \
                KEY_RIGHTALT  KEY_SPACE     \
                KEY_SYSRQ     KEY_ENTER     \
                |
        dual-function-keys -c <(echo '
            MAPPINGS:
              - KEY: KEY_BACKSPACE
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
        ') | # ./homerow_nav |
        sudo uinput -d $Thinkpad &
fi

Kadv1=/dev/input/by-id/usb-05f3_0007-event-kbd
Kadv2=/dev/input/by-id/usb-Kinesis_Advantage2_Keyboard_314159265359-if01-event-kbd
if test -e $Kadv1; then
    Kadv=$Kadv1
else
    Kadv=$Kadv2
fi

if test -e $Kadv; then
    sudo intercept -g $Kadv | # ./keylogger |
        ./remap KEY_CAPSLOCK   KEY_ESC        \
                KEY_ESC        KEY_CAPSLOCK   \
                KEY_DELETE     KEY_LEFTALT    \
                KEY_BACKSPACE  KEY_LEFTCTRL   \
                KEY_LEFTALT    KEY_BACKSPACE  \
                KEY_LEFTCTRL   KEY_LEFTMETA   \
                KEY_RIGHTCTRL  KEY_RIGHTALT   \
                KEY_LEFTBRACE  KEY_RIGHTBRACE \
                KEY_RIGHTBRACE KEY_LEFTBRACE  \
                KEY_RIGHTMETA  KEY_RIGHTALT   \
                |
        dual-function-keys -c <(echo '
            MAPPINGS:
              - KEY: KEY_LEFTCTRL
                TAP: KEY_BACKSPACE
                HOLD: KEY_LEFTCTRL
              - KEY: KEY_LEFTALT
                TAP: KEY_DELETE
                HOLD: KEY_LEFTALT
        ') | # ./keylogger |
        sudo uinput -d $Kadv &
fi


sleep 0.4

setxkbmap dvorak -option "compose:102"

xmodmap <(echo '
! mode switch on AltGr (KEY_RIGHTALT)
clear Mod1
clear Mod5
keycode 108 = Mode_switch
add Mod1 = Alt_L Alt_R
add Mod5 = Mode_switch
') || true

xset r rate 150 45
xset b off

python agr.py
