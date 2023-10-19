/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>


DigitalOut led(LED1);

bool pressed[4][4] = { 0 };
void read_keypad(PortInOut& select, PortInOut& read) {
	memset(pressed, 0, sizeof(pressed)); // alle werte in dem array auf false setzen

	for (int idx = 0; idx < 4; idx++) {
		select = ~(1 << idx); // richtige reihe auswählen
        thread_sleep_for(1);

		uint8_t val = ~read;

		for (int i = 0; i < 4; i++) { 
			pressed[idx][i] = (val & (0b00010000 << i)) ? true : false; // spalte lesen und in das array schreiben
		}
	}
}

const char translations[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

int main() {
	PortInOut select(PortB, 0b00001111);
	select.output();
	select.mode(OpenDrain);

	PortInOut read(PortB, 0b11110000);
	read.input();
	read.mode(PullUp);


	while (true) {
		read_keypad(select, read);
        printf("\033[H");
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
                if (pressed[i][j]) {
                    printf("%c ", translations[i][j]);
                } else {
                    printf(". ");
                }
			}
			printf("\n\r");
		}
		printf("\n\r");
        thread_sleep_for(50);
	}
}
