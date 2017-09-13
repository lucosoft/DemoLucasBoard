/*
 * Pulsador.c
 *
 *  Created on: 28/12/2015
 *      Author: Lucas
 */

#include "Pulsador.h"
#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* defino una variable para la lectura del pulsador*/
int pulsador;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	Leer el estado actual del Pulsador en P0.18 (J6-11)
 * @return	Nothing
 */
void  Pulsador_Lectura(void){
	/*Poner un breakpoint acá y ver el valor del pulsador*/
	pulsador=(Chip_GPIO_GetPinState(LPC_GPIO,0,18));
}
