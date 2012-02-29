/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   bemf_hardware_detect.c
 * @author Piotr Esden-Tempski <piotr@esden.net>
 * @date   Tue Aug 17 01:37:44 2010
 *
 * @brief  Hardware based commutation detection implementation.
 *
 * This implementation uses a comparator output as source of zero crossing
 * detection.
 */

#include "config.h"

#include "types.h"

#include "bemf_hardware_detect.h"

#include <stm32/misc.h>
#include <stm32/exti.h>
#include <stm32/rcc.h>
#include <stm32/gpio.h>

#include "driver/led.h"
#include "driver/debug_pins.h"
#include "comm_tim.h"

struct bemf_hd_data bemf_hd_data;
u16 bemf_line_state;

#ifdef BEMF_HD__USE_LED
#  ifdef BEMF_HD__LED_TOGGLE
#    define BEMF_HD_LED_RISING() TOGGLE(LED_GREEN)
#    define BEMF_HD_LED_FALLING() TOGGLE(LED_GREEN)
#  else
#    define BEMF_HD_LED_RISING() OFF(LED_ORANGE)
#    define BEMF_HD_LED_FALLING() ON(LED_ORANGE)
#  endif
#else
#    define BEMF_HD_LED_RISING() ((void)0)
#    define BEMF_HD_LED_FALLING() ((void)0)
#endif

/**
 * Initialize the hardware based BEMF detection peripherals
 */
void bemf_hd_init(void)
{
	NVIC_InitTypeDef nvic;
	EXTI_InitTypeDef exti;
	GPIO_InitTypeDef gpio;


	bemf_hd_reset();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);



	/* GPIOA: EXTI Pin 0, 1, 2 as interrupt input
	 * Pin 0 -> BEMF/I_Sense of PHASE A
	 * Pin 1 -> BEMF/I_Sense of PHASE B
	 * Pin 2 -> BEMF/I_Sense of PHASE C
	 */
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	//gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);

	exti.EXTI_Line = EXTI_Line10;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	exti.EXTI_Line = EXTI_Line11;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	exti.EXTI_Line = EXTI_Line12;
	EXTI_Init(&exti);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//nvic.NVIC_IRQChannel = EXTI1_IRQn;
	//NVIC_Init(&nvic);

//	nvic.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_Init(&nvic);

}

void bemf_hd_reset(void)
{
	bemf_hd_data.source = bemf_hd_phase_none;
	bemf_hd_data.trigger = false;
}

void bemf_hd_phase_u(void)
{
	bemf_line_state = GPIOB->IDR;
	OFF(DP_EXT_SDA);
	//DEBUG("UPhase Detect\n");

	/*
	 * Take care of oscillating interrupt triggers and filter them out
	 * also detect the source of the interrupt.
	 */
	if ((bemf_hd_data.source != bemf_hd_phase_u_rising) &&
		(bemf_hd_data.source != bemf_hd_phase_u_falling)) {
#ifndef BEMF__DEBUG
		comm_tim_update_capture_and_time();
#else
		DEBUG("Comm time update capture and time\n");
#endif
		if ((bemf_line_state & (1 << 10)) != 0) {
			bemf_hd_data.source = bemf_hd_phase_u_rising;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_RISING();
		}else{
			bemf_hd_data.source = bemf_hd_phase_u_falling;
			bemf_hd_data.trigger = true;
			BEMF_HD_LED_FALLING();
		}
	} else {
#ifndef BEMF__DEBUG
		comm_tim_update_next_prev();
#else
		DEBUG("Comm time update next prev\n");
#endif
	}

	EXTI_ClearITPendingBit(EXTI_Line10);
	ON(DP_EXT_SDA);
}

void bemf_hd_phase_v(void)
{
	bemf_line_state = GPIOB->IDR;
		OFF(DP_EXT_SDA);

		/*
		 * Take care of oscillating interrupt triggers and filter them out
		 * also detect the source of the interrupt.
		 */
		if ((bemf_hd_data.source != bemf_hd_phase_v_rising) &&
			(bemf_hd_data.source != bemf_hd_phase_v_falling)) {
	#ifndef BEMF__DEBUG
			comm_tim_update_capture_and_time();
	#else
			DEBUG("Comm time update capture and time\n");
	#endif
			if ((bemf_line_state & (1 << 11)) != 0) {
				bemf_hd_data.source = bemf_hd_phase_v_rising;
				bemf_hd_data.trigger = true;
				//DEBUG("VPhase Rise\n");
				BEMF_HD_LED_RISING();
			}else{
				bemf_hd_data.source = bemf_hd_phase_v_falling;
				bemf_hd_data.trigger = true;
				//DEBUG("VPhase Fall\n");
				BEMF_HD_LED_FALLING();
			}
		} else {
	#ifndef BEMF__DEBUG
			comm_tim_update_next_prev();
	#else
			DEBUG("Comm time update next prev\n");
	#endif
		}

		EXTI_ClearITPendingBit(EXTI_Line11);
		ON(DP_EXT_SDA);

}
/**
 * handler (phase W)
 */
void bemf_hd_phase_w(void)
{
	bemf_line_state = GPIOB->IDR;
		OFF(DP_EXT_SDA);

		/*
		 * Take care of oscillating interrupt triggers and filter them out
		 * also detect the source of the interrupt.
		 */
		if ((bemf_hd_data.source != bemf_hd_phase_w_rising) &&
			(bemf_hd_data.source != bemf_hd_phase_w_falling)) {
	#ifndef BEMF__DEBUG
			comm_tim_update_capture_and_time();
	#else
			DEBUG("Comm time update capture and time\n");
	#endif
			if ((bemf_line_state & (1 << 12)) != 0) {
				bemf_hd_data.source = bemf_hd_phase_w_rising;
				bemf_hd_data.trigger = true;
				//DEBUG("WPhase Rise\n");
				BEMF_HD_LED_RISING();
			}else{
				bemf_hd_data.source = bemf_hd_phase_w_falling;
				bemf_hd_data.trigger = true;
				BEMF_HD_LED_FALLING();
				//DEBUG("WPhase Falling\n");
			}
		} else {
	#ifndef BEMF__DEBUG
			comm_tim_update_next_prev();
	#else
			DEBUG("Comm time update next prev\n");
	#endif
		}

		EXTI_ClearITPendingBit(EXTI_Line12);
		ON(DP_EXT_SDA);

}

/**
 * External interrupt bank 10:15 handler
 */
void exti15_10_irq_handler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		bemf_hd_phase_u();
	}else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		bemf_hd_phase_v();
	}else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		bemf_hd_phase_w();
	}else{
		DEBUG("Stray interrupt on EXTI15_10 Bank\n")
	}
}
