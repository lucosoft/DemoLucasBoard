/*
 * LCD.c
 *
 *  Created on: 28/12/2015
 *      Author: Lucas
 */

#include "board.h"
#include "lcd.h"

uint32_t usTicks;
int Lcd_CurrentLine = 0;

/* Inicializa al display mediante instrucciones en modo 4-bit */
void LCD_Init(void) {

	RS_PIN_INIT(LCD_RS_PORT, LCD_RS_BIT);
	EN_PIN_INIT(LCD_EN_PORT, LCD_EN_BIT);
	D4_PIN_INIT(LCD_D4_PORT, LCD_D4_BIT);
	D5_PIN_INIT(LCD_D5_PORT, LCD_D5_BIT);
	D6_PIN_INIT(LCD_D6_PORT, LCD_D6_BIT);
	D7_PIN_INIT(LCD_D7_PORT, LCD_D7_BIT);

	/* Se inicializa al Timer 1 */
	LCD_Tim1Init();

//	LCD_PORT->CLR = LCD_RS;
//	LCD_PORT->CLR = LCD_EN;
	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_RS_PORT, LCD_RS_BIT, 0);
	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_EN_PORT, LCD_EN_BIT, 0);


//	/* Se envían tres nibbles 0x03 con los delays correspondientes */
//	LCD_usDelay(16000);
//	LCD_SendNibble(0x03);
//	LCD_usDelay(5000);
//	LCD_SendNibble(0x03);
//	LCD_usDelay(200);
//	LCD_SendNibble(0x03);

	/* Se envían tres nibbles 0x03 con los delays correspondientes */
	LCD_usDelay(16000);
	LCD_SendNibble(0x03);
	LCD_usDelay(5000);
	LCD_SendNibble(0x02);
	LCD_usDelay(200);
	LCD_SendNibble(0x02);

	/* Se cambia a modo 4-bit */
	LCD_SendNibble(0x02);
	LCD_usDelay(1000);

	/* Se envían las instrucciones requeridas */
	LCD_SendInstruction(LCD_FUNCTION_SET_4BIT);
	LCD_SendInstruction(LCD_DISPLAY_OFF);
	LCD_SendInstruction(LCD_DISPLAY_CLEAR);
	LCD_SendInstruction(LCD_ENTRY_MODE_SET);
	LCD_SendInstruction(LCD_DISPLAY_ON);

	LCD_Clrscr();
	LCD_Print("INICIALIZANDO...");
}

/* Envía un nibble al display */
void LCD_SendNibble(uint8_t theNibble) {
	/* Se coloca cada bit del nibble en el pin correspondiente */
	if (theNibble & 0x01)
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D4_PORT, LCD_D4_BIT, 1));
	else
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D4_PORT, LCD_D4_BIT, 0));

	if (theNibble & 0x02)
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D5_PORT, LCD_D5_BIT, 1));
	else
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D5_PORT, LCD_D5_BIT, 0));

	if (theNibble & 0x04)
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D6_PORT, LCD_D6_BIT, 1));
	else
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D6_PORT, LCD_D6_BIT, 0));

	if (theNibble & 0x08)
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D7_PORT, LCD_D7_BIT, 1));
	else
		(Chip_GPIO_WritePortBit(LPC_GPIO, LCD_D7_PORT, LCD_D7_BIT, 0));

	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_EN_PORT, LCD_EN_BIT, 1);
	LCD_usDelay(5);
	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_EN_PORT, LCD_EN_BIT, 0);
	LCD_usDelay(5);
}

/* Envía un caracter al display */
void LCD_SendChar(char theChar) {
	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_RS_PORT, LCD_RS_BIT, 1);

	if (theChar == '\n') {
		Lcd_CurrentLine++;
		if (Lcd_CurrentLine == 4)
			Lcd_CurrentLine = 0;
		LCD_GoToxy(0, Lcd_CurrentLine);
	} else if (theChar == '\b')
		LCD_Shift(CURSOR, LEFT);
	else if (theChar == '\f')
		LCD_Clrscr();
	else {
		LCD_SendByte(theChar);
		LCD_usDelay(50);

	}

}

/* Envía una instrucción al display */
void LCD_SendInstruction(uint8_t theInstruction) {
	Chip_GPIO_WritePortBit(LPC_GPIO, LCD_RS_PORT, LCD_RS_BIT, 0);

	LCD_SendByte(theInstruction);

	if (theInstruction == LCD_DISPLAY_CLEAR)
		LCD_usDelay(2000);
	else
		LCD_usDelay(50);
}

/* Envía un byte al display */
void LCD_SendByte(uint8_t theByte) {
	/* Primero se envía la parte alta */
	LCD_SendNibble(theByte >> 4);

	/* Luego se envía la parte baja */
	LCD_SendNibble(theByte);
}

/* Posiciona el cursor en la columna x - fila y */
void LCD_GoToxy(uint8_t x, uint8_t y) {
	if (y == 0)
		LCD_SendInstruction(LCD_DDRAM_ADDRESS + LCD_START_LINE1 + x);
	else if (y == 1)
		LCD_SendInstruction(LCD_DDRAM_ADDRESS + LCD_START_LINE2 + x);
	else if (y == 2)
		LCD_SendInstruction(LCD_DDRAM_ADDRESS + LCD_START_LINE3 + x);
	else if (y == 3)
		LCD_SendInstruction(LCD_DDRAM_ADDRESS + LCD_START_LINE4 + x);
}

/* Envía un string al display */
void LCD_Print(char *p) {
	while (*p != 0) {
		LCD_SendChar(*p);
		p++;
	}
}

void LCD_create_char(uint8_t char_num, uint8_t *character) {
	int i = 0;
	if (char_num < 8) {
		LCD_SendInstruction(LCD_CGRAM_ADDRESS | (char_num * 8));
		for (i = 0; i < 8; i++)
			LCD_SendChar(character[i]);
	}
	LCD_Home();
}

void LCD_Shift(uint8_t SC, uint8_t LR)	//SC 0-C / 1-S   //LR 0-L / 1-R
{
	LCD_SendInstruction((1 << 4) | (SC << 3) | (LR << 2));
}

void LCD_Home() {
	Lcd_CurrentLine = 0;
	LCD_GoToxy(0, 0);
}

void LCD_Clrscr() {
	Lcd_CurrentLine = 0;
	LCD_SendInstruction(LCD_DISPLAY_CLEAR);
}


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))


/* Delay us */
void LCD_usDelay(uint32_t usec) {
	Chip_TIMER_SetMatch(LPC_TIMER16_1, 0, usec);
	Chip_TIMER_Reset(LPC_TIMER16_1);
	Chip_TIMER_Enable(LPC_TIMER16_1);

	while (!usTicks)
		__WFI();
	usTicks = 0;

	Chip_TIMER_Disable(LPC_TIMER16_1);

}

#elif defined(BOARD_NXP_LPCXPRESSO_1769)

/* Delay us */
void LCD_usDelay(uint32_t usec)
{
	Chip_TIMER_SetMatch(LPC_TIMER1, 0, usec);
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_Enable(LPC_TIMER1);

	while (!usTicks) __WFI();
	usTicks = 0;

	Chip_TIMER_Disable(LPC_TIMER1);

}

#endif


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))

/* Inicializa al Timer 1 utilizado para realizar los delays de us */
void LCD_Tim1Init(void) {
	Chip_TIMER_Init(LPC_TIMER16_1);
	Chip_TIMER_PrescaleSet(LPC_TIMER16_1,
			Chip_Clock_GetSystemClockRate() / 1000000 - 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER16_1, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_1, 0);
	Chip_TIMER_StopOnMatchDisable(LPC_TIMER16_1, 0);

	Chip_TIMER_SetMatch(LPC_TIMER16_1, 0, 1000);

	NVIC_ClearPendingIRQ(TIMER_16_1_IRQn);
	NVIC_EnableIRQ(TIMER_16_1_IRQn);
}

#elif defined(BOARD_NXP_LPCXPRESSO_1769)

/* Inicializa al Timer 1 utilizado para realizar los delays de us */
void LCD_Tim1Init(void)
{
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_PrescaleSet(LPC_TIMER1,
	Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER1) / 1000000 - 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 0);
	Chip_TIMER_StopOnMatchDisable(LPC_TIMER1, 0);

	Chip_TIMER_SetMatch(LPC_TIMER1, 0, 1000);

	NVIC_EnableIRQ(TIMER1_IRQn);
}

#endif

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))

/* Desactiva al Timer 1 */
void LCD_Tim1DeInit(void) {
	Chip_TIMER_Disable(LPC_TIMER16_1);
	NVIC_DisableIRQ(TIMER_16_1_IRQn);
}

#elif defined(BOARD_NXP_LPCXPRESSO_1769)

/* Desactiva al Timer 1 */
void LCD_Tim1DeInit(void)
{
	Chip_TIMER_Disable(LPC_TIMER1);
	NVIC_DisableIRQ(TIMER1_IRQn);
}

#endif


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))

/* Handler de interrupción del Timer 1 */
void TIMER16_1_IRQHandler(void) {
	if (Chip_TIMER_MatchPending(LPC_TIMER16_1, 0)) {
		usTicks++;
	}
	Chip_TIMER_ClearMatch(LPC_TIMER16_1, 0);
}

#elif defined(BOARD_NXP_LPCXPRESSO_1769)

/* Handler de interrupción del Timer 1 */
void TIMER1_IRQHandler(void)
{
if (Chip_TIMER_MatchPending(LPC_TIMER1, 0)) {
			usTicks++;


	}
	Chip_TIMER_ClearMatch(LPC_TIMER1, 0);
}

#endif

void RS_PIN_INIT(int port, int bit) {

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))

	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);			// 1-OUTPUT
}

void EN_PIN_INIT(int port, int bit) {

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);
}

void D4_PIN_INIT(int port, int bit) {


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);
}

void D5_PIN_INIT(int port, int bit) {

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);
}

void D6_PIN_INIT(int port, int bit) {


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);
}

void D7_PIN_INIT(int port, int bit) {

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	switch (port) {
	case 0: {
		switch (bit) {
		case 1: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, IOCON_FUNC0);
			break;
		}
		case 2: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, IOCON_FUNC0);
			break;
		}
		case 3: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);
			break;
		}
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_5, IOCON_FUNC0);
			break;
		}
		case 6: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);
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
		case 4: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_4, IOCON_FUNC0);
			break;
		}
		case 5: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_5, IOCON_FUNC0);
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
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_8, IOCON_FUNC0);
			break;
		}
		case 9: {
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_9, IOCON_FUNC0);
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

	Chip_GPIO_SetPinDIR(LPC_GPIO, port, bit, 1);
}
