#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static uint8_t tick(void) {

	
	float x0 = (float)sini(a*40)/(0xffff/LED_WIDTH);
	float x1 = (float)sini(a*70)/(0xffff/LED_WIDTH);
	float x2 = (float)sini(a*103)/(0xffff/LED_WIDTH);

	for(int x = 0; x < LED_WIDTH; x++) 
	{

		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		uint8_t diff0 = abs(x0-x);
		uint8_t diff1 = abs(x1-x);
		uint8_t diff2 = abs(x2-x);

		if(diff0 < 32)
		{
			red = (32-diff0)*4;
		}
		if(diff1 < 32)
		{
			green = (32-diff1)*4;
		}
		if(diff2 < 32)
		{
			blue = (32-diff2)*4;
		}

		setLedX(
			x,
			red,
			green,
			blue
		);
	}
	a+=1;
	if(a==0x4000)
	{
		a=0;
	}
	return 0;
}

static void init(void)
{
	a = 0;
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, 35, 1000,1);
}


