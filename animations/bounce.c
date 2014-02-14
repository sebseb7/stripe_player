#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static const uint8_t item[] = {250,250,250,240,200,150,50,1};

static uint8_t tick(void) {

	
	float x0 = (float)sini(a*40)/(0xffff/(LED_WIDTH+20))-10;
	float x1 = (float)sini(a*70)/(0xffff/(LED_WIDTH+20))-10;
	float x2 = (float)sini(a*103)/(0xffff/(LED_WIDTH+20))-10;

	for(int x = 0; x < LED_WIDTH; x++) 
	{

		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		uint16_t diff0 = abs(x0-x);
		uint16_t diff1 = abs(x1-x);
		uint16_t diff2 = abs(x2-x);

		if(diff0 < 8)
		{
			red = item[diff0];
		}
		if(diff1 < 8)
		{
			green= item[diff1];
		}
		if(diff2 < 8)
		{
			blue = item[diff2];
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


