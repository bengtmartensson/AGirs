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

#ifndef IRREADER_H
#define	IRREADER_H
#include <Arduino.h>

#include "GirsTypes.h"

/**
 * This is an abstract base class for all IR readers, capturing or receiving.
 * It should also serve as an interface that can be printed and decoded.
 */

class IrReader {
protected:
    //unsigned int frequency;
    //uint32_t beginningTimeout; // unit milli seconds

    // Used only for the available() function, to check for interruption
    //Stream* stream;
    uint16_t bufferSize;

    IrReader(uint16_t bufSize_) : bufferSize(bufSize_) {
    }

    IrReader() {
    }

    virtual ~IrReader() {
    };

public:
    //IrCapturer(uint16_t bufSize, Stream* stream);

    virtual void reset() = 0;
    //unsigned int getFrequency() const { return frequency; };
    virtual uint16_t getDataLength() const = 0; // was getCaptureCount())
    virtual microseconds_t getDuration(uint16_t index) const = 0;
    virtual void dump(Stream &stream) const;

    // was hasContent())
    virtual boolean isReady() const {
        return getDataLength() > 0;
    }

    // These take and return values in milliseconds
    virtual void setEndingTimeout(milliseconds_t timeOut) = 0;
    virtual milliseconds_t getEndingTimeout() const = 0;
    virtual void setBeginningTimeout(milliseconds_t timeOut) = 0; //{ beginningTimeout = timeOut/1000L; }
    virtual milliseconds_t getBeginningTimeout() const = 0; //{ return 1000L * beginningTimeout; }

    uint16_t getBufferSize() const {
        return bufferSize;
    }
};

#endif	/* IRCAPTURER_H */
