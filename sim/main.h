#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

enum {
	LED_WIDTH = (32*3),
	ZOOM_X = 12,
	ZOOM_Y = 50,
	FIRST_ON = 22
};

#define STRIPE
#define SIMULATOR

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

int sdlpause;

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration, uint8_t idle);
void setLedX(uint16_t x, uint8_t r,uint8_t g,uint8_t b);
void invLedX(uint16_t x);
void Delay(uint16_t t);
void getLedX(uint16_t x, uint8_t* red, uint8_t* green, uint8_t* blue);
#endif

