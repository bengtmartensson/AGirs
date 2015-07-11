/*
Copyright (C) 2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include "NonModIrSender.h"

extern void  My_delay_uSecs(unsigned int T); // from IRLib.c, not really exported, just sloppy encapsulation :-)

NonModIrSender::NonModIrSender(uint8_t pin) {
    outputPin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

NonModIrSender::~NonModIrSender() {
    digitalWrite(outputPin, LOW); // ?
}

void NonModIrSender::send(const unsigned int buf[], uint8_t len) {
    for (unsigned int i = 0; i < len; i++) {
        digitalWrite(outputPin, (i & 1) ? LOW : HIGH);
        My_delay_uSecs(buf[i]);
    }
    digitalWrite(outputPin, LOW);
}
