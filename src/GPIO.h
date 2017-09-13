/*
 * gpio.h
 *
 *  Created on: 5/2/2016
 *      Author: Cacciola11
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdbool.h>

#define OUTPUT	true
#define INPUT 	false

#define GPIO_IN1_PORT	0
#define GPIO_IN1_BIT	1
#define GPIO_IN2_PORT	0
#define GPIO_IN2_BIT	3
#define GPIO_IN3_PORT	0
#define GPIO_IN3_BIT	5
#define GPIO_IN4_PORT	0
#define GPIO_IN4_BIT	4

#define GPIO_OUT1_PORT	3
#define GPIO_OUT1_BIT	0
#define GPIO_OUT2_PORT	3
#define GPIO_OUT2_BIT	1
#define GPIO_OUT3_PORT	3
#define GPIO_OUT3_BIT	2

#define GPIO_IN1	0x1
#define GPIO_IN2	0x2
#define GPIO_IN3	0x4
#define GPIO_IN4	0x8

void GPIO_Init (void);

int GPIO_In (void);
bool GPIO_In1 (void);
bool GPIO_In2 (void);
bool GPIO_In3 (void);
bool GPIO_In4 (void);

void GPIO_Out (int);
void GPIO_Out1 (bool);
void GPIO_Out2 (bool);
void GPIO_Out3 (bool);

void PIN_INIT(int,int,bool);

#endif /* GPIO_H_ */
