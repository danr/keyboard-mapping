#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#include "revmap.h"

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    #define STATES 128

    int State[STATES];

    void clear(int* arr, int val) {
        for (int i = 0; i < STATES; i++) {
            arr[i] = val;
        }
    }

    clear(State, 0);

    FILE* log = stderr;

    void print_state(struct input_event event, char* reason) {
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
            if (State[KEY_KATAKANA]) {
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
