#include "IrReceiver.h"

void IrReceiver::dump(Stream &stream) const {
    unsigned char count = getCaptureCount();

    for (unsigned char i = 0; i < count; i++) {
        uint32_t val = getTime(i);
        stream.write((i & 0x01) ? '-' : '+');
        stream.print(val, DEC);
        stream.print(F(" "));
    }
    //stream.print(F("-"));
    //stream.println(count < RAWBUF - 1 ? getEndingTimeout() : getTime(count - 1), DEC);
}
