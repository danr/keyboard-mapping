#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    struct input_event event;
    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY) {
            switch (event.code) {
                #define PREV event.code--; break;
                case KEY_7:           PREV;
                case KEY_8:           PREV;
                case KEY_9:           PREV;
                case KEY_0:           PREV;
                case KEY_MINUS:       PREV;
                case KEY_EQUAL:       PREV;

                case KEY_U:           PREV;
                case KEY_I:           PREV;
                case KEY_O:           PREV;
                case KEY_P:           PREV;
                case KEY_LEFTBRACE:   PREV;
                case KEY_RIGHTBRACE:  PREV;

                case KEY_J:           PREV;
                case KEY_K:           PREV;
                case KEY_L:           PREV;
                case KEY_SEMICOLON:   PREV;
                case KEY_APOSTROPHE:  PREV;
                case KEY_BACKSLASH:   event.code = KEY_APOSTROPHE; break;

                case KEY_M:           PREV;
                case KEY_COMMA:       PREV;
                case KEY_DOT:         PREV;
                case KEY_SLASH:       PREV;
                case KEY_RIGHTSHIFT:  PREV;
                #undef PREV

                case KEY_LEFTSHIFT:   event.code = KEY_RIGHTSHIFT; break;
            }
        }

        fwrite(&event, sizeof(event), 1, stdout);
    }
}
