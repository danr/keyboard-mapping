#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#include "revmap.h"

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    int Shift_mode = 0;
    int Transfer_mode = 0;
    int Nav_mode = 0;

    #define MS 128

    int Actual[MS];
    int Desired[MS];
    int Modal[MS];

    void clear(int* arr, int val) {
        for (int i = 0; i < MS; i++) {
            arr[i] = val;
        }
    }

    clear(Desired, -1);
    clear(Actual, 0);
    clear(Modal, 0);

    FILE* log = fopen("log", "a");

    #define KEY_NAVIGATION KEY_KATAKANA
    #define KEY_TRANSFER KEY_HIRAGANA

    void send(struct input_event event, char* reason) {
        if (event.code != KEY_NAVIGATION && event.code != KEY_TRANSFER) {
            fwrite(&event, sizeof(event), 1, stdout);
        }

        if (event.type == EV_KEY && event.code < MS) {
            Actual[event.code] = event.value;
            if (event.value > -1) {
                fprintf(log, "Shift_mode: %d ", Shift_mode);
                for (int i = 0; i < MS; ++i) {
                    if (Actual[i] == 0) {
                        fprintf(log, " ");
                    } else {
                        fprintf(log, "%c", revmap[i][4]);
                    }
                }
                fprintf(log, " %d %s %s %d\n", event.value, reason, revmap[event.code], event.code);
                fflush(log);
            }
        }
    };

    struct input_event event;

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        int passthrough = 1;

        if (event.type == EV_KEY) {
            if (event.code == KEY_LEFTSHIFT) {
                if (event.value == 1) {
                    Shift_mode = 1;
                    // clear(Desired, 0);
                }
                passthrough = 0;
            }

            void shiftmap(int key_f, int key_ctrl) {
                Modal[key_ctrl] = 1;
                if (event.code == key_f) {
                    if (Shift_mode) {
                        passthrough = 0;
                        Desired[key_f] = 0;
                        Desired[key_ctrl] = event.value;
                    } else {
                        passthrough = 1;
                        Desired[key_f] = -1;
                        Desired[key_ctrl] = -1;
                    }
                }
            }

            shiftmap(KEY_LEFTSHIFT, KEY_LEFTSHIFT); // SHIFT
            shiftmap(KEY_A, KEY_TRANSFER); // TRANSFER
            shiftmap(KEY_S, KEY_NAVIGATION); // NAV
            shiftmap(KEY_D, KEY_LEFTMETA); // GUI
            shiftmap(KEY_F, KEY_LEFTCTRL); // CTRL
            shiftmap(KEY_W, KEY_RIGHTALT); // AltGR: maybe more correct to call it RIGHTMETA
            shiftmap(KEY_E, KEY_LEFTALT);  // ALT

            if (Actual[KEY_NAVIGATION]) {
                // Nav mode
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

            if (Actual[KEY_TRANSFER]) {
                // Transfer mode
                int c = event.code;
                int r0 = c >= KEY_7 && c <= KEY_MINUS;
                int r1 = c >= KEY_U && c <= KEY_LEFTBRACE;
                int r2 = c >= KEY_J && c <= KEY_APOSTROPHE;
                int r3 = c >= KEY_M && c <= KEY_RIGHTSHIFT;
                if (r0 || r1 || r2 || r3) {
                    event.code = c - 6;
                }
            }
        }

        struct input_event original_event = event;

        for (int i = 0; i < MS; ++i) {
            if (Desired[i] == 0) {
                if (Actual[i] != Desired[i]) {
                    event.type = EV_KEY;
                    event.code = i;
                    event.value = Desired[i];
                    send(event, "desired off");
                }
                Desired[i] = -1;
            }
        }

        for (int i = 0; i < MS; ++i) {
            if (Desired[i] > 0) {
                if (Actual[i] != Desired[i]) {
                    event.type = EV_KEY;
                    event.code = i;
                    event.value = Desired[i];
                    send(event, Desired[i] == 1 ? "desired on " : "desired two");
                }
                Desired[i] = -1;
            }
        }

        if (Shift_mode) {
            int on = 0;
            for (int i = 0; i < MS; i++) {
                if (Modal[i]) {
                    if (Desired[i] > 0 || Actual[i] > 0) {
                        on++;
                    }
                }
            }
            if (!on) {
                Shift_mode = 0;
            }
        }

        if (passthrough) {
            send(original_event, "passthrough");
        }

    }
}
