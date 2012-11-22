#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint8_t tick(void) {
        static uint8_t xo = 0;
        xo++;
        static uint8_t y = 0;
        if (!(xo & 0x3))
                y++;

        for(int x = 0; x < LED_WIDTH; x++)
        {
                uint8_t xr = x + xo;
                setLedX(x, xr ^ y, !xr ^ y, xr ^ !y);
        }

	return 0;
}

static void init(void)
{
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, 4, 600);
}




