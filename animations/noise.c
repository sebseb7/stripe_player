#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math_emb.h"
#include "libs/hsv2rgb.h"


static uint8_t tick(void) {


	struct hsv_colour hsv;
	struct rgb_colour rgb;

//	for(int x=0;x<LED_WIDTH;x++)
	{

		int x = rand()%LED_WIDTH;

		hsv.h = (rand()%360)/360.0f;
		hsv.s = 1;
		hsv.v = 1;

		hsv2rgb( &hsv, &rgb );

		float norm = sqrtf(rgb.r*rgb.r + rgb.b*rgb.b + rgb.g*rgb.g);
		rgb.r /= norm;
		rgb.g /= norm;
		rgb.b /= norm;   

		setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);
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
	registerAnimation(init,tick,deinit, 200, 2000,0);
}




