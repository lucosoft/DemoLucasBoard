# DemoLucasBoard

Programa ejemplo que corre sobre el stick LPC1769, compatible con placa base LucasBoard

El Proyecto Demo1769_20161024 consiste en:

- Aplicacion migrada de ProbadorModuloSim908_20160120 sobre LPC1114 para demo TD2
- Perifericos en operacion:
	- Display 1602 sobre TIMER1
	- RS232 sobre UART3
	- Teclado matricial 4x4 sobre TIMER0 (en desarrollo)

Esta aplicacion corre en LPCXpresso LPC1769.

- Jumpers en Lucas Board:
	- JP1 -> JP1_1 = Board_5V, JP1_2 = Buzzer (desmontado)
	- JP2 -> JP2_1 = 7805_5V, JP2_2 = Board_5V, JP2_3 = P3_2
	- JP3 -> JP3_1 = P3_10, JP3_2 = P3_8, JP3_3 = J6-14 (P0[16]-RXD1), JP3_4 = J6-13 (P0[15]-TXD1), JP3_5 = NC, JP3_6 = NC
	- JP4 -> JP4_1 = LM1117T3.3_3V3, JP4_2 = J6-28 (VIO_3V3, VOUT if self powered, else 3V3 input)
	- JP5 -> JP5_1 = LM1117T3.3_3V3, JP5_2 = LPC1114_3V3, JP5_3 = P1_1
	- JP6 -> JP6_1 = J6-2, JP6_2 = Board_5V
