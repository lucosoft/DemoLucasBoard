/*
 * UART.h
 *
 *  Created on: 8/1/2016
 *      Author: Lucas
 */

#ifndef UART_H_
#define UART_H_

#include "Board.h"
#include "string.h"


/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 32	/* Receive */

//#define LPC_USART	LPC_UART3			/* Receive */
//#define LPC_USART	LPC_UART2			/* Receive */
#define LPC_USART	LPC_UART1			/* Receive */
//#define UART_SELECTION 	LPC_UART3
#define IRQ_USART 		UART1_IRQn
#define HANDLER_USART 	UART1_IRQHandler


void UART_Init(void);
void UART_DeInit(void);

/**
 * @brief	Envia los datos de String por la UART seguido de un fin de carro y nueva linea "\r\n"
 * @param	*String		: Puntero de la cadena de caracteres que se desea enviar
 * @return	None
 */
void UART_println(char* String);

/**
 * @brief	Envia los datos de String por la UART
 * @param	*String		: Puntero de la cadena de caracteres que se desea enviar
 * @return	None
 */
void UART_print(char* String);

/**
 * @brief	Lee el buffer circular de la UART hasta vaciarlo, o hasta maxItems
 * @param	String		: Puntero a donde se desea guardar los datos leidos
 * @param	maxItems	: Maxima cantidad de Bytes que van a ser leidos
 * @return	Cantidad de bytes leidos
 */
int UART_Read(void * String, int maxItems);

/**
 * @brief	Limpia el buffer circular de la UART
 * @return	None
 */
void UART_Flush();

#endif /* UART_H_ */
