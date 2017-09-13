/*
 * Teclado.h
 *
 *  Created on: 4/2/2016
 *      Author: Santiago
 */

#ifndef TECLADO_H_
#define TECLADO_H_

//Definiciones
#define	TECLADO_COLUMNA1_PORT	0
#define	TECLADO_COLUMNA1_BIT	21
#define	TECLADO_COLUMNA2_PORT	0
#define	TECLADO_COLUMNA2_BIT	3
#define	TECLADO_COLUMNA3_PORT	0
#define	TECLADO_COLUMNA3_BIT	27
#define	TECLADO_COLUMNA4_PORT	0
#define	TECLADO_COLUMNA4_BIT	28

#define	TECLADO_FILA1_PORT		0
#define	TECLADO_FILA1_BIT		24
#define	TECLADO_FILA2_PORT		0
#define	TECLADO_FILA2_BIT		25
#define	TECLADO_FILA3_PORT		0
#define	TECLADO_FILA3_BIT		26
#define	TECLADO_FILA4_PORT		1
#define	TECLADO_FILA4_BIT		30

#define TECLADO_BUZZER_PORT		0
#define TECLADO_BUZZER_BIT		7

#define TECLA1		0x0001	//1
#define TECLA2		0x0002	//2
#define TECLA3		0x0004	//3
#define TECLA4		0x0008	//A
#define TECLA5		0x0010	//4
#define TECLA6		0x0020	//5
#define TECLA7		0x0040	//6
#define TECLA8		0x0080	//B
#define TECLA9		0x0100	//7
#define TECLA10		0x0200	//8
#define TECLA11		0x0400	//9
#define TECLA12		0x0800	//C
#define TECLA13		0x1000	//*
#define TECLA14		0x2000	//0
#define TECLA15		0x4000	//#
#define TECLA16		0x8000	//D

#define BUZZER_PULSE	20

//#define TICKRATE_HZ1 (11)	/* 11 ticks per second */
#define TICKRATE_HZ1 (120)	/* 1 ticks per second */

// Funciones
void Teclado_Init ();
int Teclado_Presionado ();
bool Teclado_Presion_Larga();

void Buzzer_on();
void Buzzer_off();

#endif /* TECLADO_H_ */
