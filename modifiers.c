#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#include "revmap.h"

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    int Shift_mode = 0;
    int Gui_mode = 0;
    int Swe_mode = 0;
    int oneshot = 0;

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

    FILE* log = stderr; //fopen("log", "a");

    int Unreal[256];
    clear(Unreal, 0);

    #define KEY_NAVIGATION KEY_KATAKANA
    #define KEY_SHIFTPROXY KEY_HIRAGANA
    #define KEY_SHIFT      KEY_YEN

    revmap[KEY_NAVIGATION] = "KEY_NAVIGATION";
    revmap[KEY_SHIFTPROXY] = "KEY_SHIFTPROXY";
    revmap[KEY_SHIFT]      = "KEY_SHIFT";

    Unreal[KEY_NAVIGATION] = 1;
    Unreal[KEY_SHIFTPROXY] = 1;
    Unreal[KEY_SHIFT]      = 1;

    int unreal(int i) {
        return i >= 0 && i < 256 && Unreal[i];
    }

    void print_state(struct input_event event, char* reason) {
        fprintf(log, "Shift_mode: %d ", Shift_mode);
        fprintf(log, "Gui_mode: %d ", Gui_mode);
        fprintf(log, "Swe_mode: %d ", Swe_mode);
        fprintf(log, "oneshot: %d ", oneshot);
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

    void send(struct input_event event, char* reason) {
        if (event.type != EV_KEY || !unreal(event.code)) {
            fwrite(&event, sizeof(event), 1, stdout);
        }

        if (event.type == EV_KEY && event.code < MS) {
            Actual[event.code] = event.value;
        }

        if (event.type == EV_KEY) {
            print_state(event, reason);
        }
    };

    struct input_event event;

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        int passthrough = 1;

        if (event.type == EV_KEY && oneshot) {
            passthrough = 0;
            if (event.value == 1) {
                send(event, "oneshot on ");
                event.value = 0;
                send(event, "oneshot off");
                Shift_mode = 0;
                Gui_mode = 0;
                oneshot = 0;
                clear(Desired, 0);
            } else {
                print_state(event, "oneshot... ");
            }
        } else if (event.type == EV_KEY) {
            if (event.code == KEY_YEN && !Gui_mode) {
                if (event.value == 1) {
                    Shift_mode = 1;
                }
                passthrough = 0;
            }
            if (event.code == KEY_LEFTALT) {
                Gui_mode = event.value > 0;
                Shift_mode = 0;

                passthrough = 0;
                if (!Gui_mode) {
                    clear(Desired, 0);
                }
            }

            void modal_map(int in_mode, int key_f, int key_ctrl) {
                if (in_mode) {
                    if (Shift_mode) {
                        Modal[key_ctrl] = 1;
                    }
                    if (event.code == key_f) {
                        passthrough = 0;
                        Desired[key_f] = 0;
                        Desired[key_ctrl] = event.value;
                    }
                }
            }

            modal_map(Shift_mode, KEY_SHIFT, KEY_SHIFTPROXY);
            modal_map(Shift_mode, KEY_A, KEY_RIGHTSHIFT);
            modal_map(Shift_mode, KEY_S, KEY_LEFTCTRL);
            modal_map(Shift_mode, KEY_D, KEY_LEFTALT);
            modal_map(Shift_mode, KEY_F, KEY_NAVIGATION);
            // modal_map(Shift_mode, KEY_E, KEY_RIGHTALT); // AltGr unused for now

            modal_map(Gui_mode, KEY_LEFTALT, KEY_LEFTMETA);
            modal_map(Gui_mode, KEY_A, KEY_RIGHTSHIFT);
            modal_map(Gui_mode, KEY_S, KEY_LEFTCTRL);
            modal_map(Gui_mode, KEY_D, KEY_LEFTALT);
            modal_map(Gui_mode, KEY_F, KEY_NAVIGATION);

            if (Shift_mode | Gui_mode && event.code == KEY_G && event.value == 1) {
                oneshot = 1;
                passthrough = 0;
            }

            if (Shift_mode && event.code == KEY_RIGHTSHIFT) {
                if (event.value == 1) {
                    Swe_mode = 1 - Swe_mode;
                }
            }


            if (Swe_mode) {
                switch (event.code) {
                    case KEY_LEFTSHIFT: event.code = KEY_Z; break;
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

            if (Actual[KEY_NAVIGATION]) {
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

        struct input_event original_event = event;

        int On(int i) {
            return Desired[i] > 0 || Actual[i] > 0;
        };

        if (Shift_mode) {
            int num_on = 0;
            for (int i = 0; i < MS; i++) {
                if (Modal[i]) {
                    num_on += On(i);
                }
            }
            Desired[KEY_LEFTSHIFT] = num_on == 1 && On(KEY_SHIFTPROXY);
            if (num_on == 0) {
                Shift_mode = 0;
            }
        }

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

        if (passthrough) {
            send(original_event, "passthrough");
        }

    }
}
