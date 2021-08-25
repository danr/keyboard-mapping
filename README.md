## keyboard mapping

### Running

Dependencies: interception tools and its plugin dual-function-keys.

    ./setup.sh

This compiles all dependencies and starts the required processes.

### rhand.c

This program moves all keys on the right hand one step to the right so that
the hands become a little bit more separated on a normal keyboard (like on a laptop).
Another advantage is that the right hand can now more easily reach two modifiers
to use as space and enter. Then the left hand can use the big space bar as the
shift modifier.

Now we don't have a button for backspace, backslash, equals and z.
These could all be put in the middlemost row (that is now free) consisting
of `6`, dvorak `f`, `d` and `b`. But there are more convenient ways to solve this:

- backspace: already on space, but we put a copy on `f`,
- enter: already on prtsc/menu, but we put a copy on `d`,
- backslash: put on `b`,
- z: dual-function-keys on right shift: z when tapped, right shift when held.

### remap.c

Remaps pairwise using the names in `linux/input.h`, so this makes
pressing A send a B press event, and pressing C send a D press event:

    ./remap KEY_A KEY_B KEY_C KEY_D

### homerow_nav.c

Makes the homerow become the arrow keys when KEY_KATAKANA is pressed.
You can get such a katakana key using remap.

### python agr.py

Adds modifiers to the alt gr mode switch. Some are obtained from
my kak conf and written to kakmap.py.

### other changes activated in ./setup.sh

- Enter is next to left space (on Alt Gr).
- On laptop: Space is on AltGr and Enter is on the key next to it
  (On thinkpad this is PrtSc).
- Escape and caps lock are swapped
- Semicolon and colon are swapped (using xmodmap)


### Kinesis advantage 2

Turn off noises with
- Program + F8
- Program + Shift + F8

Default mapping is:

    ]                                         [
                                              \

     ` compose Left Right        Up Down = /

Remap it to

    `                                         =
                                              /

     compose \ [ ]        Left Down Up Right
