//#include "chip.h"
//#include "board.h"
//#include "string.h"
//
//#include "LED.h"
//#include "UART.h"
//#include "LCD.h"
//#include "RTC.h"
//
///*****************************************************************************
// * Private types/enumerations/variables
// ****************************************************************************/
//
///*****************************************************************************
// * Public types/enumerations/variables
// ****************************************************************************/
//// led
//extern int ledtic;
//
//// uart
//extern RINGBUFF_T txring, rxring;
//
//// delay
//#define 	MTICKS_MAX	1000 /* Tiempo de mticks*/
//
//// delay
//int mticks = MTICKS_MAX;
//
//
///*****************************************************************************
// * Private functions
// ****************************************************************************/
//
///*****************************************************************************
// * Public functions
// ****************************************************************************/
//
//// systick
///**
// * @brief	Inicializacion de SysTick para interrumpir cada 1ms
// * @return	Nothing
// */
//void SysTick_Init(void){
//	/*Inicializar el SysTick para que interrumpa cada 1 mseg*/
//	SysTick_Config(SystemCoreClock/1000);
//
//	}
//
///**
// * @brief	SysTick interrupt handler
// * @return	Nothing
// */
//void SysTick_Handler(void){
//	if(ledtic) ledtic--;
//	if(mticks) mticks--;
//}
//
///**
// * @brief	Main program body
// * @return	Always returns 1
// */
//int main(void)
//{
//
//
//	uint8_t key;
//	int bytes;
//
//	SystemCoreClockUpdate();
//
//	Board_Init();
//	LED_Init();
//	SysTick_Init();
//
//	LCD_Init();
//	LCD_SendInstruction(LCD_DISPLAY_CLEAR);
//	LCD_GoToxy(0,0);
//	LCD_Print("Inicializando...");
//
//	Board_UART_Init(UART_GSM);
//
//	UART_Init();
//
//	/* Poll the receive ring buffer for the ESC (ASCII 27) key */
//	key = 0;
//
//	RTC_Init();
//
//	while (key != 27) {
//		bytes = Chip_UART_ReadRB(UART_GSM, &rxring, &key, 1);
//		if (bytes > 0) {
//			/* Wrap value back around */
//			if (Chip_UART_SendRB(UART_GSM, &txring, (const uint8_t *) &key, 1) != 1) {
////				Board_LED_Toggle(0);/* Toggle LED if the TX FIFO is full */
//			}
//		}
//
//		LED_Parpadeo();
//
//		RTC_Funcion();
//	}
//
//	UART_DeInit();
//
//	return 1;
//}
//
//

/*
 ===============================================================================
 Name        : Main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */
//#include "Main.h"
/*****************************************************************************
 * 		Include
 ****************************************************************************/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "UART.h"
#include "Teclado.h"
#include "Led.h"

/*****************************************************************************
 * 		Define
 ****************************************************************************/

#define TICKRATE_HZ1 (1000)	/* 10 ticks per second */

// Tiempos referidos al systick
#define Seg1        TICKRATE_HZ1
#define HalfSeg     TICKRATE_HZ1/2

// Tiempo de titilado de Leds
#define LED_BLINK   TICKRATE_HZ1/4

// Definicion de pines
////#define LPC_IN1_PORT	00
////#define LPC_IN1_BIT		04
////
////#define LPC_IN2_PORT	00
////#define LPC_IN2_BIT		05
////
////#define LPC_IN3_PORT	00
////#define LPC_IN3_BIT		10
////
////#define LPC_IN4_PORT	00
////#define LPC_IN4_BIT		11
////
////#define LPC_OUT1_PORT	02
////#define LPC_OUT1_BIT	02
////
////#define LPC_OUT2_PORT	02
////#define LPC_OUT2_BIT	03
////
////#define LPC_OUT3_PORT	02
////#define LPC_OUT3_BIT	04

/*****************************************************************************
 * 		types/enumerations/variables
 ****************************************************************************/

//enum
typedef enum {
	APP_STATE_INIT = 0,
	APP_STATE_CONF,
	APP_STATE_GSM,
	APP_STATE_GPS,
	APP_STATE_DELAY,
	APP_STATE_OK,
	APP_STATE_ERROR,

} APP_STATES;

typedef enum {
	GSM_STATE_CPIN,
	GSM_STATE_CREG,
	GSM_STATE_CSQ,
	GSM_STATE_CONF,
	GSM_STATE_CGATT,
	GSM_STATE_CGSN,
	GSM_STATE_CIPSTART,
	GSM_STATE_CIPSEND,
	GSM_STATE_CIPRXGET,
	GSM_STATE_CIPCLOSE,
	GSM_STATE_OK,
	GSM_STATE_ERROR,
	GSM_STATE_ERROR_MSJ,

} GSM_STATES;

typedef enum {
	GPS_STATE_INIT,
	GPS_STATE_FIX,
	GPS_STATE_OK,
	GPS_STATE_ERROR,
	GPS_STATE_ERROR_MSJ,
	GPS_STATE_RESET,
	GPS_TEST_OVER_GSM,

} GPS_STATES;

typedef enum {
	CONFIG_STATE_ON, CONFIG_STATE_RESPONSE, CONFIG_STATE_GPS,

} CONFIG_STATES;

typedef enum {
	LCD_IDLE,
	LCD_INIT,
	LCD_CONFIG,
	LCD_SIM_ANT,
	LCD_RED,
	LCD_TCP,
	LCD_ECO,
	LCD_GSM_OK,
	LCD_GPS_INIT,
	LCD_GPS_SHOW,
	LCD_ERROR,

} LCD_STATES;

//struct
typedef struct {
	APP_STATES state;
	APP_STATES prev_state;
	GSM_STATES gsm_state;
	GSM_STATES gsm_prev_state;
	GPS_STATES gps_state;
	GPS_STATES gps_prev_state;
	CONFIG_STATES conf_state;
	LCD_STATES lcd_state;
	int stage;
	int delayms;
	char lcd_buffer[81];

} APP_DATA;


// uart test

//extern RINGBUFF_T txring;

APP_DATA appData;
uint32_t LapsedTime;						//Tiempo para el Delay
uint64_t LapsedTimeToError;					//Tiempo para considerar un error

extern char IMEI[16];
char N_GGA[128] = "", N_RMC[128] = "";		// Buffers de las tramas NMEA
char Error[5] = "";						// Para mostrar el estado de error
char sat_GGA[5] = "", sat_RMC[5] = "";// Datos necesarios extraidos de las tramas NMEA
char *Start = NULL, *End = NULL;// Punteros para extraer los datos de las tramas GPS

int ledBlink = 0, attempt = 0, Lcd_refresh = 0, GPS_onView = 0;
int Errortime = Seg1 * 10;

bool GPS_working = false, GSM_working = false;// flags indicando si se verifico el estado correctamente


// debug por UART3


int state_Ant = 0;
int conf_state_Ant = 0;
int gsm_state_Ant = 0;
int gps_state_Ant = 0;
int lcd_state_Ant = 0;
int stage_Ant = 0;


//extern int ledtic = LEDTIC;

/*****************************************************************************
 * 		Funciones ISR
 ****************************************************************************/

void SysTick_Handler(void) {

//	if(ledtic) ledtic--;

//	// control de tiempos
//	LapsedTime++;
//	LapsedTimeToError++;
//	Lcd_refresh++;
//
//	// Control de los Leds
//	ledBlink++;
//	if (ledBlink == LED_BLINK) {
//		ledBlink = 0;
//		if (GPS_working)
//			LED_GPS_ON();
//
//		if (GSM_working)
//			LED_GSM_ON();
//
//		if (!GSM_working
//				&& (appData.state == APP_STATE_GSM
//						|| appData.prev_state == APP_STATE_GSM))
//			LED_GSM_TOGGLE();
//		else if (!GSM_working
//				&& (appData.state != APP_STATE_GSM
//						|| appData.prev_state == APP_STATE_GSM))
//			LED_GSM_OFF();
//
//		if (!GPS_working
//				&& (appData.state == APP_STATE_GPS
//						|| appData.prev_state == APP_STATE_GPS))
//			LED_GPS_TOGGLE();
//		else if (!GPS_working
//				&& (appData.state != APP_STATE_GPS
//						|| appData.prev_state == APP_STATE_GPS))
//			LED_GPS_OFF();
//	}
//
//
//
//	if((state_Ant!=appData.state)||(conf_state_Ant!=appData.conf_state)||(gsm_state_Ant!=appData.gsm_state)||
//			(gps_state_Ant!=appData.gps_state)||(lcd_state_Ant!=appData.lcd_state)||(stage_Ant!=appData.stage))
//	{
//
//	state_Ant = appData.state;
//	conf_state_Ant = appData.conf_state;
//	gsm_state_Ant = appData.gsm_state;
//	gps_state_Ant = appData.gps_state;
//	lcd_state_Ant = appData.lcd_state;
//	stage_Ant = appData.stage;
//
//	DEBUGOUT("\r\n appData.state: %d \r\n", appData.state);
//	DEBUGOUT(" appData.conf_state: %d \r\n", appData.conf_state);
//	DEBUGOUT(" appData.gsm_state: %d \r\n", appData.gsm_state);
//	DEBUGOUT(" appData.gps_state: %d \r\n", appData.gps_state);
//	DEBUGOUT(" appData.lcd_state: %d \r\n", appData.lcd_state);
//	DEBUGOUT(" appData.stage: %d \r\n", appData.stage);
//
//	}
}

/*****************************************************************************
 * 		Funciones
 ****************************************************************************/

/**
 * @brief	Realiza un delay no bloqueante en ms
 * @param	ms		: Tiempo de delay
 * @return	None
 */
void delay(int ms) {
	LapsedTime = 0;
	appData.delayms = ms;
	appData.prev_state = appData.state;
	appData.state = APP_STATE_DELAY;
}

/**
 * @brief	Verifica que haya pasado el tiempo ms, y si es asi pasa a un estado de error
 * @param	ms			: Tiempo que tiene que transcurrir para que suceda un error
 * @return	None
 */
void timeToError(uint32_t ms) {
	if (LapsedTimeToError >= ms) {
		if (appData.state == APP_STATE_GSM) {

			if (appData.gsm_state >= GSM_STATE_CIPSTART)// (no probado) cierra la concexion TCP si se realizo y luego hubo un error
				UART_println("AT+CIPCLOSE=1");

			appData.gsm_prev_state = appData.gsm_state;
			appData.gsm_state = GSM_STATE_ERROR_MSJ;
		} else if (appData.state == APP_STATE_GPS) {
			if (appData.gps_state == GPS_STATE_FIX) {
				LapsedTimeToError = 0;
				appData.gps_state = GPS_TEST_OVER_GSM;
			} else {
				appData.gps_prev_state = appData.gps_state;
				appData.gps_state = GPS_STATE_ERROR_MSJ;
			}
		}
	}
}

///**
// * @brief	Enciende el modulo y el GPS
// * @return	None
// */
//void SIM908_ConfigOnSM()
//{
//	switch (appData.stage)
//	{
//		case 0:
//		{
//			appData.lcd_state = LCD_INIT;
//
//			gps_disable();			// Cambio a la UART GSM
//			gsm_enable();			//
//
//			Sim_OFF();				// PWR_ON en alto durante 1,5 Seg
//
//			appData.stage++;
//
//			delay(HalfSeg * 3);
//
//
//			break;
//		}
//		case 1:
//		{
//			Sim_ON();				// PWR_ON en bajo durante 2,5 Seg
//			appData.stage++;
//			delay(HalfSeg * 5);
//
//			break;
//		}
//		case 2:
//		{
//			UART_println("AT");
//			LapsedTime = 0;
//			appData.stage++;
////			appData.stage = 3;
//			//			appData.stage = 4;
//
//			break;
//		}
//		case 3:
//		{
//		//		if (AT_Response(AT_POWER_DOWN))	// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//		//			appData.stage = 0;
//		//		if (AT_Response(AT_OK)) {// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//		//			appData.stage++;
//		//		} else if (LapsedTime >= (Seg1)) {// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//		//			appData.stage = 0;
//		//		}
//
//			if ((AT_Response(AT_POWER_DOWN))||(LapsedTime >= (Seg1)))		// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//				appData.stage = 0;										// o si paso mas de 1 Seg sin respuesta, vuevlo al encendido (stage 0)
//			else if (AT_Response(AT_OK)) 								// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//				appData.stage++;
////				appData.stage = 5;
////				appData.stage = 4;
//
//			break;
//		}
//		case 4:
//		{
//			if (AT_Response(AT_GPS_READY))// Corroboro que la UART recibio "GPS Ready\r\n", y si es asi continuo
//				appData.stage++;
////				appData.stage = 5;
//			//		else if (AT_Response(AT_POWER_DOWN))// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//			//			appData.stage = 0;
//			//		else if (LapsedTime >= (Seg1 * 5))// si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//			//			appData.stage = 0;
//
//			else if ((AT_Response(AT_POWER_DOWN))||(LapsedTime >= (Seg1 * 5)))	// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//				appData.stage = 0;												// o si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//			//		else if (LapsedTime >= (Seg1 * 5))// si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//			//			appData.stage = 0;
//
//			break;
//		}
//		case 5:
//		{
//			appData.lcd_state = LCD_CONFIG;
//
//			UART_println("AT+CGPSPWR=1");			// Enciende GPS
//			appData.stage++;
////			appData.stage = 6;
//
//
//			break;
//		}
//		case 6:
//		{
//			if (AT_Response(AT_OK))	// Luego de cada comando enviado espero el OK para corroborar que el comando fue aceptado
//				appData.stage++;
////				appData.stage = 7;
//
//
//			break;
//		}
//		case 7:
//		{
//			UART_println("AT+CGPSRST=0");			//Reinicio GPS en frio
//			appData.stage++;
////			appData.stage = 8;
//
//			break;
//		}
//		case 8:
//		{
//			if (AT_Response(AT_OK))
//			{
//				appData.stage = 0;
//				appData.conf_state = CONFIG_STATE_RESPONSE;
//			}
//
//			break;
//		}
//		default:
//			break;
//	}
//}
//
//void SIM908_ConfigOnSM3()
//{
//	switch (appData.stage)
//	{
//		case 0:
//		{
//			appData.lcd_state = LCD_INIT;
//
//			gps_disable();			// Cambio a la UART GSM
//			gsm_enable();			//
//			Sim_ON();				// PWR_ON en bajo durante 2,5 Seg
//			delay(HalfSeg * 5);
//			appData.stage++;
////			appData.stage = 1;
//			break;
//		}
//		case 1:
//		{
//			UART_println("AT");
//			LapsedTime = 0;
//			appData.stage++;
//			break;
//		}
//		case 2:
//		{
//		//		if (AT_Response(AT_POWER_DOWN))	// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//		//			appData.stage = 0;
//		//		if (AT_Response(AT_OK)) {// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//		//			appData.stage++;
//		//		} else if (LapsedTime >= (Seg1)) {// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//		//			appData.stage = 0;
//		//		}
//
//			if ((AT_Response(AT_POWER_DOWN))||(LapsedTime >= (Seg1)))		// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//				appData.stage = 8;										// o si paso mas de 1 Seg sin respuesta, vuevlo al encendido (stage 0)
//										// POWER OFF DE 1.5 SEG
//			else if (AT_Response(AT_OK)) 								// Corroboro que la UART recibio  "OK\r\n", y si es asi continuo
//				appData.stage++;
////				appData.stage = 4;
//
//			break;
//		}
//		case 3:
//		{
//			if (AT_Response(AT_GPS_READY))// Corroboro que la UART recibio "GPS Ready\r\n", y si es asi continuo
//				appData.stage++;
////				appData.stage = 5;
//			//		else if (AT_Response(AT_POWER_DOWN))// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//			//			appData.stage = 0;
//			//		else if (LapsedTime >= (Seg1 * 5))// si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//			//			appData.stage = 0;
//
//			else if ((AT_Response(AT_POWER_DOWN))||(LapsedTime >= (Seg1 * 5)))	// Corroboro que la UART recibio "NORMAL POWER DOWN\r\n", y si es asi vuelvo al encendido (stage 0)
//				appData.stage = 8;												// o si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//									// POWER OFF DE 1.5 SEG
//			//		else if (LapsedTime >= (Seg1 * 5))// si paso mas de 5 Seg sin respuesta, vuevlo al encendido (stage 0)
//			//			appData.stage = 0;
//
//			break;
//		}
//		case 4:
//		{
//			appData.lcd_state = LCD_CONFIG;
//
//			UART_println("AT+CGPSPWR=1");			// Enciende GPS
//			appData.stage++;
////			appData.stage = 6;
//
//
//			break;
//		}
//		case 5:
//		{
//			if (AT_Response(AT_OK))	// Luego de cada comando enviado espero el OK para corroborar que el comando fue aceptado
//				appData.stage++;
////				appData.stage = 7;
//
//
//			break;
//		}
//		case 6:
//		{
//			UART_println("AT+CGPSRST=0");			//Reinicio GPS en frio
//			appData.stage++;
////			appData.stage = 8;
//
//			break;
//		}
//		case 7:
//		{
//			if (AT_Response(AT_OK))
//			{
//				appData.stage = 0;
//				appData.conf_state = CONFIG_STATE_RESPONSE;
//			}
//
//			break;
//		}
//		case 8:
//		{
//			Sim_OFF();				// PWR_ON en bajo durante 1,5 Seg
//			delay(HalfSeg * 3);
//			appData.stage++;
//			break;
//		}
//		case 9:
//		{
//			Sim_ON();				// PWR_ON en alto durante 2,5 Seg
//			delay(HalfSeg * 5);
//			appData.stage = 0;
//
//		}
//
//		default:
//			break;
//	}
//}
//
///**
// * @brief	Envia un comando AT y verifica si tiene respuesta
// * @return	None
// */
//void SIM908_ConfigResponseSM() {
//	switch (appData.stage) {
//	case 0: {
//		UART_println("AT");
//		LapsedTime = 0;
//		appData.stage++;
//
//		break;
//	}
//	case 1: {
//		if (AT_Response(AT_OK)) {
//			appData.stage = 0;
//			appData.conf_state = CONFIG_STATE_GPS;
//
//
//		}
//		break;
//	}
//	default:
//		break;
//	}
//}
//
///**
// * @brief	Inicializa la UART GPS del modulo
// * @return	None
// */
//void SIM908_ConfigGpsSM() {
//	switch (appData.stage) {
//	case 0: {
//		UART_println("AT+CGPSIPR=115200"); // Configuro velocidad de UART GPS
//		//			UART_println("AT+CGPSIPR=9600");
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		if (AT_Response(AT_OK))
//			appData.stage++;
//		break;
//	}
//	case 2: {
//		/************************************ Seteo las tramas de salida del GPS ********************************************/
//		/*  bit    |0     |  1  |  2  |  3  |  4  |  5  |  6  |  7  |                                                       */
//		/*  trama  |Null  | GGA | GLL | GSA | GSV | RMC | VTG | ZDA |                                                       */
//		/********************************************************************************************************************/
//		UART_println("AT+CGPSOUT=34"); // Configuro las tramas que quiero que salgan por la UART GPS
//		appData.stage++;
//		break;
//	}
//	case 3: {
////		if (AT_Response(AT_OK))
////			appData.stage = 0;
////		appData.state = APP_STATE_GSM;
//		if (AT_Response(AT_OK))
//		{	appData.stage = 0;
//			appData.state = APP_STATE_GSM;
//		}
//		break;
//	}
//	default:
//		break;
//	}
//}
//
///**
// * @brief	Verifica que la SIM este insertada
// * @return	None
// */
//void SIM_GSM_CPIN() {
//	switch (appData.stage) {
//	case 0: {
//		gps_disable(); // Cambio a la UART GSM. Necesario cuando vuelvo de GPS
//		gsm_enable();
//		delay(HalfSeg / 20);
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		appData.lcd_state = LCD_SIM_ANT;
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		UART_println("AT+CPIN?");	// Verifica que la SIM este insertada
//		appData.stage++;
//		break;
//	}
//	case 3: {
//		if (AT_Response(AT_CPIN_READY)) {
//			appData.gsm_state = GSM_STATE_CREG;
//			appData.stage = 0;
//		}
//		timeToError(Seg1 * 3);
////		Errortime = Seg1 * 3;
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Verifica que se encuentre registrado en la red
// * @return	None
// */
//void SIM_GSM_CREG() {
//	switch (appData.stage) {
//	case 0: {
//		LapsedTimeToError = 0;
//
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CREG?"); // Verifica que se encuentre registrado en la red
//		LapsedTime = 0;
//		appData.stage++;
//		break;
//	}
//	case 2: {
//
//		if (AT_Response(AT_REGISTER_OK)) {
//			appData.gsm_state = GSM_STATE_CSQ;
//			appData.stage = 0;
//		} else if (AT_Response(AT_REGISTER_SEARCHING)) {
//			appData.stage--;
//			delay(HalfSeg);
//		}
//
//		timeToError(Seg1 * 10);
////		Errortime = Seg1 * 10;
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Verifica si encuentra Antena y hay una señal mayor a -96dBm
// * @return	None
// */
//void SIM_GSM_CSQ() {
//	switch (appData.stage) {
//	case 0: {
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CSQ");	// Revisa si encuentra Antena y hay una señal mayor a -96dBm
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_SIGNAL)) {
//			appData.gsm_state = GSM_STATE_CONF;
//			appData.stage = 0;
//		}
//		timeToError(Seg1 * 2);
////		Errortime = Seg1 * 2;
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Configura el modulo para realizar conexion por GPRS
// * @return	None
// */
//void SIM_GSM_CONF() {
//	switch (appData.stage) {
//	case 0: {
//		UART_println("AT+CMEE=1");		// Configuro el reporte de errores
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		if (AT_Response(AT_OK))
//			appData.stage++;
//		break;
//	}
//	case 2: {
//		UART_println("AT+CIPRXGET=1");	// Los datos son obtenidos manualmente
//		appData.stage++;
//		break;
//	}
//	case 3: {
//		if (AT_Response(AT_OK))
//			appData.stage++;
//		break;
//	}
//	case 4: {
//		UART_println("AT+CIPMUX=0");		// Una conexion IP
//		appData.stage++;
//		break;
//	}
//	case 5: {
//		if (AT_Response(AT_OK))
//			appData.stage++;
//		break;
//	}
//	case 6: {
//		UART_println("AT+CGDCONT=1,\"IP\",\"INTERNET.CTIMOVIL.COM.AR\"");// Define el contexto PDP
//		appData.stage++;
//		break;
//	}
//	case 7: {
//		if (AT_Response(AT_OK)) {
//			appData.gsm_state = GSM_STATE_CGATT;
//			appData.stage = 0;
//		}
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Realiza la conexion a la red GPRS
// * @return	None
// */
//void SIM_GSM_CGATT() {
//	switch (appData.stage) {
//	case 0: {
//		appData.lcd_state = LCD_RED;
//
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CGATT=1");			// Conectar a la red GPRS
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_OK)) {
//			appData.gsm_state = GSM_STATE_CGSN;
//			appData.stage = 0;
//		}
//		timeToError(Seg1 * 10);
////		Errortime = Seg1 * 10;
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Obtiene el IMEI y lo guarda en la variable "IMEI"
// * @return	None
// */
//void SIM_GSM_CGSN() {
//	switch (appData.stage) {
//	case 0: {
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CGSN");						// Lectura de IMEI
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_SERIAL)) {// Obtiene el IMEI y lo guarda en la variable "IMEI"
//			appData.gsm_state = GSM_STATE_CIPSTART;
//			appData.stage = 0;
//		}
//		timeToError(Seg1 * 3);
////		Errortime = Seg1 * 3;
//		break;
//	}
//	default:
//		break;
//	}                  //END STAGE
//}
//
///**
// * @brief	Realiza una conexion TCP
// * @return	None
// */
//void SIM_GSM_CIPSTART() {
//	switch (appData.stage) {
//	case 0: {
//		appData.lcd_state = LCD_TCP;
//
//		attempt = 0;
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {												//Conexion TCP
////		UART_println("AT+CIPSTART=\"TCP\",\"200.63.20.218\",\"11012\""); //PC-Santiago
////		UART_println("AT+CIPSTART=\"TCP\",\"200.63.20.218\",\"1111\""); 	//Servidor
//		UART_println("AT+CIPSTART=\"TCP\",\"186.19.126.29\",\"11000\""); 	//Lucas
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_OK)) {
//			appData.stage++;
//		}
//		timeToError(Seg1 * 3);
////		Errortime = Seg1 * 3;
//		break;
//	}
//	case 3: {
//		if (AT_Response(AT_CONECT_OK)) { // Corroboro que la UART recibio "CONNECT OK\r\n", y si es asi continuo
//			appData.gsm_state = GSM_STATE_CIPSEND;
//			appData.stage = 0;
//		} else if (AT_Response(AT_CONECT_FAIL)) { // Corroboro que la UART recibio "CONNECT FAIL\r\n", y si es asi se intenta nuevamente realizar la conexion
//			appData.stage--;
//			delay(Seg1 / 50);
//			attempt++;
//			if (attempt == 5) {	// Si luego de  intentos no se logro realizar la conexion, pasa a un estado de error
//				appData.gsm_state = GSM_STATE_ERROR_MSJ;
//			}
//		}
//		timeToError(Seg1 * 15);
////		Errortime = Seg1 * 15;
//		break;
//	}
//	default:
//		break;
//	}	//END STAGE
//}
//
///**
// * @brief	Realiza el envio del IMEI por GPRS
// * @return	None
// */
//void SIM_GSM_CIPSEND() {
//	switch (appData.stage) {
//	case 0: {
//		appData.lcd_state = LCD_ECO;
//
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CIPSEND=15");	// Realiza envio de datos (15 Bytes)
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_CURSOR))	// Espera que en la UART este el cursor para poder enviar
//			appData.stage++;
//		break;
//	}
//	case 3: {
//		UART_print(IMEI);					// Envia el IMEI
//		appData.stage++;
//		break;
//	}
//	case 4: {
//		if (AT_Response(AT_SEND)) {	// Corrobora que la UART recibio "SEND OK\r\n" y si es asi continua
//			appData.stage++;
//		}
//		timeToError(Seg1 * 10);
////		Errortime = Seg1 * 10;
//		break;
//	}
//	case 5: {
////		if (AT_Response(AT_CIPRXGET))
//		delay(HalfSeg / 5);
//		appData.stage++;
//		break;
//	}
//	case 6: {
//		appData.gsm_state = GSM_STATE_CIPRXGET;
//		appData.stage = 0;
//
//		break;
//	}
//	default:
//		break;
//	}	//END STAGE
//}
//
///**
// * @brief	Verifica que el eco se haya realizado correctamente
// * @return	None
// */
//void SIM_GSM_CIPRXGET() {
//	switch (appData.stage) {
//	case 0: {
//		LapsedTimeToError = 0;
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CIPRXGET=2,64");// Lee los datos recibidos por GPRS manualmente hasta un maximo de 64 Bytes
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (AT_Response(AT_RECEIVE)) {// Corrobora que los datos recibidos son los mismos que el IMEI (espera de ECO)
//			appData.gsm_state = GSM_STATE_CIPCLOSE;
//			appData.stage = 0;
//		}
//		timeToError(Seg1 * 3);
////		Errortime = Seg1 * 3;
//		break;
//	}
//	default:
//		break;
//	}	//END STAGE
//}
//
///**
// * @brief	Cierra la conexion TCP
// * @return	None
// */
//void SIM_GSM_CIPCLOSE() {
//	switch (appData.stage) {
//	case 0: {
//		appData.lcd_state = LCD_GSM_OK;
//		UART_println("AT+CIPCLOSE=1");			// Cierra la conexion TCP
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		if (AT_Response(AT_CLOSE)) {
//			appData.stage = 0;
//			appData.gsm_state = GSM_STATE_OK;
//		}
//		break;
//	}
//	default:
//		break;
//	}
//}
//
///**
// * @brief	Inicializa la UART para leer los datos de GPS
// * @return	None
// */
//void SIM908_GpsInit() {
//	gsm_disable();				// Cambio a UART GPS
//	gps_enable();
//	appData.lcd_state = LCD_GPS_INIT;
//	appData.gps_state = GPS_STATE_FIX;
////	appData.gps_state=GPS_TEST_OVER_GSM;
//	appData.stage = 0;
//	LapsedTimeToError = 0;
//}
//
///**
// * @brief	Corrobora los datos desde la UART GPS para saber si hay localizacion o no
// * @return	None
// */
//void SIM908_GpsFix() {
//	int aux = 0;
//	if (NMEA_get(N_GGA, N_RMC)) {		// Obtiene las tramas desde la UART
//		strncpy(sat_GGA, "", 5);
//		strncpy(sat_RMC, "", 5);
//		Start = NULL;
//		End = NULL;
//
//		if ((Start = strstr(N_GGA, ",")) != NULL)// Obtiene la cantidad de satelites en vista de la trama GGA
//			for (aux = 0; aux <= 5; aux++) {
//				if ((Start = strstr(Start + 1, ",")) == NULL)
//					break;
//			}
//		if (Start != NULL)
//			if ((End = strstr(Start + 1, ",")) != NULL)
//				strncpy(sat_GGA, Start + 1, End - (Start + 1));
//		GPS_onView = atoi(sat_GGA);
//
//		Start = NULL;
//		End = NULL;
//
//		if ((Start = strstr(N_RMC, ",")) != NULL)// Obtiene el estado de conexion de la trama RMC (V=Void o A=Active)
//			if ((Start = strstr(Start + 1, ",")) != NULL)
//				strncpy(sat_RMC, Start + 1, 1);
//	}
//
//	if (GPS_onView > 0)	// Si tengo algun satelite a la vista el GPS esta funcionando
//		appData.gps_state = GPS_STATE_OK;// Por lo que vi, ambos suceden al mismo tiempo
//	if (!strcmp(sat_RMC, "A"))// Si la localizacion esta activa el GPS esta funcionando
//		appData.gps_state = GPS_STATE_OK;
//
//	timeToError(Seg1 * 120);
////	Errortime = Seg1 * 10;
//}
//
///**
// * @brief	Corrobora los datos desde la UART GSM para saber si hay localizacion o no
// * @return	None
// */
//void SIM_GPS_TEST_OVER_GSM() {			//COMENTAR
//
//	int aux = 0;
//
//	switch (appData.stage) {
//	case 0: {
//		gps_disable();				//
//		gsm_enable();
//		delay(HalfSeg / 20);
//		strncpy(sat_GGA, "", 5);
//		strncpy(sat_RMC, "", 5);
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		UART_println("AT+CGPSINF=32");
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		if (NMEA_get_gsm(RMC, N_RMC)) {
//			Start = NULL;
//			End = NULL;
//
//			Start = strstr(N_RMC, ",");
//			if (Start != NULL) {
//				Start = strstr(Start + 1, ",");
//				if (Start != NULL)
//					strncpy(sat_RMC, Start + 1, 1);
//			}
//			strncpy(N_RMC, "", 128);
//			appData.stage++;
//		}
//		timeToError(Seg1 * 60);
////		Errortime = Seg1 * 10;
//		break;
//	}
//	case 3: {
//		UART_println("AT+CGPSINF=2");
//		appData.stage++;
//		break;
//	}
//	case 4: {
//		if (NMEA_get_gsm(GGA, N_GGA)) {
//			Start = NULL;
//			End = NULL;
//
//			if ((Start = strstr(N_GGA, ",")) != NULL)
//				for (aux = 0; aux <= 5; aux++) {
//					if ((Start = strstr(Start + 1, ",")) == NULL)
//						break;
//				}
//			if (Start != NULL)
//				if ((End = strstr(Start + 1, ",")) != NULL)
//					strncpy(sat_GGA, Start + 1, End - (Start + 1));
//			GPS_onView = atoi(sat_GGA);
//
//			strncpy(N_GGA, "", 128);
//			appData.stage++;
//		}
//		timeToError(Seg1 * 10);
////		Errortime = Seg1 * 10;
//		break;
//	}
//	case 5: {
//		if (GPS_onView > 0)
//			appData.gps_state = GPS_STATE_OK;
//		if (!strcmp(sat_RMC, "A"))
//			appData.gps_state = GPS_STATE_OK;
//		appData.stage = 0;
//		delay(Seg1);
//		break;
//	}
//	default:
//		break;
//	}	// END stage
//}
//
///**
// * @brief	Reinicia en caliente el GPS
// * @return	None
// */
//void SIM_GPS_RESET() {
//	switch (appData.stage) {
//	case 0: {
//		gps_disable();
//		gsm_enable();
//		delay(HalfSeg / 20);
//		appData.stage++;
//		break;
//	}
//	case 1: {
//		//                            UART_println("AT+CGPSPWR=0");
//		//                            delay(HalfSeg/20);
//		appData.stage++;
//		break;
//	}
//	case 2: {
//		//                            UART_println("AT+CGPSPWR=1");
//		//                            delay(HalfSeg/20);
//		appData.stage++;
//		break;
//	}
//	case 3: {
//		UART_println("AT+CGPSRST=1");
//		delay(HalfSeg / 20);
//		appData.stage++;
//		break;
//	}
//	case 4: {
//		appData.stage = 0;
//		appData.gps_state = GPS_STATE_INIT;
//		break;
//	}
//	default:
//		break;
//	}	// END stage
//}

///////////////////////////////////////////////////////////////////////////////
//**					Maquina de estados Secundarias						*//
//**							Maquina Init								*//
//**							Maquina Config								*//
//**							Maquina GSM									*//
//**							Maquina GPS									*//
//**							Maquina LCD									*//
///////////////////////////////////////////////////////////////////////////////

//////// Maquina INIT
/**
 * @brief	Inicializa Timer, UART, GPIO, y estados iniciales
 * @return	None
 */
void CONTROL_BoardInit() {

	SystemCoreClockUpdate();
	Board_Init();
	SysTick_Config(SystemCoreClock / TICKRATE_HZ1);


//	UART_Init();
//	LCD_Init();
//	SIM_GPIO_Init();
//
//	DEBUGOUT("Inicializando... \r\n");
//
////	appData.state = APP_STATE_INIT;			//inicializacion de estados
////	appData.conf_state = CONFIG_STATE_ON;
////
////
////	appData.gsm_state = GSM_STATE_CPIN;
////	appData.gps_state = GPS_STATE_INIT;
////	appData.lcd_state = LCD_IDLE;
//
//
//	//	appData.state = APP_STATE_CONF;
//
////	APP_SMsInit();

}

void APP_SMsInit(void)
{
	appData.state = APP_STATE_INIT;			//inicializacion de estados
	appData.conf_state = CONFIG_STATE_ON;


	appData.gsm_state = GSM_STATE_CPIN;
	appData.gps_state = GPS_STATE_INIT;
	appData.lcd_state = LCD_IDLE;

}

//////// Maquina Config
/**
 * @brief	Inicializa el modulo SIM908, y realiza configuraciones basicas
 * @return	None
 */
void SIM908_ConfigSM() {
	switch (appData.conf_state) {
	case CONFIG_STATE_ON: {
		SIM908_ConfigOnSM();
		break;
	} //END CONFIG_STATE_ON
	case CONFIG_STATE_RESPONSE: {
		SIM908_ConfigResponseSM();
		break;
	} //END CONFIG_STATE_RESPONSE
	case CONFIG_STATE_GPS: {
		SIM908_ConfigGpsSM();
		break;
	} //END CONFIG_STATE_GPS
	default:
		break;
	} //END conf_state
}

//////// Maquina GSM
void SIM908_GsmSM() {
	switch (appData.gsm_state) {

	/****************************** Chequea que la SIM este insertada ******************************/
	case GSM_STATE_CPIN: {
		SIM_GSM_CPIN();
		break;
	}                  //END GSM_STATE_CPIN

		/****************************** Chequea que se encuentre registrado en la red ******************************/
	case GSM_STATE_CREG: {
		SIM_GSM_CREG();
		break;
	}                  //END GSM_STATE_CREG

		/****************************** Revisa si encuentra Antena y hay una señal mayor a -96dBm ******************************/
	case GSM_STATE_CSQ: {
		SIM_GSM_CSQ();
		break;
	}                  //END GSM_STATE_CSQ

		/****************************** Comandos de configuracion ******************************/
	case GSM_STATE_CONF: {
		SIM_GSM_CONF();
		break;
	}                  //END GSM_STATE_CONF

		/****************************** Conexion GPRS ******************************/
	case GSM_STATE_CGATT: {
		SIM_GSM_CGATT();
		break;
	}                  //END GSM_STATE_CGATT

		/****************************** Lectura del IMEI ******************************/
	case GSM_STATE_CGSN: {
		SIM_GSM_CGSN();
		break;
	}                  //END GSM_STATE_CGSN

		/****************************** Inicio de la conexion TCP ******************************/
	case GSM_STATE_CIPSTART: {
		SIM_GSM_CIPSTART();
		break;
	}	//END GSM_STATE_CIPSTART

		/****************************** Envio del mensaje ******************************/
	case GSM_STATE_CIPSEND: {
		SIM_GSM_CIPSEND();
		break;
	}	//END GSM_STATE_CIPSEND

		/****************************** Recibiendo datos echo ******************************/
	case GSM_STATE_CIPRXGET: {
		SIM_GSM_CIPRXGET();
		break;
	}	//END GSM_STATE_CIPRXGET

		/****************************** Cierre de la conexion TCP ******************************/
	case GSM_STATE_CIPCLOSE: {
		SIM_GSM_CIPCLOSE();
		break;
	}	//END GSM_STATE_CIPCLOSE

	case GSM_STATE_OK: {
		GSM_working = true;
		appData.state = APP_STATE_GPS;
		break;
	}	//END GSM_STATE_OK

	case GSM_STATE_ERROR_MSJ: {
		appData.lcd_state = LCD_ERROR;
		appData.gsm_state = GSM_STATE_ERROR;
		delay(Seg1 * 5);
		break;
	}
	case GSM_STATE_ERROR: {
		appData.gsm_state = GSM_STATE_CPIN;
		if (!GPS_working) {
			appData.state = APP_STATE_GPS;
		}
		break;
	}	//END GSM_STATE_ERROR
	default:
		break;
	}	//END gsm_state
}

//////// Maquina GPS
void SIM908_GpsSM() {
	switch (appData.gps_state) {

	/****************************** Inicializacion GPS ******************************/
	case GPS_STATE_INIT: {
		SIM908_GpsInit();
		break;
	}	//END GPS_STATE_INIT

		/****************************** Verificacion funcionamiento de GPS por la UART GPS ******************************/
	case GPS_STATE_FIX: {
		SIM908_GpsFix();
		break;
	}	//END GPS_STATE_TIME

		/****************************** Verificacion funcionamiento de GPS por la UART GSM ******************************/
	case GPS_TEST_OVER_GSM: {
		SIM_GPS_TEST_OVER_GSM();
		break;
	}

		/****************************** Reinicio del GPS ******************************/
	case GPS_STATE_RESET: {
		SIM_GPS_RESET();
		break;
	}

	case GPS_STATE_OK: {
		GPS_working = true;
		if (GSM_working)
			appData.gps_state = GPS_STATE_INIT;
		else
			appData.state = APP_STATE_GSM;

		break;
	}	//END GPS_STATE_OK

	case GPS_STATE_ERROR_MSJ: {
		gps_disable();
		gsm_enable();
		appData.lcd_state = LCD_ERROR;
		appData.gps_state = GPS_STATE_ERROR;
		delay(Seg1 * 3);
		break;
	}

	case GPS_STATE_ERROR: {
		appData.gps_state = GPS_STATE_RESET;
		appData.stage = 0;
		if (!GSM_working) {
			//                        Flush();
			appData.state = APP_STATE_GSM;
		}
		break;
	}	//END GPS_STATE_ERROR

	default:
		break;
	}	//END gps_state
}

//////// Maquina LCD
/**
 * @brief	Muestra los mensajes en el LCD, dependiendo del estado en el que se encuentre el programa
 * @return	None
 */
void SIM_LCD_MSJ() {

	int aux = 0;
///// LCD //////////
	switch (appData.lcd_state) {
	case LCD_IDLE:
		break;
	case LCD_INIT: {
		LCD_Clrscr();
		LCD_Print("INICIALIZANDO...");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_CONFIG: {
		LCD_Clrscr();
		LCD_Print("  Configurando\n      GPS");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_SIM_ANT: {
		LCD_Clrscr();
		LCD_Print("Comprobando SIM\nAntena y Red");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_RED: {
		LCD_Clrscr();
		LCD_Print("Conectando\n a la Red");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_TCP: {
		LCD_Clrscr();
		LCD_Print("Realizando \nConexion TCP");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_ECO: {
		LCD_Clrscr();
		LCD_Print("Esperando eco\ndel servidor");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_GSM_OK: {
		LCD_Clrscr();
		LCD_Print("Conexion GSM\nFuncionando OK");
		appData.lcd_state = LCD_IDLE;
		break;
	}
	case LCD_GPS_INIT: {
		LCD_Clrscr();
		LCD_Print("Cantidad de Sat.\nGGA");
		LCD_GoToxy(8, 1);
		LCD_Print("RMC");
		appData.lcd_state = LCD_GPS_SHOW;
		Lcd_refresh = 0;
		break;
	}
	case LCD_GPS_SHOW: {
//		if (Lcd_refresh > Seg1) {
		if (Lcd_refresh > Seg1*5) {
			LCD_GoToxy(4, 1);
			LCD_Print(sat_GGA);
			for (aux = 0; aux < (4 - strlen(sat_GGA)); aux++)
				LCD_SendChar(' ');
			LCD_GoToxy(12, 1);
			LCD_Print(sat_RMC);

			strncpy(sat_GGA, "", 5);
			strncpy(sat_RMC, "", 5);

			Lcd_refresh = 0;
		}
		break;
	}
	case LCD_ERROR: {
		LCD_Clrscr();
		LCD_Print("ERROR\n");
		switch (appData.prev_state) {
		case APP_STATE_GSM: {
			LCD_Print("GSM ");
			switch (appData.gsm_prev_state) {
			case GSM_STATE_CPIN: {
				LCD_Print("No SIM");
				break;
			}
			case GSM_STATE_CREG: {
				LCD_Print("Reg. red");
				break;
			}
			case GSM_STATE_CSQ: {
				LCD_Print("No Antena");
				break;
			}
			case GSM_STATE_CONF: {
				LCD_Print("Config ");
				itoa(appData.stage, Error, 10);
				LCD_Print(Error);
				strncpy(Error, "", 5);
//				LCD_Print((char) (appData.stage + '0'));
				break;
			}
			case GSM_STATE_CGATT: {
				LCD_Print("Att Red");
				break;
			}
			case GSM_STATE_CGSN: {
				LCD_Print("Serial");
				break;
			}
			case GSM_STATE_CIPSTART: {
				LCD_Print("TCP Con.");
				break;
			}
			case GSM_STATE_CIPSEND: {
				LCD_Print("Envio");
				itoa(appData.stage, Error, 10);
				LCD_Print(Error);
				strncpy(Error, "", 5);
//				LCD_Print((char) (appData.stage + '0'));
				break;
			}
			case GSM_STATE_CIPRXGET: {
				LCD_Print("Recepcion");
				break;
			}
			case GSM_STATE_CIPCLOSE: {
				LCD_Print("Cierre TCP");
				break;
			}
			default:
				break;
			}	//END gsm_state
			break;
		}	// END APP_STATE_GSM

		case APP_STATE_GPS: {
			LCD_Print("GPS");
			break;
		}

		default:
			LCD_Print("Ninguno");
			break;
		}	//END prev_state

		appData.lcd_state = LCD_IDLE;
		break;
	}
	default:
		break;

	}
}



///////////////////////////////////////////////////////////////////////////////
//**																	*//
//**					Maquina de estados primaria							*//
///////////////////////////////////////////////////////////////////////////////
int main(void) {

//	CONTROL_BoardInit();

//	Teclado_Init();

	CONTROL_BoardInit();
//	APP_SMsInit();
//
//	LCD_Clrscr();
//	LCD_Print("INICIALIZANDO...");

//	LED_Init();

	while (1) {

//		LED_Parpadeo();

//		switch (appData.state) {
//		case APP_STATE_INIT: {
//
////			Teclado_Init();
////
////			CONTROL_BoardInit();
////			APP_SMsInit();
////
////			LCD_Clrscr();
////			LCD_Print("INICIALIZANDO...");
//
//
//			appData.state = APP_STATE_CONF;			//cambio al siguiente estado de state
//			appData.stage = 0;						//cambio al siguiente estado de stage
//
//			break;
//		} //END APP_STATE_INIT
//
//		case APP_STATE_CONF: {
//
//			LCD_Clrscr();
//			LCD_Print("INICIALIZANDO2..");
//
//			SIM908_ConfigSM();
//			break;
//		} //END APP_STATE_CONF
//
//		case APP_STATE_GSM: {
//			SIM908_GsmSM();
//			break;
//		}	//END APP_STATE_GSM
//
//		case APP_STATE_GPS: {
//			SIM908_GpsSM();
//			break;
//		}	//END APP_STATE_GPS
//
//		case APP_STATE_DELAY: {
//			if (LapsedTime >= appData.delayms) {
//				appData.state = appData.prev_state;
//
//				appData.prev_state = -1;
//			}
//			break;
//		}	//END APP_STATE_DELAY
//
//		case APP_STATE_OK: {
//			break;
//		}	//END APP_STATE_OK
//
//		case APP_STATE_ERROR: {
//			break;
//		}	//END APP_STATE_ERROR
//
//		default: {
//			appData.state = APP_STATE_ERROR;
//			break;
//		}
//		}	//END appData.state

//		SIM_LCD_MSJ();
//	timeToError(Errortime);

	}
	return (0); // nunca deberia llegar aca
}


