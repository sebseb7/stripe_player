#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "libs/math.h"

static uint16_t b = 0;

static uint8_t tick(void) {

	int x;

	if(b==0)
		for(x=0;x<LED_WIDTH;x++)
			setLedX(x,255,0,0);
	if(b==1)
		for(x=0;x<LED_WIDTH;x++)
			setLedX(x,0,255,0);
	if(b==2)
		for(x=0;x<LED_WIDTH;x++)
			setLedX(x,0,0,255);
	
	b++;
	if(b==3)
		b=0;
	return 0;
}

static void init(void)
{
	b = 0;
}
static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, 140, 4000);
}




