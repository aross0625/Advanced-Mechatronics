#include "font.h"
#include "ssd1306.h"
#include <string.h>

void drawChar(unsigned char x_i, unsigned char y_i, char c) {
    int c1;
    int x, y;
    int i = c - 32;
    for (c1 = 0; c1 < 5; c1++) {
        x = x_i + c1;
        char key = ASCII[i][c1];
        int bit = 0;
        while (bit < 8) {
            y = y_i + bit;
            if (key & 0x01) {
                ssd1306_drawPixel(x, y, 1); // bit is 1, turn on
            } else {
                ;
            }
            key = key >> 1;
            bit++;
        }

    }
    ssd1306_update();
}

void drawStr(unsigned char x, unsigned char y, char * string) {
    int len = strlen(string);
    int i;
    for (i = 0; i < len; i++) {
        drawChar(x, y, string[i]);
        x = x + 6;
    }
    ssd1306_update();
}