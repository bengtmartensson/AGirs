#include "IrSignal.h"

IrSignal::IrSignal() {
}

//IrSignal::IrSignal(const IrSignal& orig) {
//}

IrSignal::~IrSignal() {
}

IrSignal::IrSignal(unsigned int frequency_, unsigned int lengthIntro_, unsigned int lengthRepeat_, unsigned int lengthEnding_,
    const uint16_t *intro_, const uint16_t *repeat_, const uint16_t *ending_) {
    frequency = frequency_;
    lengthIntro = lengthIntro_;
    lengthRepeat = lengthRepeat_;
    lengthEnding = lengthEnding_;
    intro = intro_;
    repeat = repeat_;
    ending = ending_;
}

void IrSignal::dump(Stream& stream) {
    stream.print("f=");
    stream.println(frequency);
    dump(stream, lengthIntro, intro);
    dump(stream, lengthRepeat, repeat);
    dump(stream, lengthEnding, ending);
}

void IrSignal::dump(Stream& stream, unsigned int length, const uint16_t *data) {
    for (unsigned int i = 0U; i < length; i++) {
        if (i > 0U)
            stream.print(' ');
        stream.print((i & 1) ? '-' : '+');
        stream.print(data[i]);
    }
    stream.println();
}