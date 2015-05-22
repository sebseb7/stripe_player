#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math_emb.h"
#include "libs/hsv2rgb.h"

static uint16_t t = 0;

#define TRAVEL LED_WIDTH / 5
#define SHARK_LENGTH LED_WIDTH / 12

uint8_t state = 0;
uint8_t start_idx = 0;
uint8_t direction = 0;

static uint8_t tick(void) {

	t+=1;

	state++;
	if(state == TRAVEL)
	{
		state = 0;
	}


	if(state == 0)
	{
		direction = (rand() % 2);

		if(direction == 0)
		{
			start_idx = (rand() %  (LED_WIDTH-(TRAVEL+SHARK_LENGTH)))+TRAVEL+SHARK_LENGTH;
		}
		else
		{
			start_idx = (rand() %  (LED_WIDTH-(TRAVEL+SHARK_LENGTH)));
		}
	}
	else
	{
		fillRGB(0,0,0);

		for(int x = 0;x<=SHARK_LENGTH*2;x++)
		{
			if(direction == 0)
			{
				setLedX((start_idx+x)-state,255,255,255);
			}
			else
			{
				setLedX(x+start_idx+state,255,255,255);
			}
		}
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
	registerAnimation(init,tick,deinit, 35, 1000,0);
}




