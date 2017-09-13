/*
 * RTC.c
 *
 *  Created on: 4/1/2016
 *      Author: Lucas
 */

#include "RTC.h"
//#include "lpc_types.h"
#include "chip.h"
#include <stdlib.h>
#include "LCD.h"



/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

// rtc
static volatile bool fIntervalReached = 0;
static volatile bool fAlarmTimeMatched = 0;
static volatile bool On0 = false;
static volatile bool On1 = false;

//// rtc
//fIntervalReached  = 0;
//fAlarmTimeMatched = 0;
//On0 = On1 = false;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

// rtc
RTC_TIME_T FullTime;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

// rtc
/* Gets and shows the current time and date */
//static void showTime(RTC_TIME_T *pTime)
static void showTime(RTC_TIME_T *pTime)
{
//	DEBUGOUT("Time: %.2d:%.2d:%.2d %.2d/%.2d/%.4d\r\n",
//			 pTime->time[RTC_TIMETYPE_HOUR],
//			 pTime->time[RTC_TIMETYPE_MINUTE],
//			 pTime->time[RTC_TIMETYPE_SECOND],
//			 pTime->time[RTC_TIMETYPE_MONTH],
//			 pTime->time[RTC_TIMETYPE_DAYOFMONTH],
//			 pTime->time[RTC_TIMETYPE_YEAR]);

//	setDisplay(&pTime);

	char data_bf[2];		// variable usada para convertir uint32_t a string
	char data_bf2[17]={};		// variable usada para convertir uint32_t a string
	char data_bf3[17]={};		// variable usada para convertir uint32_t a string

	uint32_t time_aux;

	asm("nop");
	asm("nop");
	asm("nop");

//	LCD_GoToxy(0,0);			// muestro en display
//	LCD_Print("Fecha:05/10/2012");
//
//	LCD_GoToxy(0,1);			// muestro en display
//	LCD_Print("Hora:14:00:00");

	strcat(data_bf2,"Fecha:");		// armo string de fecha actual

	itoa(pTime->time[RTC_TIMETYPE_DAYOFMONTH], data_bf3, 10);	// dia del mes
	if(pTime->time[RTC_TIMETYPE_DAYOFMONTH]<10)
		strcat(data_bf2, "0");
	strcat(data_bf2, data_bf3);
	strcat(data_bf2,"/");

	itoa(pTime->time[RTC_TIMETYPE_MONTH], data_bf3, 10);	// mes
	if(pTime->time[RTC_TIMETYPE_MONTH]<10)
		strcat(data_bf2, "0");
	strcat(data_bf2, data_bf3);
	strcat(data_bf2,"/");

	itoa(pTime->time[RTC_TIMETYPE_YEAR], data_bf3, 10);	// año
	strcat(data_bf2, data_bf3);

	LCD_GoToxy(0,0);			// muestro en display
	LCD_Print(data_bf2);
//	LCD_Print("Fecha:05/10/2012");

	data_bf2[0] = '\0';



	strcat(data_bf2,"Hora:");		// armo string de hora actual

	itoa(pTime->time[RTC_TIMETYPE_HOUR], data_bf3, 10);		// hora
	if(pTime->time[RTC_TIMETYPE_HOUR]<10)
		strcat(data_bf2, "0");
	strcat(data_bf2, data_bf3);
	strcat(data_bf2,":");

	itoa(pTime->time[RTC_TIMETYPE_MINUTE], data_bf3, 10);	// minutos
	if(pTime->time[RTC_TIMETYPE_MINUTE]<10)
		strcat(data_bf2, "0");
	strcat(data_bf2, data_bf3);
	strcat(data_bf2,":");

	itoa(pTime->time[RTC_TIMETYPE_SECOND], data_bf3, 10);	// segundos
	if(pTime->time[RTC_TIMETYPE_SECOND]<10)
		strcat(data_bf2, "0");
	strcat(data_bf2, data_bf3);

	LCD_GoToxy(0,1);			// muestro en display
	LCD_Print(data_bf2);
//	LCD_Print("Hora:14:00:00");


//	if(strlen(data_bf2)<17)

//	LCD_GoToxy(0,0);
//	LCD_Print("Fecha:");
//	LCD_GoToxy(6,0);
//	time_aux = pTime->time[RTC_TIMETYPE_DAYOFMONTH];
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_DAYOFMONTH],data_bf));
//	LCD_GoToxy(8,0);
//	LCD_Print("/");
//	LCD_GoToxy(9,0);
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_MONTH],data_bf));
//	LCD_GoToxy(11,0);
//	LCD_Print("/");
//	LCD_GoToxy(12,0);
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_YEAR],data_bf));
//
//
//	LCD_GoToxy(0,1);
//	LCD_Print("Hora:");
//	LCD_GoToxy(5,1);
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_HOUR],data_bf));
//	LCD_GoToxy(7,1);
//	LCD_Print(":");
//	LCD_GoToxy(8,1);
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_MINUTE],data_bf));
//	LCD_GoToxy(10,1);
//	LCD_Print(":");
//	LCD_GoToxy(11,1);
//	LCD_Print(Byte_2_LCD(pTime->time[RTC_TIMETYPE_SECOND],data_bf));
//	LCD_GoToxy(14,1);

}

/*****************************************************************************
 * Public functions
 ****************************************************************************/



void RTC_Init(void)
{
	Chip_RTC_Init(LPC_RTC);

	/* Set current time for RTC 2:00:00PM, 2012-10-05 */
	FullTime.time[RTC_TIMETYPE_SECOND]  = 0;
	FullTime.time[RTC_TIMETYPE_MINUTE]  = 36;
	FullTime.time[RTC_TIMETYPE_HOUR]    = 10;
	FullTime.time[RTC_TIMETYPE_DAYOFMONTH]  = 5;
	FullTime.time[RTC_TIMETYPE_DAYOFWEEK]   = 3;
	FullTime.time[RTC_TIMETYPE_DAYOFYEAR]   = 5;
	FullTime.time[RTC_TIMETYPE_MONTH]   = 1;
	FullTime.time[RTC_TIMETYPE_YEAR]    = 2016;

	Chip_RTC_SetFullTime(LPC_RTC, &FullTime);

	/* Set ALARM time for 17 seconds from time */
	FullTime.time[RTC_TIMETYPE_SECOND]  = 17;
	Chip_RTC_SetFullAlarmTime(LPC_RTC, &FullTime);

	/* Set the RTC to generate an interrupt on each second */
	Chip_RTC_CntIncrIntConfig(LPC_RTC, RTC_AMR_CIIR_IMSEC, ENABLE);

	/* Enable matching for alarm for second, minute, hour fields only */
	Chip_RTC_AlarmIntConfig(LPC_RTC, RTC_AMR_CIIR_IMSEC | RTC_AMR_CIIR_IMMIN | RTC_AMR_CIIR_IMHOUR, ENABLE);

	/* Clear interrupt pending */
	Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE | RTC_INT_ALARM);

	/* Enable RTC interrupt in NVIC */
	NVIC_EnableIRQ((IRQn_Type) RTC_IRQn);

	/* Enable RTC (starts increase the tick counter and second counter register) */
	Chip_RTC_Enable(LPC_RTC, ENABLE);
}


void RTC_Funcion(void)
{
	if (fIntervalReached) {	/* Every 5s */
		fIntervalReached = 0;

		On1 = (bool) !On1;
#if !defined(CHIP_LPC175X_6X)
		Board_LED_Set(1, On1);
#endif
		/* read and display time */
		Chip_RTC_GetFullTime(LPC_RTC, &FullTime);
		showTime(&FullTime);
	}

	if (fAlarmTimeMatched) {
		fAlarmTimeMatched = false;

		/* announce event */
//		DEBUGSTR("ALARM triggered!\r\n");

		/* read and display time */
		Chip_RTC_GetFullTime(LPC_RTC, &FullTime);
		showTime(&FullTime);
	}
}

// rtc
/**
 * @brief	RTC interrupt handler
 * @return	Nothing
 */
void RTC_IRQHandler(void)
{
	uint32_t sec;

	/* Toggle heart beat LED for each second field change interrupt */
	if (Chip_RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE)) {
		/* Clear pending interrupt */
		Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
		On0 = (bool) !On0;
//		Board_LED_Set(0, On0);
	}

	/* display timestamp every 5 seconds in the background */
	sec = Chip_RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
//	if (!(sec % 5)) {
	if (!(sec % 1)) {
		fIntervalReached = true;	/* set flag for background */
	}

	/* Check for alarm match */
	if (Chip_RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM)) {
		/* Clear pending interrupt */
		Chip_RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
		fAlarmTimeMatched = true;	/* set alarm handler flag */
	}
}
