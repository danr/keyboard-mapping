#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include "revmap.h"

int table[REVMAP_SIZE];

int parse(char *s) {
    for (int i = 0; i < REVMAP_SIZE; ++i) {
        if (0 == strcmp(s, revmap[i])) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char** argv) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    for (int i = 0; i < REVMAP_SIZE; i++) {
        table[i] = i;
    }

    for (int i = 1; i < argc; i += 2) {
        int from = parse(argv[i]);
        int to   = parse(argv[i+1]);
        if (from == -1 || to == -1) {
            fprintf(stderr, "parse(\"%s\")=%i\n", argv[i], from);
            fprintf(stderr, "parse(\"%s\")=%i\n", argv[i+1], to);
            return 1;
        }
        table[from] = to;
    }

    struct input_event event;
    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY) {
            if (event.code >= 0 && event.code < REVMAP_SIZE) {
                event.code = table[event.code];
            }
        }

        fwrite(&event, sizeof(event), 1, stdout);
        // fprintf(stderr, "%d %d %d\n", event.type, event.code, event.value);
    }
}
