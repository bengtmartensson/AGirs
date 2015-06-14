#include "Rc5Decoder.h"

String Rc5Decoder::toString() const {
    return isValid()
      ? String(F("RC5 ")) + String(D) + F(" ") + String(F) + F(" ") + String(T)
      : String();
}

Rc5Decoder::Length Rc5Decoder::decode(uint32_t t) const {
    Length len =  (t < getTimebaseLower()) ? invalid
            : (t <= getTimebaseUpper()) ? half
            : (t >= 2*getTimebaseLower() && t <= 2*getTimebaseUpper()) ? full
            : invalid;
    return len;
}

unsigned int Rc5Decoder::decode(uint32_t flash, uint32_t gap) const {
    boolean result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 3) ? 1
            : getDuration(gap, 1) ? 0
            : invalid;
}

bool Rc5Decoder::tryDecode(const IRdecodeBase& iRdecodeBase, Stream& stream) {
    Rc5Decoder decoder(iRdecodeBase);
    return decoder.printDecode(stream);
}

bool Rc5Decoder::tryDecode(const IrCapturer& irCapturer, Stream& stream) {
    Rc5Decoder decoder(irCapturer);
    return decoder.printDecode(stream);
}

Rc5Decoder::Rc5Decoder(const IrCapturer& irCapturer) {
    //super(irSequence);
    unsigned int index = 0U;
    unsigned int sum = 0U;
    int doublet = -1;

    while (doublet < 25) {
        Length length = decode(irCapturer.getTime(index++));
        if (length == invalid)
            return;
        doublet += (int) length;
        if (doublet % 2 == 1)
            sum = (sum << 1U) + (index & 1U);
    }
    sum = ~sum & 0x1FFFU;

    boolean success = getEnding(irCapturer.getTime(irCapturer.getCaptureCount()-1));
    if (!success)
        return;

    F = (sum & 0x3FU) | ((~sum & 0x1000U) >> 6U);
    D = (sum & 0x7C0U) >> 6U;
    T = (sum & 0x0800U) >> 11U;

    setValid(true);
}

Rc5Decoder::Rc5Decoder(const IRdecodeBase& iRdecodeBase) {
    unsigned int index = 1U;
    unsigned int sum = 0U;
    int doublet = -1;

    while (doublet < 25) {
        Length length = decode(iRdecodeBase.rawbuf[index++]);
        if (length == invalid)
            return;
        doublet += (int) length;
        if (doublet % 2 == 1)
            sum = (sum << 1U) + (index & 1U);
    }
    sum &= 0x1FFFU;

    boolean success = getEnding(iRdecodeBase.rawbuf[iRdecodeBase.rawlen-1]);
    if (!success)
        return;

    F = (sum & 0x3FU) | ((~sum & 0x1000U) >> 6U);
    D = (sum & 0x7C0U) >> 6U;
    T = (sum & 0x0800U) >> 11U;

    setValid(true);
}
