/*
 * RTC.h
 *
 *  Created on: 4/1/2016
 *      Author: Lucas
 */

#ifndef RTC_H_
#define RTC_H_

//#include "rtc_17xx_40xx.h"

//void showTime(RTC_TIME_T *pTime);
void RTC_Init(void);
void RTC_Funcion(void);
void RTC_IRQHandler(void);

#endif /* RTC_H_ */
