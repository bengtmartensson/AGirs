/*
Copyright (C) 2014 Bengt Martensson.

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

#include "IrCapturer.h"

IrCapturer::IrCapturer(unsigned int bufSize_, Stream* stream_) {
    bufSize = bufSize_;
    stream = stream_;
    frequency = (unsigned int) -1;
    beginningTimeout = 10000L;
}

const char IrCapturer::fEqualsString[] = "f=";

void IrCapturer::dump(Stream &stream) const {
    if (getFrequency() > 0 && getFrequency() != (unsigned int) -1) {
        stream.print(fEqualsString);
        stream.print(getFrequency(), DEC);
        stream.write(' ');
    }
    unsigned int count = getCaptureCount();

    for (unsigned int i = 0; i < count; i++) {
        uint32_t val = getTime(i);
        stream.write((i & 0x01) ? '-' : '+');
        stream.print(val, DEC);
        stream.print(" ");
    }
    stream.println();
}
