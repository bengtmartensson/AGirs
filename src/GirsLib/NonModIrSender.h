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

#ifndef NONDEMODIRSENDER_H
#define	NONDEMODIRSENDER_H

#include <Arduino.h>

class NonModIrSender {
public:
    NonModIrSender(uint8_t pin);
    virtual ~NonModIrSender();
    
    void send(const uint16_t buf[], uint8_t len);

private:
    uint8_t outputPin;
};

#endif	/* NONDEMODIRSENDER_H */

