/*
 ===============================================================================
 Nombre      : lcd.h
 Autor	     : Cátedra Técnicas Digitales II - UTN FRH
 Versión     : 2.0
 Fecha 		 : Junio 2014
 Descripción : Contiene la definición de constantes y prototipos de funciones
 ===============================================================================
 */
#ifndef LCD_H_
#define LCD_H_

/* Puerto y pines a los que se conecta el display */
// Reserved: PIO0_0, PIO0_10, PIO1_3
//#define LCD_RS_PORT		0		//Placa probadora
//#define LCD_RS_BIT 		4
//#define LCD_EN_PORT		0
//#define LCD_EN_BIT 		5


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))

#define LCD_RS_PORT		1	  //Placa con cambios
#define LCD_RS_BIT 		8
#define LCD_EN_PORT		1
#define LCD_EN_BIT 		9

#define LCD_D4_PORT		0
#define LCD_D4_BIT 		6
#define LCD_D5_PORT		0
#define LCD_D5_BIT 		7
#define LCD_D6_PORT		0
#define LCD_D6_BIT 		8
#define LCD_D7_PORT		0
#define LCD_D7_BIT 		9

#elif defined(BOARD_NXP_LPCXPRESSO_1769)

#define LCD_RS_PORT		2
#define LCD_RS_BIT 		0
#define LCD_EN_PORT		2
#define LCD_EN_BIT 		1

#define LCD_D4_PORT		2
#define LCD_D4_BIT 		5
#define LCD_D5_PORT		2
#define LCD_D5_BIT 		6
#define LCD_D6_PORT		2
#define LCD_D6_BIT 		7
#define LCD_D7_PORT		2
#define LCD_D7_BIT 		8

//	/* Puerto y pines a los que se conecta el display */
//	#define LCD_PORT	LPC_GPIO2
//	#define LCD_D4 		1<<5
//	#define LCD_D5 		1<<6
//	#define LCD_D6 		1<<7
//	#define LCD_D7 		1<<8
//	#define LCD_RS 		1<<0
//	#define LCD_EN 		1<<1

#endif

/* Códigos hexadecimales de las instrucciones utilizadas */
#define LCD_FUNCTION_SET_4BIT 	0x28 // 0b00101000 -> DL=0, N=1, F=0
#define LCD_DISPLAY_OFF       	0x08 // 0b00001000 -> D=0, C=0, B=0
#define LCD_DISPLAY_CLEAR     	0x01 // 0b00000001
#define LCD_ENTRY_MODE_SET    	0x06 // 0b00000110 -> I/D=1, S=0
#define LCD_DISPLAY_ON        	0x0C // 0b00001100 -> D=1 , C=0, B=0

#define LCD_DDRAM_ADDRESS	  	0x80 // 0b10000000
#define LCD_START_LINE1 	  	0x00
#define LCD_START_LINE2 	  	0x40
#define LCD_START_LINE3 	  	0x10
#define LCD_START_LINE4 	  	0x50

#define LCD_CGRAM_ADDRESS	  	0x40

#define LCD_CURSOR_LEFT 	  	0x10 // 0b00010000 -> S/C=0, R/L=0
#define LCD_CURSOR_RIGHT	  	0x14 // 0b00010100 -> S/C=0, R/L=1
#define LCD_DISPLAY_LEFT	  	0x18 // 0b00011000 -> S/C=1, R/L=0
#define LCD_DISPLAY_RIGHT		0x1C // 0b00011100 -> S/C=1, R/L=1



// Para LCD_Shift
#define CURSOR	0
#define SCREEN	1
#define LEFT	0
#define RIGHT	1


/* Prototipos de funciones */
void LCD_Init(void);
void LCD_SendNibble(uint8_t theNibble);
void LCD_SendChar(char theChar);
void LCD_SendInstruction(uint8_t theInstruction);
void LCD_SendByte(uint8_t theByte);
void LCD_GoToxy(uint8_t x, uint8_t y);
void LCD_Print(char *p);
void LCD_create_char(uint8_t, uint8_t*);
void LCD_Shift(const uint8_t, const uint8_t); //SC 0-C / 1-S   //LR 0-L / 1-R
void LCD_Home();
void LCD_Clrscr();

// Funciones necesarias para los delays
void LCD_usDelay(uint32_t usec);
void LCD_Tim1Init(void);
void LCD_Tim1DeInit(void);

// Inicializacion de los pines de conexion
void RS_PIN_INIT(int, int);
void EN_PIN_INIT(int, int);
void D4_PIN_INIT(int, int);
void D5_PIN_INIT(int, int);
void D6_PIN_INIT(int, int);
void D7_PIN_INIT(int, int);

#endif /* LCD_H_ */
