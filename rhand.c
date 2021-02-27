#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    struct input_event event;
    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY) {
            switch (event.code) {
                case KEY_7:
                case KEY_8:
                case KEY_9:
                case KEY_0:
                case KEY_MINUS:
                case KEY_EQUAL:

                case KEY_U:
                case KEY_I:
                case KEY_O:
                case KEY_P:
                case KEY_LEFTBRACE:
                case KEY_RIGHTBRACE:

                case KEY_J:
                case KEY_K:
                case KEY_L:
                case KEY_SEMICOLON:
                case KEY_APOSTROPHE:

                case KEY_M:
                case KEY_COMMA:
                case KEY_DOT:
                case KEY_SLASH:
                case KEY_RIGHTSHIFT:
                    event.code--;
                    break;

                case KEY_BACKSLASH:
                    event.code = KEY_APOSTROPHE;
                    break;
            }
        }

        fwrite(&event, sizeof(event), 1, stdout);
    }
}
