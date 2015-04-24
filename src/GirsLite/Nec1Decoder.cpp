#include "Nec1Decoder.h"

//Nec1Decoder::~Nec1Decoder() {
//}
//{38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,1,^108m,(16,-4,1,^108m)*) [D:0..255,S:0..255=255-D,F:0..255]

String Nec1Decoder::toString() const {
    return isValid()
            ? (ditto ? "NEC1 ditto" : ("NEC1 " + String(D) + " " + String(S) + " " + String(F)))
            : NULL;
}

unsigned int Nec1Decoder::decodeFlashGap(uint32_t flash, uint32_t gap) const {
    boolean result = getDuration(flash, 1);
    if (!result)
        return invalid;

    return getDuration(gap, 3) ? 1
            : getDuration(gap, 1) ? 0
            : invalid;
}

unsigned int Nec1Decoder::decode(const IrCapturer& irCapturer, unsigned int index) const {
    unsigned int sum = 0;
    for (int i = 7; i >= 0; i--) {
        unsigned int result = decodeFlashGap(irCapturer.getTime(2 * i + index), irCapturer.getTime(2 * i + 1 + index));
        if (result == invalid)
            return invalid;
        sum = (sum << 1) + result;
    }
    return sum;
}

unsigned int Nec1Decoder::decode(const IrReceiver& irCapturer, unsigned int index) const {
    unsigned int sum = 0;
    for (int i = 7; i >= 0; i--) {
        unsigned int result = decodeFlashGap(irCapturer.getTime(2 * i + index), irCapturer.getTime(2 * i + 1 + index));
        if (result == invalid)
            return invalid;
        sum = (sum << 1) + result;
    }
    return sum;
}

Nec1Decoder::Nec1Decoder(const IrReceiver& irCapturer) : Decoder() {
    //super(irSequence);
    unsigned int index = 0;
    boolean success;
    if (irCapturer.getCaptureCount() == 4U) {
        success = getDuration(irCapturer.getTime(index++), 16U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 4U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 1U);
        if (!success)
            return;
        success = getEnding(irCapturer.getTime(index));
        if (!success)
            return;
        ditto = true;
        setValid(true);
    } else if (irCapturer.getCaptureCount() == 34U * 2U) {
        success = getDuration(irCapturer.getTime(index++), 16U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 8U);
        if (!success)
            return;
        D = decode(irCapturer, index);
        if (D == invalid)
            return;
        index += 16;
        S = decode(irCapturer, index);
        if (S == invalid)
            return;
        index += 16;
        F = decode(irCapturer, index);
        if (F == invalid)
            return;
        index += 16;
        int invF = decode(irCapturer, index);
        if (invF < 0)
            return;
        if ((F ^ invF) != 0xFF)
            return;
        index += 16;

        success = getDuration(irCapturer.getTime(index++), 1U);
        if (!success)
            return;
        success = getEnding(irCapturer.getTime(index));
        if (!success)
            return;
        ditto = false;
        setValid(true);
    }
}

Nec1Decoder::Nec1Decoder(const IrCapturer& irCapturer) : Decoder() {
    //super(irSequence);
    unsigned int index = 0;
    boolean success;
    Serial.println("Nec1Decoder");
    Serial.println(irCapturer.getCaptureCount());
    if (irCapturer.getCaptureCount() == 4U) {
        success = getDuration(irCapturer.getTime(index++), 16U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 4U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 1U);
        if (!success)
            return;
        success = getEnding(irCapturer.getTime(index));
        if (!success)
            return;
        ditto = true;
        setValid(true);
    } else if (irCapturer.getCaptureCount() == 34U * 2U) {
        success = getDuration(irCapturer.getTime(index++), 16U);
        if (!success)
            return;
        success = getDuration(irCapturer.getTime(index++), 8U);
        if (!success)
            return;
        D = decode(irCapturer, index);
        if (D == invalid)
            return;
        index += 16;
        S = decode(irCapturer, index);
        if (S == invalid)
            return;
        index += 16;
        F = decode(irCapturer, index);
        if (F == invalid)
            return;
        index += 16;
        int invF = decode(irCapturer, index);
        if (invF < 0)
            return;
        if ((F ^ invF) != 0xFF)
            return;
        index += 16;

        success = getDuration(irCapturer.getTime(index++), 1U);
        if (!success)
            return;
        success = getEnding(irCapturer.getTime(index));
        if (!success)
            return;
        ditto = false;
        setValid(true);
    }
}

/*public static void main(String[] args) {
    try {
        IrSignal irSignal = new IrSignal("0000 006C 0022 0002 015B 00AD 0016 0016 0016 0016 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0041 0016 0041 0016 0041 0016 0016 0016 0016 0016 0016 0016 0041 0016 0041 0016 06A4 015B 0057 0016 0E6C");
        IrSequence irSequence = irSignal.toModulatedIrSequence(3).noisify(20);
        IrSequence[] seqs = irSequence.chop(20000);
        for (IrSequence irs : seqs) {
            System.out.println(irs);
            Nec1Decoder decoder = new Nec1Decoder(irs);
            System.out.println(decoder);
        }

    } catch (IrpMasterException ex) {
        System.err.println(ex);
    }
}*/

