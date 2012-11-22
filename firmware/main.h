#ifndef MAIN_H_
#define MAIN_H_

#if STM32F == 2
#include "stm32f2xx.h"
#endif
#if STM32F == 4
#include "stm32f4xx.h"
#endif

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

uint32_t getSysTick(void);

typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);

void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t t, uint16_t duration);

void setLedX(uint8_t x, uint8_t r, uint8_t g, uint8_t b);
void fillRGB(uint8_t r, uint8_t g, uint8_t b);

#define LED_WIDTH 192
#define LED_HEIGHT 1
#define STRIPE

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#define CCM_ATTRIBUTES
//#define CCM_ATTRIBUTES	__attribute__ ((section(".ccm")));

#endif
