#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"
#include <string.h>
#include<sys/time.h>



#define FRAMETIME 29

int sdlpause = 0;

#define MAX_ANIMATIONS 200
#define MAX_APPS 200

int animationcount = 0;
int appcount = 0;

struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int duration;
	int min_delay;
} animations[MAX_ANIMATIONS];


struct app {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	int min_delay;
} apps[MAX_APPS];


SDL_Surface* screen;
void Delay(uint16_t t)
{
	
}

int leds[LED_WIDTH][4];
void setLedX(uint8_t x, uint8_t red,uint8_t green,uint8_t blue) {
	if (x >= LED_WIDTH) return;
	leds[x][0] = red;
	leds[x][1] = green;
	leds[x][2] = blue;
	leds[x][3] = 1;
}

void invLedX(uint8_t x) {
	if (x >= LED_WIDTH) return;
	leds[x][0] = 255 - leds[x][0];
	leds[x][1] = 255 - leds[x][1];
	leds[x][2] = 255 - leds[x][2];
	leds[x][3] = 1;
}
void getLedX(uint8_t x, uint8_t* red, uint8_t* green, uint8_t* blue) {
	if (x >= LED_WIDTH) return;
	*red = leds[x][0];
	*green = leds[x][1];
	*blue = leds[x][2];
}

void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
{
	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].duration = count;
	animations[animationcount].min_delay = t;

	animationcount++;

}


void fillRGB(uint8_t r,uint8_t g, uint8_t b)
{
	int x;

	for(x = 0; x < LED_WIDTH; x++) {
		leds[x][0]=r;
		leds[x][1]=g;
		leds[x][2]=b;
		leds[x][3]=1;
	}
}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	fillRGB(0,0,0);

	srand(time(NULL));


	int current_animation = 0;

	screen = SDL_SetVideoMode(LED_WIDTH*ZOOM_X,ZOOM_Y,32, SDL_SWSURFACE | SDL_DOUBLEBUF);


	animations[current_animation].init_fp();
	
	int tick_count = 0;
	int running = 1;
	//unsigned long long int startTime = get_clock();
	Uint32 lastFrame = SDL_GetTicks(); 

	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							if(sdlpause == 0)
							{
								sdlpause = 1;
							}
							else{
								sdlpause = 0;
							}
							break;
						default: break;
					}
				default: break;
			}
		}

		animations[current_animation].tick_fp();


	


		
		int x;
		for(x = 0; x < LED_WIDTH; x++) {

			if(leds[x][3] == 1)
			{
				SDL_Rect rect = { ZOOM_X*x+(ZOOM_X/15), 0, ZOOM_X-(ZOOM_X/15), ZOOM_Y};
				SDL_FillRect(
					screen, 
					&rect, 
					SDL_MapRGB(screen->format, leds[x][0],leds[x][1],leds[x][2])
				);

				leds[x][3] = 0;

			}

		}

		
		SDL_Flip(screen);


		Uint32 now = SDL_GetTicks(); 

		if( (now - lastFrame) < FRAMETIME )
		{
			SDL_Delay(FRAMETIME - (now - lastFrame));
		}
		lastFrame = SDL_GetTicks();

		
		tick_count++;


		if(tick_count == animations[current_animation].duration)
		{
			animations[current_animation].deinit_fp();

			current_animation++;
			if(current_animation == animationcount)
			{
				current_animation = 0;
			}
			tick_count=0;

			SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM_X,ZOOM_Y };
			SDL_FillRect(
				screen, 
				&rect, 
				SDL_MapRGB(screen->format,0,0,0)
			);
			SDL_Flip(screen);
			SDL_Delay(300);


			animations[current_animation].init_fp();


		}
	}

	SDL_Quit();
	return 0;
}

