#ifndef RC5DECODER_H
#define	RC5DECODER_H

#include "Decoder.h"
#include "IrReader.h"

class Rc5Decoder : public Decoder {
public:
    Rc5Decoder(const IrReader& irReader);
    virtual ~Rc5Decoder() {}
    String toString() const;

    int16_t getF() const {
        return F;
    }

    int16_t getD() const {
        return D;
    }

    int16_t getT() const {
        return T;
    }
    static boolean tryDecode(const IrReader& irReader, Stream& string);
private:
    const static microseconds_t timebase = 889U;
    const static microseconds_t timebaseLower = 800U;
    const static microseconds_t timebaseUpper = 1000U;
    static boolean getDuration(microseconds_t duration, uint16_t time) {
        return duration <= time * timebaseUpper
                && duration >= time * timebaseLower;
    }
    int16_t F;
    int16_t D;
    int16_t T;

    enum Length {
        invalid = 0,
        half = 1,
        full = 2
    };

    static Length decode(microseconds_t t);
    static unsigned int decode(microseconds_t flash, microseconds_t gap);
};

#endif	/* RC5DECODER_H */
