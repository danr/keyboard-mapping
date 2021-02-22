#!/bin/bash
set -x
set -euo pipefail

sudo rm -f /dev/shm/{Both,Left,Right} || true
sudo killall intercept mux uinput dual-function-keys || true
# sudo killall xcape || true
sudo rm -f log || true

mux -c Left -c Right -c Both

LEFT=/dev/input/by-id/usb-0911_2188-event-kbd
RIGHT=/dev/input/by-id/usb-Hantick_USB_Keyboard_HID_Composite_device-event-kbd

> dual-left.yml echo '
MAPPINGS:
  - KEY: KEY_SPACE
    TAP: KEY_BACKSPACE
    HOLD: KEY_LEFTSHIFT
  - KEY: KEY_LEFTALT
    TAP: KEY_DELETE
    HOLD: KEY_LEFTALT
'

> dual-right.yml echo '
'

sudo intercept -g $LEFT | mux -o Left &
sudo intercept -g $RIGHT | mux -o Right &
mux -i Left  | ./remap KEY_SPACE KEY_LEFTSHIFT | mux -o Both &
mux -i Right | ./remap KEY_RIGHTALT KEY_ENTER | mux -o Both &
mux -i Both | ./modifiers | sudo uinput -d $LEFT &

sleep 0.4

setxkbmap dvorak -option "compose:102"

xmodmap <(echo '
! Swap Caps_Lock and Escape
remove Lock = Caps_Lock
keysym Escape = Caps_Lock
keysym Caps_Lock = Escape
add Lock = Caps_Lock

! mode switch on AltGr
clear Mod1
clear Mod5
keycode 108 = Mode_switch
add Mod5 = Mode_switch
add Mod1 = Alt_L Alt_R

! space with shift is enter
keycode 65 = space Return

! a A å Å
keycode 38 = U61 U41 UE5 UC5
! o O ä Ä
keycode 39 = U6F U4F UE4 UC4
! e E ö aÖ
keycode 40 = U65 U45 UF6 UD6

! : ; : ;
keycode 52 = U3A U3B U3A U3B
') || true

xset r rate 150 45
xset b off

tail -f log
