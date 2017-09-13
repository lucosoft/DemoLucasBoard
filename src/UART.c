/*
 * UART.c
 *
 *  Created on: 8/1/2016
 *      Author: Lucas
 */


#include "UART.h"


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

///* Transmit and receive ring buffer sizes */
//#define UART_SRB_SIZE 128	/* Send */
//#define UART_RRB_SIZE 32	/* Receive */
//
////#define LPC_USART	LPC_UART3			/* Receive */
//#define LPC_USART	LPC_UART2			/* Receive */



/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

RINGBUFF_T txring, rxring;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void Init_UART_PinMux(void)
{
//#if (defined(BOARD_NXP_XPRESSO_11U14) || defined(BOARD_NGX_BLUEBOARD_11U24))
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_18 used for RXD */
//	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 19, IOCON_FUNC1 | IOCON_MODE_INACT);	/* PIO0_19 used for TXD */
//#elif (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
//	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
//	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */
//#else
//#error "No Pin muxing defined for UART operation"
//#endif
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */


#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
void UART_IRQHandler(void)
#elif defined(BOARD_NXP_LPCXPRESSO_1769)
void UART1_IRQHandler(void)
#endif
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

void UART_Init (void)
{
	Init_UART_PinMux();
	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
//	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_SetBaud(LPC_USART, 9600);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));


//	/* inicializo los pines TXD2 y RXD2*/
//	Chip_IOCON_PinMux(LPC_IOCON_BASE, 0, 10, IOCON_MODE_INACT, IOCON_FUNC1);
//	Chip_IOCON_PinMux(LPC_IOCON_BASE, 0, 11, IOCON_MODE_INACT, IOCON_FUNC1);
	/* inicializo los pines TXD1 y RXD1*/
	Chip_IOCON_PinMux(LPC_IOCON_BASE, 0, 15, IOCON_MODE_INACT, IOCON_FUNC1);
	Chip_IOCON_PinMux(LPC_IOCON_BASE, 0, 16, IOCON_MODE_INACT, IOCON_FUNC1);

	Chip_UART_TXEnable(LPC_USART);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);
#elif defined(BOARD_NXP_LPCXPRESSO_1769)

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(IRQ_USART, 1);
	NVIC_EnableIRQ(IRQ_USART);

	UART_print("Inicializado");

#endif
}

void UART_DeInit (void)
{

#if (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	/* DeInitialize UART0 peripheral */
	NVIC_DisableIRQ(UART0_IRQn);
#elif defined(BOARD_NXP_LPCXPRESSO_1769)
	/* DeInitialize UART0 peripheral */
	NVIC_DisableIRQ(IRQ_USART);
#endif

	Chip_UART_DeInit(LPC_USART);
}

/* Imprime String segido de \r\n */
void UART_println(char *String) {
	char Buff_aux[128] = "";

	if (String != NULL) {
		strncpy(Buff_aux, String, 128);
		strcat(Buff_aux, "\r\n");
		Chip_UART_SendRB(LPC_USART, &txring, &Buff_aux, strlen(Buff_aux));
	}
}

/* Imprime String segido de \r\n */
void UART_print(char *String) {
	if (String != NULL)
		Chip_UART_SendRB(LPC_USART, &txring, String, strlen(String));
}

/* Lee el buffer circular y lo guarda en String. deuelve la cantidad de Bytes leidos */
int UART_Read(void *String, int maxItems) {
	int bytes = 0;
	bytes = Chip_UART_ReadRB(LPC_USART, &rxring, (uint8_t *) String, maxItems);
	return (bytes);
}

/* Limpia el buffer circular */
void UART_Flush() {
	char Buff_aux[128] = "";
	Chip_UART_ReadRB(LPC_USART, &rxring, &Buff_aux, 128);
}
