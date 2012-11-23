#include "main.h"
#include "libs/spi.h"
#include "libs/math.h"

/*
 *	boot loader: http://www.st.com/stonline/stappl/st/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/APPLICATION_NOTE/CD00167594.pdf (page 31)
 *  data sheet : http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00277537.pdf
 *
 *
 */

static uint8_t color_correction[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,20,20,20,21,21,22,22,23,23,23,24,24,25,25,26,26,27,27,28,29,29,30,30,31,32,32,33,33,34,35,35,36,37,38,38,39,40,41,41,42,43,44,45,46,47,48,48,49,50,51,52,53,54,56,57,58,59,60,61,62,64,65,66,67,69,70,71,73,74,76,77,79,80,82,83,85,87,88,90,92,93,95,97,99,101,103,105,107,109,111,113,115,118,120,122,125,127};

static __IO uint32_t TimingDelay;
static __IO uint32_t tick;
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime*10;

	while(TimingDelay != 0);
}
static void Delay100us(__IO uint32_t nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	tick++;
}
uint32_t getSysTick(void)
{
	return tick;
}

#define MAX_ANIMATIONS 30

static int animationcount;



struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t duration;
	uint16_t timing;
};

static struct animation animations[MAX_ANIMATIONS];



void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count)
{

	// this is for initialization, probably registerAnimation gets called bevore global variables are initialized
	static int firstrun = 1;

	if(firstrun == 1)
	{
		firstrun = 0;
		animationcount = 0;
	}


	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].duration = count;
	animations[animationcount].timing = 10000 / t;

	animationcount++;
}

uint8_t leds[LED_WIDTH][3];

void setLedX(uint8_t x, uint8_t red,uint8_t green,uint8_t blue) {
	if (x >= LED_WIDTH) return;
	leds[x][0] = red;
	leds[x][1] = green;
	leds[x][2] = blue;
}

void fillRGB(uint8_t red,uint8_t green,uint8_t blue) 
{
	for(int x= 0;x < LED_WIDTH;x++)
	{
		leds[x][0] = red;
		leds[x][1] = green;
		leds[x][2] = blue;
	}
}

void lcdFlush(void)
{
	spi_send(0);
	for(int x= 0;x < LED_WIDTH;x++)
	{
		spi_send (0x80 | color_correction[leds[x][1]] );
		spi_send (0x80 | color_correction[leds[x][0]] );
		spi_send (0x80 | color_correction[leds[x][2]] );
	}
	spi_send(0);
}


int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;


	RCC_GetClocksFreq(&RCC_Clocks);
	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 10000);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;       
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	

	init_spi();
	
	
	int current_animation = 0;
	animations[current_animation].init_fp();
	int tick_count = 0;
	
	
/*	while(1)
	{
		spi_send(0);
		for(int x= 0;x < 194;x++)
		{
			spi_send (0x80 | 0xff );
			spi_send (0x80 | 0xff );
			spi_send (0x80 | 0xff );
		}
		spi_send(0);
		Delay(200);
		spi_send(0);
		for(int x= 0;x < 194;x++)
		{
			spi_send (0x80 | 0x00 );
			spi_send (0x80 | 0x00 );
			spi_send (0x80 | 0x00 );
		}
		spi_send(0);
		Delay(200);
	}
*/
	
	

	int loopcount = 0;
	

	while(1)
	{
		loopcount++;
		if((loopcount == 55)||(loopcount == 57))
		{
			GPIOB->ODR           |=       1<<13;
		}
		if((loopcount == 56)||(loopcount == 58))
		{
			GPIOB->ODR           &=       ~(1<<13);

			if(loopcount==58)
				loopcount = 0;
		}
		
		uint32_t start_tick = tick;

		animations[current_animation].tick_fp();

#ifdef lun1k
#else
		GPIOB->ODR           |=       1<<12;
#endif

		lcdFlush();

#ifdef lun1k
#else
		GPIOB->ODR           &=       ~(1<<12);
#endif

		uint32_t duration = tick - start_tick;

		if(animations[current_animation].timing - duration > 0)
			Delay100us(animations[current_animation].timing - duration);

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
	
			fillRGB(0,0,0);

			animations[current_animation].init_fp();


		}
	}

}

