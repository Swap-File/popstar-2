
#include <Arduino.h>

#include "FastLED.h"
#include "main.h"

#define CHIN_LEN 60

CRGB Snapshot_Array[MATRIX_WIDTH][MATRIX_HEIGHT];  // snapshot for fades
CRGB chin_snapshot[CHIN_WIDTH][CHIN_HEIGHT];

static void decay_snapshots(void) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            Snapshot_Array[x][y].nscale8(240);
        }
    }
    for (uint8_t y = 0; y < CHIN_HEIGHT; y++) {
        for (uint8_t x = 0; x < CHIN_WIDTH; x++) {
            chin_snapshot[x][y].nscale8(240);
        }
    }
}

static uint16_t XY(uint16_t x, uint16_t y) {
    uint16_t tempindex = 0;
    //determine if row is even or odd abd place pixel

    if (x < 8) {
        tempindex = 5 * STRIP_LEN;
        if ((y & 0x01) == 0)
            tempindex += x + (y * 8);
        else
            tempindex += (7 - x) + (y * 8);

    } else if (x < 16) {
        tempindex = 6 * STRIP_LEN;
        if ((y & 0x01) == 0)
            tempindex += ((x - 8)) + ((15 - y) * 8);
        else
            tempindex += (7 - (x - 8)) + ((15 - y) * 8);

    } else {
        tempindex = 7 * STRIP_LEN;
        if ((y & 0x01) == 0)
            tempindex += (x - 16) + (y * 8);
        else
            tempindex += (7 - (x - 16)) + (y * 8);
    }

    return tempindex;
}

static uint16_t XY_chin(uint16_t x, uint16_t y) {
    uint16_t tempindex = 4 * STRIP_LEN;

    if (y < 4) {
        if ((y & 0x01) == 0)
            tempindex += x + (y * 7);
        else
            tempindex += (6 - x) + (y * 7);
        return tempindex;
    } else if (y < 8 && x > 0 && x < 6) {
        if ((y & 0x01) == 0)
            tempindex += (x - 1) + ((y - 4) * 5) + (7 * 4);
        else
            tempindex += 4 - (x - 1) + ((y - 4) * 5) + (7 * 4);
        return tempindex;

    } else if (x > 1 && x < 5) {
        if ((y & 0x01) == 0)
            tempindex += (x - 2) + ((y - 8) * 3) + (7 * 4) + (20);
        else
            tempindex += 2 - (x - 2) + ((y - 8) * 3) + (7 * 4) + (20);
        return tempindex;
    }
    return 0;
}

static void do_chin(CRGB Output_Array[], const struct popstar_struct *helmet) {
    for (uint8_t x = 0; x < CHIN_WIDTH; x++) {
        for (uint8_t y = 0; y < CHIN_HEIGHT; y++) {
            int location = XY_chin(x, y);
            if (location > 0)
                Output_Array[location] = helmet->chin_array[x][y];
        }
    }
}

void do_line_h(CRGB Output_Array[], const CRGB outer_horiz[]) {
    //start of left
    for (int i = 0; i < OUTLINE_HORIZONTAL; i++) {
        if (i == 0 || i == 1) {
            for (int j = 54; j < 73; j++) {
                Output_Array[3 * STRIP_LEN + j] += outer_horiz[i];  // left side
            }
        }

        if (i == 2 || i == 3) {
            Output_Array[3 * STRIP_LEN + 73] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 39] += outer_horiz[i];  // -
        }

        if (i == 2)
            Output_Array[3 * STRIP_LEN + 53] += outer_horiz[i];  // ^

        if (i == 3)
            Output_Array[3 * STRIP_LEN + 52] += outer_horiz[i];  // ^

        if (i == 4 || i == 5) {
            Output_Array[3 * STRIP_LEN + 74] += outer_horiz[i];  // top line

            Output_Array[3 * STRIP_LEN + 38] += outer_horiz[i];  // -
        }
        if (i == 4)
            Output_Array[3 * STRIP_LEN + 51] += outer_horiz[i];  // ^

        if (i == 5)
            Output_Array[3 * STRIP_LEN + 50] += outer_horiz[i];  // ^

        if (i == 6 || i == 7) {
            Output_Array[3 * STRIP_LEN + 75] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 38] += outer_horiz[i];  // -
            Output_Array[3 * STRIP_LEN + 49] += outer_horiz[i];  // ^
        }

        if (i == 8 || i == 9) {
            Output_Array[3 * STRIP_LEN + 76] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 48] += outer_horiz[i];  // ^
            Output_Array[3 * STRIP_LEN + 37] += outer_horiz[i];  // -
        }
        if (i == 10 || i == 11) {
            Output_Array[3 * STRIP_LEN + 77] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 47] += outer_horiz[i];  // ^
            Output_Array[3 * STRIP_LEN + 36] += outer_horiz[i];  // -
            Output_Array[3 * STRIP_LEN + 15] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 16] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 17] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 18] += outer_horiz[i];  // C
        }

        if (i == 12 || i == 13) {
            Output_Array[3 * STRIP_LEN + 78] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 46] += outer_horiz[i];  // ^
            Output_Array[3 * STRIP_LEN + 35] += outer_horiz[i];  // -
            Output_Array[3 * STRIP_LEN + 14] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 19] += outer_horiz[i];  // C
        }
        if (i == 14 || i == 15) {
            Output_Array[3 * STRIP_LEN + 79] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 34] += outer_horiz[i];  // -
            Output_Array[3 * STRIP_LEN + 13] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 20] += outer_horiz[i];  // C
        }
        if (i == 14)
            Output_Array[3 * STRIP_LEN + 45] += outer_horiz[i];  // ^

        if (i == 15)
            Output_Array[3 * STRIP_LEN + 44] += outer_horiz[i];  // ^

        if (i == 16 || i == 17) {
            Output_Array[3 * STRIP_LEN + 80] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 34] += outer_horiz[i];  // -

            Output_Array[3 * STRIP_LEN + 12] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 21] += outer_horiz[i];  // C
        }

        if (i == 16)
            Output_Array[3 * STRIP_LEN + 43] += outer_horiz[i];  // ^

        if (i == 17)
            Output_Array[3 * STRIP_LEN + 42] += outer_horiz[i];  // ^

        if (i == 18 || i == 19) {
            Output_Array[3 * STRIP_LEN + 81] += outer_horiz[i];  // top line
            Output_Array[3 * STRIP_LEN + 33] += outer_horiz[i];  // -

            Output_Array[3 * STRIP_LEN + 11] += outer_horiz[i];  // C
            Output_Array[3 * STRIP_LEN + 22] += outer_horiz[i];  // C
        }

        if (i == 18)
            Output_Array[3 * STRIP_LEN + 41] += outer_horiz[i];  // ^

        if (i == 19)
            Output_Array[3 * STRIP_LEN + 40] += outer_horiz[i];  // ^

        if (i == 20 || i == 21) {
            for (int j = 0; j < 11; j++) {
                Output_Array[3 * STRIP_LEN + j] += outer_horiz[i];  // top bar
            }

            for (int j = 23; j < 33; j++) {
                Output_Array[3 * STRIP_LEN + j] += outer_horiz[i];  // bottom bar
            }
        }

        //end of left

        if (i == 22 || i == 23) {
            Output_Array[6 * STRIP_LEN + 128 + 16] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 0] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 24] += outer_horiz[i];   // chin top
            Output_Array[4 * STRIP_LEN + 60 + 7] += outer_horiz[i];    // chin bottom
        }
        if (i == 22)
            Output_Array[4 * STRIP_LEN + 60 + 6] += outer_horiz[i];  // chin ^
        if (i == 23)
            Output_Array[4 * STRIP_LEN + 60 + 5] += outer_horiz[i];  // chin ^

        if (i == 24 || i == 25) {
            Output_Array[6 * STRIP_LEN + 128 + 15] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 1] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 25] += outer_horiz[i];   // chin top
        }

        if (i == 24) {
            Output_Array[4 * STRIP_LEN + 60 + 7] += outer_horiz[i];  // chin bottom
            Output_Array[4 * STRIP_LEN + 60 + 4] += outer_horiz[i];  // chin ^
        }

        if (i == 25) {
            Output_Array[4 * STRIP_LEN + 60 + 8] += outer_horiz[i];  // chin bottom
            Output_Array[4 * STRIP_LEN + 60 + 3] += outer_horiz[i];  // chin ^
        }

        if (i == 26 || i == 27) {
            Output_Array[6 * STRIP_LEN + 128 + 14] += outer_horiz[i];  //forehead top
            Output_Array[6 * STRIP_LEN + 128 + 2] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 26] += outer_horiz[i];   // chin top
            Output_Array[4 * STRIP_LEN + 60 + 8] += outer_horiz[i];    // chin bottom
        }
        if (i == 26) {
            Output_Array[4 * STRIP_LEN + 60 + 26] += outer_horiz[i];  // chin top
            Output_Array[4 * STRIP_LEN + 60 + 2] += outer_horiz[i];   // chin ^
        }

        if (i == 27) {
            Output_Array[4 * STRIP_LEN + 60 + 27] += outer_horiz[i];  // chin top
            Output_Array[4 * STRIP_LEN + 60 + 1] += outer_horiz[i];   // chin ^
        }

        if (i == 28 || i == 29) {
            Output_Array[6 * STRIP_LEN + 128 + 13] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 3] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 9] += outer_horiz[i];    // chin bottom
            Output_Array[4 * STRIP_LEN + 60 + 27] += outer_horiz[i];   // chin top
        }

        if (i == 28)
            Output_Array[4 * STRIP_LEN + 60 + 0] += outer_horiz[i];  // chin ^

        if (i == 29)
            Output_Array[4 * STRIP_LEN + 60 + 23] += outer_horiz[i];  // chin ^

        if (i == 30 || i == 31) {
            Output_Array[6 * STRIP_LEN + 128 + 12] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 4] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 28] += outer_horiz[i];   // chin top
            Output_Array[4 * STRIP_LEN + 60 + 11] += outer_horiz[i];   // chin bottom
        }
        if (i == 30) {
            Output_Array[4 * STRIP_LEN + 60 + 9] += outer_horiz[i];   // chin bottom
            Output_Array[6 * STRIP_LEN + 128 + 3] += outer_horiz[i];  // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 22] += outer_horiz[i];  // chin ^
        }

        if (i == 31) {
            Output_Array[4 * STRIP_LEN + 60 + 10] += outer_horiz[i];  // chin bottom
            Output_Array[6 * STRIP_LEN + 128 + 4] += outer_horiz[i];  // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 21] += outer_horiz[i];  // chin ^
        }

        if (i == 32 || i == 33) {
            Output_Array[6 * STRIP_LEN + 128 + 11] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 4] += outer_horiz[i];   // forehead bottom

            Output_Array[4 * STRIP_LEN + 60 + 10] += outer_horiz[i];  // chin bottom
        }

        if (i == 32) {
            Output_Array[4 * STRIP_LEN + 60 + 28] += outer_horiz[i];  // chin top
            Output_Array[4 * STRIP_LEN + 60 + 20] += outer_horiz[i];  // chin ^
        }

        if (i == 33) {
            Output_Array[4 * STRIP_LEN + 60 + 29] += outer_horiz[i];  // chin top
            Output_Array[4 * STRIP_LEN + 60 + 19] += outer_horiz[i];  // chin ^
        }

        if (i == 34 || i == 35) {
            Output_Array[6 * STRIP_LEN + 128 + 10] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 5] += outer_horiz[i];   // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 11] += outer_horiz[i];   // chin bottom
        }

        if (i == 34) {
            Output_Array[4 * STRIP_LEN + 60 + 18] += outer_horiz[i];  // chin ^
            Output_Array[4 * STRIP_LEN + 60 + 11] += outer_horiz[i];  // chin bottom
        }

        if (i == 35) {
            Output_Array[4 * STRIP_LEN + 60 + 17] += outer_horiz[i];  // chin ^
            Output_Array[4 * STRIP_LEN + 60 + 12] += outer_horiz[i];  // chin bottom
        }

        if (i == 36 || i == 37) {
            Output_Array[6 * STRIP_LEN + 128 + 9] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 6] += outer_horiz[i];  // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 29] += outer_horiz[i];  // chin top
            Output_Array[4 * STRIP_LEN + 60 + 12] += outer_horiz[i];  // chin bottom
        }

        if (i == 36)
            Output_Array[4 * STRIP_LEN + 60 + 16] += outer_horiz[i];  // chin ^

        if (i == 37)
            Output_Array[4 * STRIP_LEN + 60 + 15] += outer_horiz[i];  // chin ^

        if (i == 38 || i == 39) {
            Output_Array[6 * STRIP_LEN + 128 + 8] += outer_horiz[i];  // forehead top
            Output_Array[6 * STRIP_LEN + 128 + 7] += outer_horiz[i];  // forehead bottom
            Output_Array[4 * STRIP_LEN + 60 + 30] += outer_horiz[i];  // chin top
        }
        if (i == 38)
            Output_Array[4 * STRIP_LEN + 60 + 14] += outer_horiz[i];  // chin ^

        if (i == 39)
            Output_Array[4 * STRIP_LEN + 60 + 13] += outer_horiz[i];  // chin ^

        //right

        if (i == OUTLINE_HORIZONTAL - 0 || i == OUTLINE_HORIZONTAL - 1) {
            for (int j = 54; j < 73; j++) {
                Output_Array[2 * STRIP_LEN + j] += outer_horiz[i];  // left side
            }
        }

        if (i == OUTLINE_HORIZONTAL - 2 || i == OUTLINE_HORIZONTAL - 3) {
            Output_Array[2 * STRIP_LEN + 73] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 39] += outer_horiz[i];  // -
        }

        if (i == OUTLINE_HORIZONTAL - 2) {
            Output_Array[2 * STRIP_LEN + 53] += outer_horiz[i];  // ^
        }

        if (i == OUTLINE_HORIZONTAL - 3) {
            Output_Array[2 * STRIP_LEN + 52] += outer_horiz[i];  // ^
        }
        if (i == OUTLINE_HORIZONTAL - 4 || i == OUTLINE_HORIZONTAL - 5) {
            Output_Array[2 * STRIP_LEN + 74] += outer_horiz[i];  // top line

            Output_Array[2 * STRIP_LEN + 38] += outer_horiz[i];  // -
        }
        if (i == OUTLINE_HORIZONTAL - 4) {
            Output_Array[2 * STRIP_LEN + 51] += outer_horiz[i];  // ^
        }
        if (i == OUTLINE_HORIZONTAL - 5) {
            Output_Array[2 * STRIP_LEN + 50] += outer_horiz[i];  // ^
        }

        if (i == OUTLINE_HORIZONTAL - 6 || i == OUTLINE_HORIZONTAL - 7) {
            Output_Array[2 * STRIP_LEN + 75] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 38] += outer_horiz[i];  // -
            Output_Array[2 * STRIP_LEN + 49] += outer_horiz[i];  // ^
        }

        if (i == OUTLINE_HORIZONTAL - 8 || i == OUTLINE_HORIZONTAL - 9) {
            Output_Array[2 * STRIP_LEN + 76] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 48] += outer_horiz[i];  // ^
            Output_Array[2 * STRIP_LEN + 37] += outer_horiz[i];  // -
        }
        if (i == OUTLINE_HORIZONTAL - 10 || i == OUTLINE_HORIZONTAL - 11) {
            Output_Array[2 * STRIP_LEN + 77] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 47] += outer_horiz[i];  // ^
            Output_Array[2 * STRIP_LEN + 36] += outer_horiz[i];  // -
            Output_Array[2 * STRIP_LEN + 15] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 16] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 17] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 18] += outer_horiz[i];  // C
        }

        if (i == OUTLINE_HORIZONTAL - 12 || i == OUTLINE_HORIZONTAL - 13) {
            Output_Array[2 * STRIP_LEN + 78] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 46] += outer_horiz[i];  // ^
            Output_Array[2 * STRIP_LEN + 35] += outer_horiz[i];  // -
            Output_Array[2 * STRIP_LEN + 14] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 19] += outer_horiz[i];  // C
        }
        if (i == OUTLINE_HORIZONTAL - 14 || i == OUTLINE_HORIZONTAL - 15) {
            Output_Array[2 * STRIP_LEN + 79] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 34] += outer_horiz[i];  // -
            Output_Array[2 * STRIP_LEN + 13] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 20] += outer_horiz[i];  // C
        }
        if (i == OUTLINE_HORIZONTAL - 14)
            Output_Array[2 * STRIP_LEN + 45] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 15)
            Output_Array[2 * STRIP_LEN + 44] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 16 || i == OUTLINE_HORIZONTAL - 17) {
            Output_Array[2 * STRIP_LEN + 80] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 34] += outer_horiz[i];  // -

            Output_Array[2 * STRIP_LEN + 12] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 21] += outer_horiz[i];  // C
        }

        if (i == OUTLINE_HORIZONTAL - 16)
            Output_Array[2 * STRIP_LEN + 43] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 17)
            Output_Array[2 * STRIP_LEN + 42] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 18 || i == OUTLINE_HORIZONTAL - 19) {
            Output_Array[2 * STRIP_LEN + 81] += outer_horiz[i];  // top line
            Output_Array[2 * STRIP_LEN + 33] += outer_horiz[i];  // -

            Output_Array[2 * STRIP_LEN + 11] += outer_horiz[i];  // C
            Output_Array[2 * STRIP_LEN + 22] += outer_horiz[i];  // C
        }

        if (i == OUTLINE_HORIZONTAL - 18)
            Output_Array[2 * STRIP_LEN + 41] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 19)
            Output_Array[2 * STRIP_LEN + 40] += outer_horiz[i];  // ^

        if (i == OUTLINE_HORIZONTAL - 20 || i == OUTLINE_HORIZONTAL - 21) {
            for (int j = 0; j < 11; j++) {
                Output_Array[2 * STRIP_LEN + j] += outer_horiz[i];  // top bar
            }

            for (int j = 23; j < 33; j++) {
                Output_Array[2 * STRIP_LEN + j] += outer_horiz[i];  // bottom bar
            }
        }

        //end of right
    }
}

void do_line_l(CRGB Output_Array[], const CRGB outer_vert[]) {

    for (int i = 0; i < OUTLINE_VERTICAL; i++) {
        if (i == 0 || i == 1) {
            Output_Array[3 * STRIP_LEN + 0] = outer_vert[i];

            for (int j = 0; j < 10; j++)
                Output_Array[3 * STRIP_LEN + j + 72] = outer_vert[i];

            for (int j = 0; j < 9; j++)
                Output_Array[6 * STRIP_LEN + 128 + 8 + j] = outer_vert[i];
        }

        if (i == 2 || i == 3) {
            Output_Array[3 * STRIP_LEN + 1] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 71] = outer_vert[i];
        }
        if (i == 4 || i == 5) {
            Output_Array[3 * STRIP_LEN + 2] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 70] = outer_vert[i];
        }
        if (i == 6 || i == 7) {
            Output_Array[3 * STRIP_LEN + 3] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 69] = outer_vert[i];
        }

        if (i == 8 || i == 9) {
            Output_Array[3 * STRIP_LEN + 4] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 68] = outer_vert[i];
        }
        if (i == 10 || i == 11) {
            Output_Array[3 * STRIP_LEN + 5] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 67] = outer_vert[i];
        }
        if (i == 12 || i == 13) {
            Output_Array[3 * STRIP_LEN + 6] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 66] = outer_vert[i];
        }
        if (i == 14 || i == 15) {
            Output_Array[3 * STRIP_LEN + 7] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 65] = outer_vert[i];
        }
        if (i == 16 || i == 17) {
            Output_Array[3 * STRIP_LEN + 8] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 64] = outer_vert[i];
        }
        if (i == 18 || i == 19) {
            Output_Array[3 * STRIP_LEN + 9] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 63] = outer_vert[i];
        }
        if (i == 20 || i == 21) {
            for (int j = 0; j < 6; j++)
                Output_Array[3 * STRIP_LEN + 10 + j] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 62] = outer_vert[i];

            for (int j = 0; j < 8; j++)
                Output_Array[6 * STRIP_LEN + 128 + j] = outer_vert[i];
        }

        if (i == 22 || i == 23) {
            Output_Array[3 * STRIP_LEN + 16] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 61] = outer_vert[i];
        }
        if (i == 24 || i == 25) {
            Output_Array[3 * STRIP_LEN + 17] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 60] = outer_vert[i];
        }

        if (i == 26 || i == 27) {
            for (int j = 0; j < 6; j++)
                Output_Array[3 * STRIP_LEN + 18 + j] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 59] = outer_vert[i];

            for (int j = 0; j < 7; j++)
                Output_Array[4 * STRIP_LEN + 60 + 24 + j] = outer_vert[i];
        }

        if (i == 28 || i == 29) {
            Output_Array[3 * STRIP_LEN + 24] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 58] = outer_vert[i];
            for (int j = 0; j < 4; j++)
                Output_Array[4 * STRIP_LEN + 60 + 23 - j] = outer_vert[i];  // chin

            for (int j = 0; j < 4; j++)
                Output_Array[3 * STRIP_LEN + 46 + j] = outer_vert[i];  // side  ^
        }
        if (i == 30 || i == 31) {
            Output_Array[3 * STRIP_LEN + 25] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 57] = outer_vert[i];

            Output_Array[4 * STRIP_LEN + 60 + 19] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 0] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 45] = outer_vert[i];  // side  ^
            Output_Array[3 * STRIP_LEN + 50] = outer_vert[i];  // side  ^
        }
        if (i == 32 || i == 33) {
            Output_Array[3 * STRIP_LEN + 26] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 56] = outer_vert[i];

            Output_Array[4 * STRIP_LEN + 60 + 18] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 1] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 44] = outer_vert[i];  // side  ^
            Output_Array[3 * STRIP_LEN + 51] = outer_vert[i];  // side  ^
        }
        if (i == 34 || i == 35) {
            Output_Array[3 * STRIP_LEN + 27] = outer_vert[i];
            Output_Array[3 * STRIP_LEN + 55] = outer_vert[i];
            Output_Array[4 * STRIP_LEN + 60 + 17] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 2] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 43] = outer_vert[i];  // side  ^
            Output_Array[3 * STRIP_LEN + 52] = outer_vert[i];  // side  ^
        }
        if (i == 36 || i == 37) {
            Output_Array[3 * STRIP_LEN + 28] = outer_vert[i];
            Output_Array[4 * STRIP_LEN + 60 + 16] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 3] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 54] = outer_vert[i];

            Output_Array[3 * STRIP_LEN + 42] = outer_vert[i];  // side  ^
            Output_Array[3 * STRIP_LEN + 53] = outer_vert[i];  // side  ^
        }

        if (i == 38 || i == 39) {
            Output_Array[3 * STRIP_LEN + 29] = outer_vert[i];
            Output_Array[4 * STRIP_LEN + 60 + 15] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 4] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 41] = outer_vert[i];  // side  ^
        }
        if (i == 38) {
            Output_Array[3 * STRIP_LEN + 39] = outer_vert[i];
        }
        if (i == 39) {
            Output_Array[3 * STRIP_LEN + 38] = outer_vert[i];
        }

        if (i == 40 || i == 41) {
            Output_Array[3 * STRIP_LEN + 30] = outer_vert[i];
            Output_Array[4 * STRIP_LEN + 60 + 14] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 5] = outer_vert[i];   // chin

            Output_Array[3 * STRIP_LEN + 40] = outer_vert[i];  // side  ^
        }

        if (i == 40) {
            Output_Array[3 * STRIP_LEN + 37] = outer_vert[i];
        }
        if (i == 41) {
            Output_Array[3 * STRIP_LEN + 36] = outer_vert[i];
        }

        if (i == 42 || i == 43) {
            Output_Array[3 * STRIP_LEN + 31] = outer_vert[i];
            Output_Array[4 * STRIP_LEN + 60 + 13] = outer_vert[i];  // chin
            Output_Array[4 * STRIP_LEN + 60 + 6] = outer_vert[i];   // chin
        }
        if (i == 42) {
            Output_Array[3 * STRIP_LEN + 35] = outer_vert[i];
        }
        if (i == 43) {
            Output_Array[3 * STRIP_LEN + 34] = outer_vert[i];
        }

        if (i == 44) {
            Output_Array[3 * STRIP_LEN + 33] = outer_vert[i];
        }
        if (i == 44 || i == 45) {
            Output_Array[3 * STRIP_LEN + 32] = outer_vert[i];

            for (int j = 0; j < 6; j++)
                Output_Array[4 * STRIP_LEN + 60 + 7 + j] = outer_vert[i];  // chin
        }
    }

    // copy left to right
   memcpy(&Output_Array[2 * STRIP_LEN], &Output_Array[3 * STRIP_LEN], 82 * sizeof(CRGB));
}

void render(CRGB Output_Array[], const struct popstar_struct *helmet) {
    //update the array
    for (uint8_t y = 0; y < 16; y++) {
        for (uint8_t x = 0; x < 24; x++) {
            CRGB final_color = helmet->Background_Array[x][y];
            //  CRGB final_color = CRGB(0, 0, 0);
            /* if (supress_leds == false) {
                final_color = helmet.Background_Array[x][y];*/
            if (helmet->background_mode <= PATTERN_NOISE_LAST && helmet->background_mode >= PATTERN_NOISE_FIRST)
                final_color += Snapshot_Array[x][y];

            Output_Array[XY(x, y)] = final_color;
        }
    }

    do_chin(Output_Array, helmet);

   
    do_line_l(Output_Array, helmet->outer_vert);
   do_line_h(Output_Array, helmet->outer_horiz);


    decay_snapshots();
}

void render_snapshot_check(const struct popstar_struct *helmet) {
    if (helmet->background_mode != helmet->background_mode_last) {
        memcpy(Snapshot_Array, helmet->Background_Array, MATRIX_WIDTH * MATRIX_HEIGHT * sizeof(CRGB));
    }
    
}
