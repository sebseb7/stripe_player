#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math_emb.h"
#include "libs/hsv2rgb.h"

static uint16_t t = 0;
static int curr_sec = 0;

#define NUM_SECTORS 5
#define SEC_SIZE LED_WIDTH / NUM_SECTORS


static uint16_t sectors_h[NUM_SECTORS];
static uint16_t sectors_v[NUM_SECTORS];


static uint8_t tick(void) {

	t+=1;


	

	int step = t % 20;

	if(step==0)
	{
		curr_sec = rand() % NUM_SECTORS;
		sectors_h[curr_sec]= rand() % 360;
		sectors_v[curr_sec] = 60;
	}

		

	for(int i = 0; i < NUM_SECTORS;i++)
	{
	

		struct hsv_colour hsv;
		struct rgb_colour rgb;

		hsv.h = sectors_h[i]/360.0f;
		hsv.s = 1;
		hsv.v = sectors_v[i]/60.0f;
		
		
		if((t % 3) > 0)
			hsv.v=0;

		hsv2rgb( &hsv, &rgb );

/*		float norm = sqrtf(rgb.r*rgb.r + rgb.b*rgb.b + rgb.g*rgb.g);
		rgb.r /= norm;
		rgb.g /= norm;
		rgb.b /= norm;   
*/


		for(int x=i*SEC_SIZE;x<(i+1)*SEC_SIZE;x++)
			setLedX(x,rgb.r*255,rgb.g*255,rgb.b*255);

			if(sectors_v[i] > 0)
				sectors_v[i]--;
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
	registerAnimation(init,tick,deinit, 60, 1000,0);
}




