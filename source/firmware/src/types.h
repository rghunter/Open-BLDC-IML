/*
 * Open-BLDC - Open BrushLess DC Motor Controller
 * Copyright (C) 2009-2010 by Piotr Esden-Tempski <piotr@esden.net>
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

#ifndef __TYPES_H
#define __TYPES_H

#include <cmsis/stm32.h>
#include "driver/usart.h"
#include <stdio.h>
#include "gprot.h"
#define false 0
#define true 1

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define NULL ((void *)0)

//#define DEBUG_ON

#ifdef DEBUG_ON
#define DEBUG(STR) gpc_send_string(STR,strlen(STR));
#define ERRORF(STR, ARGS...) \
		char buffer[50]; \
		sprintf(buffer, STR, ARGS); \
		gpc_send_string(buffer, 50); \
		while(gpc_not_empty());
#define ERROR(STR) \
		gpc_send_string(STR, strlen(STR)); \
		while(gpc_not_empty());
#else
#define DEBUG(STR)
#define ERRORF(STR, ARGS...)
#define ERROR(STR)
#endif


#endif /* __TYPES_H */
