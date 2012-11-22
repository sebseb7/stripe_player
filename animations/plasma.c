#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t a = 0;

static uint8_t tick(void) {

	
	uint8_t x;

	uint16_t sin1 = sini(a);
	float x0 = (float)sini(a*10)/256-64;
	float y0 = (float)sini((a*10)+0x1000)/256-64;
	float x1 = (float)sini(a*20)/128-128;
	float y1 = (float)sini((a*20)+0x1000)/128-128;
	
		
	uint16_t y_part =  sini(sin1);


	for(x = 0; x < LED_WIDTH; x++) 
	{
		
		float dist = pythagorasf(x0-x,y0);
		float dist2 = pythagorasf(y1-x,x1);


		uint16_t h = sini(sin1+x*12)+ y_part;
		h += sini(dist*50);
		h += sini(dist2*30);
		//uint16_t h = sini(sin1+x*600)+ y_part + sini(dist*500) + sini(dist2*300);
		setLedX(
			x,
			sini((h>>2)+a*300)>>8,
			sini((h>>2)+a*400)>>8,
			sini((h>>2)+a*500)>>8
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
	registerAnimation(init,tick,deinit, 29, 600);
}




