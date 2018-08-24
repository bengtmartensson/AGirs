// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

// Author: Bengt Martensson

#ifndef DECODEIRCLASS_H
#define DECODEIRCLASS_H

#include <IrSignal.h>
#include "DecodeIR.h"

/**
 * This class is an OO wrapper for DecodeIR, using classes from Infrared4Arduino.
 * NOTE: DecodeIR can deliver more than one decode for a particular
 * IR signal. This class presently only delivers the first decode.
 * Fixing this restriction is not completely straight-forward: a list/vector of
 * decodes should be returned.
 *
 * It depends on the library Infrared (Infrared4Arduino), by myself.
 * It can be installed from the Arduino library manager.
 *
 * This file should _not_ reside in the src directory, because it makes the
 * library depend on Infrared4Arduino.
 */

class DecodeIRClass {
private:
    uint32_t decodeir_context[2] = {0, 0};
    char protocol[255];
    int32_t device = -1;
    int32_t subdevice = -1;
    int32_t obc = -1;
    int32_t hex[4] = {-1, -1, -1, -1};
    char misc_message[255];
    char error_message[255];

    void init(const microseconds_t* data, size_t length, frequency_t frequency = IrSignal::defaultFrequency);

public:
    DecodeIRClass(const microseconds_t* data, size_t length, frequency_t frequency = IrSignal::defaultFrequency);
    DecodeIRClass(const IrSequence& irSequence, frequency_t frequency = IrSignal::defaultFrequency);
    virtual ~DecodeIRClass();

    const char* getProtocol() const { return protocol; }
    int32_t getD() const { return device; }
    int32_t getS() const { return subdevice; }
    int32_t getF() const { return obc; }
    const int32_t *getHex() const { return hex; }
    const char* getMiscMessage() const { return misc_message; }
    const char* getErrorMessage() const { return error_message; }
};

#endif /* DECODEIRCLASS_H */

