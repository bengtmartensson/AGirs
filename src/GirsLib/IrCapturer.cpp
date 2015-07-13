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
#if 0
IrCapturer::IrCapturer(unsigned int bufSize_, Stream* stream_) {
    bufSize = bufSize_;
    stream = stream_;
    frequency = (unsigned int) -1;
    beginningTimeout = 10000L;
}

const char IrCapturer::fEqualsString[] = "f=";
#endif

void IrCapturer::dump(Stream &stream) const {
#if 0
    if (getFrequency() > 0 && getFrequency() != (unsigned int) - 1) {
        stream.print(fEqualsString);
        stream.print(getFrequency(), DEC);
        stream.write(' ');
    }
#endif
    uint16_t count = getCaptureCount();
    for (uint16_t i = 0; i < count; i++) {
        stream.write((i & 1U) ? '-' : '+');
        stream.print(getTime(i), DEC);
        stream.print(" ");
    }
    stream.println();
}
