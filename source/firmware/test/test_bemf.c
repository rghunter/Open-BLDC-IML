#include <stm32/rcc.h>
#include <stm32/flash.h>
#include <stm32/misc.h>
#include <stm32/gpio.h>
#include <stm32/tim.h>
#include <stm32/exti.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/systick.h>

#include "types.h"
#include "driver/led.h"
#include "driver/usart.h"
#include "driver/debug_pins.h"
//#include "driver/bemf_hardware_detect.h"

/**
 * Initialize the STM32
 */


EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;


static void system_init(void)
{
	/* Initialize the microcontroller system. Initialize clocks. */
	SystemInit();
/*	rcc_clock_setup_in_hse_12mhz_out_72mhz();
	systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);
	rcc_set_ppre1(RCC_CFGR_PPRE1_HCLK_DIV2);*/
}
/**
 * Blink test main function
 *
 * @return Nothing really...
 */



int main(void)
{

	system_init();
	led_init();
	int flag = 0;

	  /* Enable GPIOB clock */
	  /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	  /* Configure PB.09 pin as input floating */
	  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* Enable AFIO clock */
	 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	  /* Connect EXTI9 Line to PB.09 pin */
	 // GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);

	  /* Configure EXTI9 line */
	 /* EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
	/*  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	  NVIC_Init(&NVIC_InitStructure);*/

	while(1)
	{
		if(!(flag % 100000))
		{
			TOGGLE(LED_RED);
			//DEBUG("HELLO WORLD\n");
			flag++;
		}else
		{
			flag++;
		}

	}
}

void exti0_irq_handler(void)
{
	TOGGLE(LED_BLUE);

    EXTI_ClearITPendingBit(EXTI_Line0);
}


