#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math_emb.h"
#include "libs/hsv2rgb.h"

static uint16_t h = 0;
static uint16_t t = 0;
static int curr_sec = 0;

#define SECTORS 8
#define SEC_SIZE LED_WIDTH / SECTORS 


static uint8_t tick(void) {

	h+=2;
	t+=1;
	if(h>=360)
		h=0;

	struct hsv_colour hsv;
	struct rgb_colour rgb;

	hsv.h = h/360.0f;
	hsv.s = 1;
	hsv.v = 1;

	hsv2rgb( &hsv, &rgb );

	float norm = sqrtf(rgb.r*rgb.r + rgb.b*rgb.b + rgb.g*rgb.g);
	rgb.r /= norm;
	rgb.g /= norm;
	rgb.b /= norm;   


	int step = t % 14;

	if(step==0)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		curr_sec = rand()%8;
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if(step==4)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x+5,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if(step==8)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if ( (step==2)||(step==6)||(step==10))
	{
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
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
	registerAnimation(init,tick,deinit, 40, 1000,0);
}




