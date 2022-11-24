/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


DigitalOut led(LED1);

bool pressed[4][4] = { 0 };
void read_keypad(PortInOut& select, PortInOut& read) {
	memset(pressed, 0, sizeof(pressed)); // alle werte in dem array auf false setzen

	for (int idx = 0; idx < 4; idx++) {
		select = ~(1 << idx); // richtige reihe auswählen

		uint8_t val = ~read;

		for (int i = 0; i < 4; i++) { 
			pressed[idx][i] = (val & (0b00010000 << i)) ? true : false; // spalte lesen und in das array schreiben
		}
	}
}

int main() {
	PortInOut select(PortB, 0b00001111);
	select.output();
	select.mode(OpenDrain);

	PortInOut read(PortB, 0b11110000);
	read.input();
	read.mode(PullUp);


	while (true) {
		read_keypad(select, read);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				printf("%d ", pressed[i][j]);
			}
			printf("\n\r");
		}
		printf("\n\r");
	}
}
