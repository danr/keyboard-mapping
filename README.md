## keyboard mapping

Keymap loosely inspired by callum's qmk firmware
https://github.com/qmk/qmk_firmware/tree/master/users/callum

Designed for the split keyboard R-go which has a split space bar.
Its two sides turn up as two different sources so with interception tools
it is possible to make the keyboards send different keys.
The setup here makes a multi-stage shift-based modifier on the left
space bar and uses the right as a normal space.

### Running

Dependencies: interception tools and its plugin dual-function-keys.

    ./setup.sh

This compiles all dependencies and starts the required processes.

#### Shift modifier on left thumb with further modifiers on home row

The idea is to have a shift modifier on the left thumb.
When pressed it also enables further modifier keys around the left hand home row:

    home row:     shift  ctrl  alt  nav  oneshot
                   (a)   (o)   (e)  (u)   (i)

**alt**, **ctrl**:
  This releases the simulated shift key and instead makes alt or ctrl pressed.

**shift**:
  The simulated shift is released when any other modifier is activated.
  If shift needs to be pressed as too use this.

**nav**:
  This makes the right hand home row become arrow keys and the row
  below that home, pg dn, pg up and end.

**oneshot**:
  This oneshot mode makes the combinations that are otherwise impossible
  possible to create, like `ctrl a`. This is a three step process:

  1. Press down the modifiers you want (in the example: `ctrl`)
  2. Now press the oneshot trigger (in the example: `i`)
     Now the keyboard is "frozen": key up events are ignored and it is waiting for a key down event.
  3. Press the desired key (in the example: `a`).
     This will happen under the desired modifiers. Now everything else is released and
     the keyboard is unfrozen and the shift modifier mode is exited.

#### GUI on next key of left thumb

Left thumb needs to have another key as well for the window manager.
This modifier key has many names like `gui`, `super`, `windows key`, `leftmeta`.
Here I repurpose left alt. In `gui` mode these are accessible just like in shift mode:

    home row:     shift  ctrl  alt  nav  oneshot
                   (a)   (o)   (e)  (u)   (i)

#### Swedish mode

Pressing the left shift together with right shift toggles swedish mode.
It replaces the left hand's `shift compose ; q` keys with `; å ä ö`.
This is done by mapping to unused keys (on the numpad) and then
making them be åäö using xmodmap.

#### Dual function keys

The left shift when tapped sends backspace and the gui key when tapped sends delete.

#### Other remappings

Enter is next to left space (on Alt Gr).

Using xmodmap:

- Semicolon and colon are swapped
- Escape and caps lock are swapped


### rhand.c

This program moves all keys on the right hand one step to the right so that
the hands become a little bit more separated on a normal keyboard (like on a laptop).
Another advantage is that the right hand can now more easily reach two modifiers
to use as space and enter. Then the left hand can use the big space bar as the
shift modifier.
