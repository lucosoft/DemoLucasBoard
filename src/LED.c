/*
 * Led.c
 *
 *  Created on: 28/12/2015
 *      Author: Lucas
 */

#include "Led.h"
#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

//#define 	LEDTIC 		500 /* Tiempo de parpadeo*/


//int ledtic = LEDTIC;
int ledtic;
int LED = 0 ;



/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	Inicializacion de LED2
 * @return	Nothing
 */
void LED_Init(void){
	/*Inicializar el puerto 22 como salida*/

	Chip_GPIO_SetDir(LPC_GPIO,0,22,1);
}

/**
 * @brief	Parpadeo de LED2 cada LEDTIC milisegundos
 * @return	Nothing
 */
void  LED_Parpadeo(void){
	//Hago parpadear el led
	if(!ledtic)
	{
		ledtic = LEDTIC;
		if (!LED)
		{
			/*PRENDER_LED Poner un "1" en el puerto 22*/
			Chip_GPIO_SetPinOutHigh(LPC_GPIO,0,22);
			LED=1;
		}
		else
		{
			/*APAGAR_LED Poner un "0" en el puerto 22*/
			Chip_GPIO_SetPinOutLow(LPC_GPIO,0,22);
			LED=0;
		}
	}
}

void LED_On (void)
{

	/*PRENDER_LED Poner un "1" en el puerto 22*/
	Chip_GPIO_SetPinOutHigh(LPC_GPIO,0,22);

}

void LED_Off (void)
{

	/*APAGAR_LED Poner un "0" en el puerto 22*/
	Chip_GPIO_SetPinOutLow(LPC_GPIO,0,22);
}
