#include <stdlib.h>
#include "main.h"


static uint8_t tick(void) {

	for(int x=0;x<LED_WIDTH;x++)
		setLedX(x,255,255,255);
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
	registerAnimation(init,tick,deinit, 35, 600);
}




