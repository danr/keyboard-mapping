#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>

#include "revmap.h"

int main(void) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    #define NUM_WATCHES 128

    int State[NUM_WATCHES];

    memset(State, 0, sizeof(State));

    FILE* log = stderr;

    struct input_event event;

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        fwrite(&event, sizeof(event), 1, stdout);

        if (event.type == EV_KEY && event.code < NUM_WATCHES) {
            State[event.code] = event.value;
        }

        if (event.type == 1) {
            for (int i = 0; i < NUM_WATCHES; ++i) {
                if (State[i] == 0) {
                    fprintf(log, " ");
                } else {
                    fprintf(log, "%c", revmap[i][4]);
                }
            }
            fprintf(log, " %3d %-20s %d %d\n", event.code, revmap[event.code], event.value, event.type);
        }
    }
}
