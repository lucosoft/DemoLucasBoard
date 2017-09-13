/*
 * gpio.c
 *
 *  Created on: 5/2/2016
 *      Author: Cacciola11
 */

#include "GPIO.h"
#include "chip.h"
#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

void GPIO_Init ()
{
	PIN_INIT(GPIO_IN1_PORT,GPIO_IN1_BIT,INPUT);
	PIN_INIT(GPIO_IN2_PORT,GPIO_IN2_BIT,INPUT);
	PIN_INIT(GPIO_IN3_PORT,GPIO_IN3_BIT,INPUT);
	PIN_INIT(GPIO_IN4_PORT,GPIO_IN4_BIT,INPUT);

	PIN_INIT(GPIO_OUT1_PORT,GPIO_OUT1_BIT,OUTPUT);
	PIN_INIT(GPIO_OUT2_PORT,GPIO_OUT2_BIT,OUTPUT);
	PIN_INIT(GPIO_OUT3_PORT,GPIO_OUT3_BIT,OUTPUT);

	GPIO_Out (0);
}

int GPIO_In ()
{
	return ((GPIO_In4()<<3) | (GPIO_In3()<<2) | (GPIO_In2()<<1) | GPIO_In1());
}

bool GPIO_In1 ()
{
	if (!Chip_GPIO_GetPinState(LPC_GPIO, GPIO_IN1_PORT, GPIO_IN1_BIT))
		return 1;
	else
		return 0;
}

bool GPIO_In2 ()
{
	if (!Chip_GPIO_GetPinState(LPC_GPIO, GPIO_IN2_PORT, GPIO_IN2_BIT))
		return 1;
	else
		return 0;
}

bool GPIO_In3 ()
{
	if (!Chip_GPIO_GetPinState(LPC_GPIO, GPIO_IN3_PORT, GPIO_IN3_BIT))
		return 1;
	else
		return 0;
}

bool GPIO_In4 ()
{
	if (!Chip_GPIO_GetPinState(LPC_GPIO, GPIO_IN4_PORT, GPIO_IN4_BIT))
		return 1;
	else
		return 0;
}

void GPIO_Out (int Out)
{
	if (Out > 7)
		Out = 7;

	GPIO_Out1(Out & 0x1);
	GPIO_Out2((Out >> 1) & 0x1);
	GPIO_Out3((Out >> 2) & 0x1);
}

void GPIO_Out1 (bool Out)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_OUT1_PORT, GPIO_OUT1_BIT, Out);
}

void GPIO_Out2 (bool Out)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_OUT2_PORT, GPIO_OUT2_BIT, Out);
}

void GPIO_Out3 (bool Out)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_OUT3_PORT, GPIO_OUT3_BIT, Out);
}

void PIN_INIT(int port, int bit, bool dir) {

#ifdef BOARD_NXP_XPRESSO_11C24

	switch (port) {
	case 0: {
		switch (bit) {
		case 0:			//Reset
			break;
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 7: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_7, IOCON_FUNC0);
			break;
		}
		case 8: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC0);
			break;
		}
		case 10:		//SWCLK 1115
			break;
		case 11: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_11, IOCON_FUNC1);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 1: {
		switch (bit) {
		case 0: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_0, IOCON_FUNC1);
			break;
		}
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1, IOCON_FUNC1);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2, IOCON_FUNC1);
			break;
		}
		case 3:		//SWDIO 1115
			break;
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, IOCON_FUNC0);
			break;
		}
		case 7: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, IOCON_FUNC0);
			break;
		}
		case 8: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0 | IOCON_MODE_PULLUP);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
			break;
		}
		case 10: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_10, IOCON_FUNC0);
		}
			break;
		case 11: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_11, IOCON_FUNC0);
		}
			break;
		default:
			break;
		}
		break;
	}
	case 2: {
		switch (bit) {
		case 0: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_0, IOCON_FUNC0);
			break;
		}
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_6, IOCON_FUNC0);
			break;
		}
		case 7: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_7, IOCON_FUNC0);
			break;
		}
		case 8: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_9, IOCON_FUNC0);
			break;
		}
		case 10: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_10, IOCON_FUNC0);
			break;
		}
		case 11: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_11, IOCON_FUNC0);
			break;
		}
		default:
			break;
		}
		break;
	}
	case 3: {
		switch (bit) {
		case 0: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_0, IOCON_FUNC0);
			break;
		}
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO3_5, IOCON_FUNC0);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

#endif

//	Chip_IOCON_PinMux(LPC_GPIO, port, bit,IOCON_MODE_PULLUP, IOCON_FUNC0);
	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, dir);
}

