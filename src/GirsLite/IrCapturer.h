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

#include <Platform.h>

class IrCapturer {
public:
    IrCapturer(unsigned int bufSize, Stream* stream);
    virtual ~IrCapturer() {};
    virtual void reset() = 0;
    unsigned int getFrequency() const { return frequency; };
    virtual unsigned int getCaptureCount() const = 0;
    virtual uint32_t getTime(unsigned int index) const = 0;
    void dump(Stream &stream) const;
    boolean hasContent() const { return getCaptureCount() > 0; }
    virtual void setEndingTimeout(uint32_t) = 0;
    virtual uint32_t getEndingTimeout() const = 0;
    unsigned int getBufSize() const { return bufSize; }
protected:
    unsigned int frequency;

    // Used only for the available() function, to check for interruption
    Stream* stream;
    unsigned int bufSize;

private:
    static const char fEqualsString[];
};

#endif	/* IRCAPTURER_H */
