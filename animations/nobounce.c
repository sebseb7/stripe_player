#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"


static float pos_r = 0.0f;
static float pos_g = 0.0f;
static float pos_b = 0.0f; 

static const uint8_t item[] = {250,250,250,240,200,150,50,1};

static uint8_t tick(void) {

	

	for(int x = 0; x < LED_WIDTH; x++) 
	{

		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		uint8_t diff0 = abs(pos_r-x);
		uint8_t diff1 = abs(pos_b-x);
		uint8_t diff2 = abs(pos_g-x);

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
	
	pos_r--;
	pos_g+=2.1f;
	pos_b+=1.2f;

	if(pos_r < 0)
	{
		pos_r+=LED_WIDTH;
	}
	if(pos_g > LED_WIDTH)
	{
		pos_g-=LED_WIDTH;
	}
	if(pos_b > LED_WIDTH)
	{
		pos_b-=LED_WIDTH;
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
	registerAnimation(init,tick,deinit, 35, 1000,1);
}


