#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static uint8_t tick(void) {

	
	uint8_t x;

	uint16_t sin1 = sini(a);
		
	uint16_t y_part =  sini(sin1);


	for(x = 0; x < LED_WIDTH; x++) 
	{
		uint16_t h = sini(x*20)+ y_part;

		uint8_t r = sini((h>>2)+a*30)>>8;
		uint8_t g = sini((h>>2)+a*40)>>8;
		uint8_t b = sini((h>>2)+a*50)>>8;

		if(r < FIRST_ON)
			r = FIRST_ON;
		if(g < FIRST_ON)
			g = FIRST_ON;
		if(b < FIRST_ON)
			b = FIRST_ON;

		setLedX(x,r,g,b);
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
	registerAnimation(init,tick,deinit, 35, 1600,1);
}


