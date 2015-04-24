#include "Rc5Decoder.h"

String Rc5Decoder::toString() const {
    return isValid()
            ? ("rc5 " + String(D) + " " + String(F) + " " + String(T))
            : NULL;
}

Rc5Decoder::Length Rc5Decoder::decode(uint32_t t) const {
    Length len =  (t < getTimebaseLower()) ? invalid
            : (t < getTimebaseUpper()) ? half
            : (t > 2*getTimebaseLower() && t < 2*getTimebaseUpper()) ? full
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

unsigned int Rc5Decoder::decode(IrCapturer& irCapturer, unsigned int index) const {
    unsigned int sum = 0U;
    for (unsigned int i = 7U; i >= 0U; i--) {
        int result = decode(irCapturer.getTime(2U * i + index), irCapturer.getTime(2U * i + 1U + index));
        if (result == invalid)
            return invalid;
        sum = (sum << 1U) + result;
    }
    return sum;
}

Rc5Decoder::Rc5Decoder(IrCapturer& irCapturer) {
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

Rc5Decoder::Rc5Decoder(IrReceiver& irCapturer) {
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
