#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    struct input_event event;
    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY) {
            switch (event.code) {
                // case KEY_RIGHTALT:   event.code = KEY_RIGHTSHIFT; break;
                case KEY_RIGHTALT:   event.code = KEY_ENTER; break;
            }
        }

        fwrite(&event, sizeof(event), 1, stdout);
    }
}
