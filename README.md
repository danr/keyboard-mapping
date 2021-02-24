## keyboard mapping

Keymap inspired by callum's qmk firmware
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

                         (,)  (.)
    row above:           trf  agr
    home row:     shift  nav  alt  ctrl
                   (a)   (o)  (e)  (u)

**alt**, **ctrl**:
  This releases the simulated shift key and instead makes alt or ctrl pressed.

**shift**:
  The simulated shift is released when any other modifier is activated.
  If shift needs to be pressed as too use this.

**nav**:
  This makes the right hand home row become arrow keys and the row
  below that home, pg dn, pg up and end.

**agr**:
  This is AltGR/X's Mod5. This is configured in X for example using xmodmap.

**trf**:
  This _transfer_ mode makes the combinations that are otherwise impossible
  possible to create, like `ctrl a`. This _transfer_ mode moves the normal
  left hand keys to the right hand so that its layout becomes:

    f',.py
    daoeui
    b:qjkx

So to press `ctrl a` instead press `ctrl trf h` which is `shift u , h`. As a picture:

        ctrl   a        (desired)
          |   / \
        ctrl trf h      (use trf)
          |   |  h
    shift u   ,  h

#### GUI on next key of left thumb

Left thumb needs to have another key as well for the window manager.
This modifier key has many names like `gui`, `super`, `windows key`, `leftmeta`.
Here I repurpose left alt. In `gui` mode:

    home row:     shift  nav  alt  ctrl
                   (a)   (o)  (e)  (u)

There is no `trf` here, just configure your WM so that it does not use `aoeu`.

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
