// Copying and distribution of this file, with or without modification,
// are permitted in any medium without royalty provided the copyright
// notice and this notice are preserved.  This file is offered as-is,
// without any warranty.

// Author: Bengt Martensson

#include "DecodeIRClass.h"

void DecodeIRClass::init(const microseconds_t* data, size_t length, frequency_t frequency) {
    decodeir_context[0] = 0;
    decodeir_context[1] = 0;
    protocol[0] = '\0';
    device = -1;
    subdevice = -1;
    obc = -1;
    for (unsigned int i = 0; i < 4; i++)
        hex[i] = -1;
    misc_message[0] = '\0';
    error_message[0] = '\0';

    DecodeIR(decodeir_context, data, frequency, length / 2, 0,
            protocol, &device, &subdevice, &obc, hex, misc_message,
            error_message);
}

DecodeIRClass::~DecodeIRClass() {
}

DecodeIRClass::DecodeIRClass(const microseconds_t* data, size_t length, frequency_t frequency) {
    init(data, length, frequency);
}

DecodeIRClass::DecodeIRClass(const IrSequence& irSequence, frequency_t frequency) {
    init(irSequence.getDurations(), irSequence.getLength(), frequency);
}
