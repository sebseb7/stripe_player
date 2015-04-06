#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math_emb.h"
#include "libs/hsv2rgb.h"

static uint16_t h = 0;
static uint16_t t = 0;
static int curr_sec = 0;

#define SECTORS 4
#define SEC_SIZE LED_WIDTH / 4 


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



	if((t&0x7)==1)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		curr_sec = rand()%4;
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if((t&0x7)==2)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x+1,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if((t&0x7)==3)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x+2,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if((t&0x7)==4)
	{
//		curr_sec++;
//		if(curr_sec == SECTORS)
//		{
//			curr_sec=0;
//		}
		curr_sec = rand()%4;
		for(int x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,0);
		for(int x=curr_sec*SEC_SIZE;x<(curr_sec+1)*SEC_SIZE;x++)
			setLedX(x+3,rgb.r*255,rgb.g*255,rgb.b*255);
	}
	else if ((t&0x7) == 5)
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
	registerAnimation(init,tick,deinit, 30, 1000,0);
}




