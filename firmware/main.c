#include "main.h"
#include "libs/math_emb.h"


#include "libs/spi.h"
#include "libs/usb_serial.h"

/*
 *	boot loader: http://www.st.com/stonline/stappl/st/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/APPLICATION_NOTE/CD00167594.pdf (page 31)
 *  data sheet : http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00277537.pdf
 *
 *
 */

static const uint8_t color_correction[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,20,20,20,21,21,22,22,23,23,23,24,24,25,25,26,26,27,27,28,29,29,30,30,31,32,32,33,33,34,35,35,36,37,38,38,39,40,41,41,42,43,44,45,46,47,48,48,49,50,51,52,53,54,56,57,58,59,60,61,62,64,65,66,67,69,70,71,73,74,76,77,79,80,82,83,85,87,88,90,92,93,95,97,99,101,103,105,107,109,111,113,115,118,120,122,125,127};

static uint16_t key_state = 0;
static uint16_t key_press = 0;
static uint32_t buttonsInitialized = 0;
//static int mode = 0;
//static int dim = 0;

static __IO uint32_t TimingDelay = 0;
static __IO uint32_t tick = 0;
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
	static uint16_t ct0=0, ct1=0;
	uint16_t i;
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	if(buttonsInitialized)
	{
		uint16_t key_curr = ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)<<1)|
							  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));

		i = key_state ^ ~key_curr;
		ct0 = ~( ct0 & i );
		ct1 = ct0 ^ (ct1 & i);
		i &= ct0 & ct1;
		key_state ^= i;
		key_press |= key_state & i;
	}
	tick++;
}
uint32_t getSysTick(void)
{
	return tick;
}
uint16_t get_key_press( uint16_t key_mask )
{
	key_mask &= key_press;                          // read key(s)
	key_press ^= key_mask;                          // clear key(s)
	return key_mask;
}

uint16_t get_key_state( uint16_t key_mask )
{
	return key_mask & key_press;
}

#define MAX_ANIMATIONS 30

static int animationcount;



struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t duration;
	uint16_t timing;
	uint8_t idle;
};

static struct animation animations[MAX_ANIMATIONS];



void registerAnimation(init_fun init,tick_fun tick, deinit_fun deinit,uint16_t t, uint16_t count, uint8_t idle)
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
	animations[animationcount].idle = idle;
	animations[animationcount].timing = 10000 / t;

	animationcount++;
}

static uint8_t leds[LED_WIDTH][3];

void setLedX(uint16_t x, uint8_t red,uint8_t green,uint8_t blue) {
	if (x >= LED_WIDTH) return;



	leds[x][0] = color_correction[green];
	leds[x][1] = color_correction[red];
	leds[x][2] = color_correction[blue];

}

void fillRGB(uint8_t red,uint8_t green,uint8_t blue) 
{
	for(int x= 0;x < LED_WIDTH;x++)
	{
		leds[x][0] = color_correction[green];
		leds[x][1] = color_correction[red];
		leds[x][2] = color_correction[blue];
	}
}

#ifndef WS2812B
static void lcdFlush(void)
{



	for(int x= 0;x < LED_WIDTH;x++)
	{
		spi_send (0x80 | (color_correction[leds[x][1]]>>dim) );
		spi_send (0x80 | (color_correction[leds[x][0]]>>dim) );
		spi_send (0x80 | (color_correction[leds[x][2]]>>dim) );
	}
	spi_send(0);
	

	if(mode == 0)
	{
		spi_send (0x80 | 1);
		spi_send (0x80 | 0);
		spi_send (0x80 | 0);
	}
	if(mode == 1)
	{
		spi_send (0x80 | 0);
		spi_send (0x80 | 1);
		spi_send (0x80 | 0);
	}
	if(mode == 2)
	{
		spi_send (0x80 | 0);
		spi_send (0x80 | 0);
		spi_send (0x80 | 1);
	}
	
	spi_send (0x80 | 0);
	spi_send (0x80 | 0);
	spi_send (0x80 | 0);


}
#endif


static uint32_t itmode = 0;


static uint32_t led_nr = 0;
static uint32_t col_nr = 0;
static uint32_t bit_nr = 8;


//toggle a pin to check length of interrupt and optimize it (use bitbanding for led access and interrupt flags)
//even more sophisticated: use DMA to update CCR1
//
void TIM3_IRQHandler(void)
{
//	if(((TIM3->SR & TIM_IT_Update) == (uint16_t)RESET) ||  ((TIM3->DIER & TIM_IT_Update) == (uint16_t)RESET))

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		//TIM3->SR = (uint16_t)~TIM_IT_Update;
		itmode++;
#define XYZ (LED_WIDTH*3*8)
		if(itmode < XYZ)
		{
		
			if((leds[led_nr][col_nr] & (1<<(bit_nr-1))) == (1<<(bit_nr-1)) )
			{
				TIM3->CCR1=67u;
			}else
			{
				TIM3->CCR1=34u;
			}
		}
		
		if(itmode == XYZ)
		{
			TIM3->PSC=13u;
			TIM3->CCR1=34u;
		}
		if(itmode == XYZ+1)
		{
			TIM3->CCR1=0u;
		}
		if(itmode == XYZ+2)
		{
			itmode =0;
			led_nr = 0;
			col_nr = 0;
			bit_nr = 7;
				

			TIM3->PSC=0u;
			if((leds[0][0] & (1<<7)) == (1<<7) )
			{
				TIM3->CCR1=67u;
			}else
			{
				TIM3->CCR1=34u;
			}
		}

		bit_nr--;
		if(bit_nr == 0)
		{
			bit_nr=8;
			col_nr++;
			if(col_nr==3)
			{
				col_nr=0;
				led_nr++;
			}
		}
	}
}

int next_animation = 0;
void process_usb_byte(uint8_t rx)
{
	if(rx == 0x6a)
	{
		next_animation = 1;
	}

	//USB_OTG_DisableGlobalInt(&USB_OTG_dev);
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
	
	//buttons
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	buttonsInitialized=1;
		
		

#ifdef USE_USB_OTG_FS
	usb_serial_init();
#endif

#ifdef WS2812B

	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;


		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);


		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

		TIM_TimeBaseStructure.TIM_Period = 104u;
		TIM_TimeBaseStructure.TIM_Prescaler = 0u;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0u;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_Pulse = 0u;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(TIM3, ENABLE);

		//0: 34 vs. 1:67

		TIM3->CCR1 = 67u;

		NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM3 gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM3, ENABLE);

	}

#else
	init_spi();
#endif	
	
#ifdef WS2812B

#else
	spi_send(0);
#endif	


	int current_animation = 0;
	animations[current_animation].init_fp();
	int tick_count = 0;
	
	fillRGB(0,0,0);

	int loopcount = 0;
	while(1)
	{
		loopcount++;
		if((loopcount == 50)||(loopcount == 150))
		{
			GPIOB->ODR           |=       1<<13;
			GPIOB->ODR           &=       ~(1<<12);
		}
		if((loopcount == 100)||(loopcount == 200))
		{
			GPIOB->ODR           &=       ~(1<<13);
			GPIOB->ODR           |=       1<<12;

			if(loopcount==200)
				loopcount = 0;
		}

		uint32_t start_tick = tick;

		animations[current_animation].tick_fp();

		//lcdFlush();

		uint32_t duration = tick - start_tick;

		if(animations[current_animation].timing - duration > 0)
			Delay100us(animations[current_animation].timing - duration);

		tick_count++;

	/*
	   if(mode != 2)
	   tick_count++;

	   if(get_key_press(KEY_B))
	   {
	   if(get_key_state( KEY_A))
	   {
	   mode++;
	   if(mode == 3)
	   mode = 0;
	   }
	   else
	   {
	   dim++;
	   if(dim == 5)
	   dim = 0;
	   }
	   }
	*/

		//if( (tick_count == animations[current_animation].duration) || get_key_press( KEY_A) )
		//if(tick_count == animations[current_animation].duration )
		if((tick_count == animations[current_animation].duration )||(next_animation == 1))
		{
			next_animation = 0;
#ifdef USE_USB_OTG_FS
			usb_printf("switch\n");
#endif
			animations[current_animation].deinit_fp();

			tick_count=0;

			//get_key_press( KEY_A)
				
			current_animation++;
			if(current_animation == animationcount)
			{
				current_animation = 0;
			}

			fillRGB(0,0,0);

			animations[current_animation].init_fp();

		}
	}

}

