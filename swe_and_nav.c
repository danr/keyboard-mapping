#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#include "revmap.h"

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    int Swe_mode = 0;
    int Nav_mode = 0;

    #define STATES 128

    int State[STATES];

    void clear(int* arr, int val) {
        for (int i = 0; i < STATES; i++) {
            arr[i] = val;
        }
    }

    clear(State, 0);

    FILE* log = stderr; //fopen("log", "a");

    void print_state(struct input_event event, char* reason) {
        fprintf(log, "Swe_mode: %d ", Swe_mode);
        fprintf(log, "Nav_mode: %d ", Nav_mode);
        for (int i = 0; i < STATES; ++i) {
            if (State[i] == 0) {
                fprintf(log, " ");
            } else {
                fprintf(log, "%c", revmap[i][4]);
            }
        }
        fprintf(log, " %d %s %s %d\n", event.value, reason, revmap[event.code], event.code);
        fflush(log);
    }

    void send(struct input_event event, char* reason) {
        fwrite(&event, sizeof(event), 1, stdout);

        if (event.type == EV_KEY && event.code < STATES) {
            State[event.code] = event.value != 0;
        }

        if (event.type == EV_KEY) {
            print_state(event, reason);
        }
    };

    struct input_event event;

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        int passthrough = 1;

        if (event.type == EV_KEY) {
            if (State[KEY_LEFTCTRL] && event.code == KEY_ESC) {
                passthrough = 0;
                if (event.value == 1) {
                    Swe_mode = 1 - Swe_mode;
                    print_state(event, "Swe_mode   ");
                }
            }

            if (event.code == KEY_KATAKANA) {
                passthrough = 0;
                if (State[KEY_LEFTCTRL]) {
                    if (event.value == 1) {
                        Nav_mode = 1 - Nav_mode;
                        print_state(event, "Nav tog    ");
                    }
                } else {
                    Nav_mode = event.value != 0;
                    print_state(event, "Nav mod    ");
                }
            }

            if (Swe_mode) {
                switch (event.code) {
                    case KEY_LEFTSHIFT: event.code = KEY_KP0; break;
                    case KEY_102ND: event.code = KEY_KP1; break;
                    case KEY_Z:     event.code = KEY_KP2; break;
                    case KEY_X:     event.code = KEY_KP3; break;
                    /*
                       // X will interpret these as 87 88 89, so execute these:
                       keycode 87 = aring Aring
                       keycode 88 = adiaeresis Adiaeresis
                       keycode 89 = odiaeresis Odiaeresis
                    */
                }
            }

            if (Nav_mode) {
                switch (event.code) {
                    case KEY_J: event.code = KEY_LEFT; break;
                    case KEY_K: event.code = KEY_DOWN; break;
                    case KEY_L: event.code = KEY_UP; break;
                    case KEY_SEMICOLON: event.code = KEY_RIGHT; break;

                    case KEY_M: event.code = KEY_HOME; break;
                    case KEY_COMMA: event.code = KEY_PAGEDOWN; break;
                    case KEY_DOT: event.code = KEY_PAGEUP; break;
                    case KEY_SLASH: event.code = KEY_END; break;
                }
            }
        }

        if (passthrough) {
            send(event, "passthrough");
        }
    }
}
