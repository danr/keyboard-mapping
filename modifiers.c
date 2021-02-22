#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#include "revmap.h"

#define SHIFT 0
#define GUI 1
#define ALT 2
#define CONTROL 3
#define META 4
#define DOUBLESHIFT 5
#define COUNT 6

int tr[COUNT] = {
    KEY_RIGHTSHIFT,
    KEY_LEFTMETA,
    KEY_LEFTALT,
    KEY_LEFTCTRL,
    KEY_RIGHTALT,
    KEY_RIGHTSHIFT,
};

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    int L_mode = 0;
    int R_mode = 0;
    int desired[COUNT];
    int actual[COUNT];
    int Nav_mode = 0;

    FILE* log = fopen("log", "a");

    struct input_event event;
    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        int passthrough = 1;

        if (event.type == EV_KEY) {
            switch (event.code) {
                case KEY_LEFTALT:
                    Nav_mode = (event.value) > 0;
                case KEY_LEFTSHIFT:
                    if (!R_mode) {
                        L_mode = (event.value > 0);
                        for (int i = 0; i < COUNT; i++) {
                            desired[i] = 0;
                        }
                        passthrough = 0;
                    }
                    break;

                case KEY_RIGHTSHIFT:
                    if (!L_mode) {
                        R_mode = (event.value > 0);
                        for (int i = 0; i < COUNT; i++) {
                            desired[i] = 0;
                        }
                        passthrough = 0;
                    } else {
                        event.code = KEY_ENTER;
                    }
                    break;

                case KEY_A:         if (L_mode) { passthrough = 0; desired[GUI]         = event.value > 0; } break;
                case KEY_S:         if (L_mode) { passthrough = 0; desired[ALT]         = event.value > 0; } break;
                case KEY_D:         if (L_mode) { passthrough = 0; desired[DOUBLESHIFT] = event.value > 0; } break;
                case KEY_F:         if (L_mode) { passthrough = 0; desired[CONTROL]     = event.value > 0; } break;
                case KEY_W:         if (L_mode) { passthrough = 0; desired[META]        = event.value > 0; } break;
                case KEY_E:         if (L_mode) { passthrough = 0; desired[DOUBLESHIFT] = event.value > 0; } break;

                case KEY_SEMICOLON: if (R_mode) { passthrough = 0; desired[GUI]         = event.value > 0; } break;
                case KEY_L:         if (R_mode) { passthrough = 0; desired[ALT]         = event.value > 0; } break;
                case KEY_K:         if (R_mode) { passthrough = 0; desired[DOUBLESHIFT] = event.value > 0; } break;
                case KEY_J:         if (R_mode) { passthrough = 0; desired[CONTROL]     = event.value > 0; } break;
                case KEY_O:         if (R_mode) { passthrough = 0; desired[META]        = event.value > 0; } break;
                case KEY_I:         if (R_mode) { passthrough = 0; desired[DOUBLESHIFT] = event.value > 0; } break;
            }
            if (Nav_mode) {
                switch (event.code) {
                    case KEY_J: event.code = KEY_LEFT; break;
                    case KEY_K: event.code = KEY_DOWN; break;
                    case KEY_L: event.code = KEY_UP; break;
                    case KEY_SEMICOLON: event.code = KEY_RIGHT; break;

                    case KEY_COMMA: event.code = KEY_PAGEDOWN; break;
                    case KEY_DOT: event.code = KEY_PAGEUP; break;
                    case KEY_I: event.code = KEY_HOME; break;
                    case KEY_O: event.code = KEY_END; break;
                }
            }
            if (event.code >= 0 && event.code < 195 && event.value != 2) {
                fprintf(log, "%d %s %d\n", passthrough, revmap[event.code], event.value);
                fflush(log);
            }
        }

        if (passthrough) {
            fwrite(&event, sizeof(event), 1, stdout);
        }

        if (!passthrough) {
            int shift = 1;
            for (int i = 0; i < COUNT; ++i) {
                if (i != SHIFT) {
                    if (desired[i]) {
                        shift = 0;
                    }
                }
            }
            desired[SHIFT] = (L_mode || R_mode) && ((!Nav_mode && shift) || desired[DOUBLESHIFT]);
            for (int i = 0; i < COUNT; ++i) {
                if (i != DOUBLESHIFT) {
                    if (desired[i] != actual[i]) {
                        event.type = EV_KEY;
                        event.code = tr[i];
                        event.value = desired[i];
                        actual[i] = desired[i];
                        fprintf(log, "! %s %d\n", revmap[event.code], event.value);
                        fflush(log);
                        fwrite(&event, sizeof(event), 1, stdout);
                    }
                }
            }

            fprintf(log, "%d %d %d %d %d %d %d %d %d \n", L_mode, R_mode, Nav_mode, actual[SHIFT], actual[GUI], actual[ALT], actual[CONTROL], actual[META], actual[DOUBLESHIFT]);
            fflush(log);
        }
    }
}
