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

#ifndef IRCAPTURER_H
#define	IRCAPTURER_H
#include <Arduino.h>

class IrCapturer {
protected:
    //unsigned int frequency;
    //uint32_t beginningTimeout; // unit milli seconds

    // Used only for the available() function, to check for interruption
    //Stream* stream;
    uint16_t bufSize;

public:
    //IrCapturer(uint16_t bufSize, Stream* stream);

    IrCapturer(uint16_t bufSize_) : bufSize(bufSize_) {
    }

    virtual ~IrCapturer() {
    };
    virtual void reset() = 0;
    //unsigned int getFrequency() const { return frequency; };
    virtual unsigned int getCaptureCount() const = 0;
    virtual uint16_t getTime(uint16_t index) const = 0;
    virtual void dump(Stream &stream) const;

    boolean hasContent() const {
        return getCaptureCount() > 0;
    }

    // These take and return values in milliseconds
    virtual void setEndingTimeout(uint16_t) = 0;
    virtual uint16_t getEndingTimeout() const = 0;
    virtual void setBeginningTimeout(uint16_t timeOut) = 0; //{ beginningTimeout = timeOut/1000L; }
    virtual uint16_t getBeginningTimeout() const = 0; //{ return 1000L * beginningTimeout; }

    uint16_t getBufSize() const {
        return bufSize;
    }
};

#endif	/* IRCAPTURER_H */
