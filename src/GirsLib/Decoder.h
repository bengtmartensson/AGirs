#ifndef DECODER_H
#define	DECODER_H

#include <Arduino.h>
#include "GirsTypes.h"

class Decoder {
public:
    Decoder() {
        valid = false;
    }
    virtual ~Decoder() {}

    virtual String toString() const = 0;

    virtual boolean isValid() const {
        return valid;
    }

    boolean printDecode(Stream& stream) const {
        if (isValid())
            stream.println(toString());
        return isValid();
    }
    
private:
    //const static double tolerance = 0.1; //10% relative tolerance
    const static uint32_t endingMin = 20000U;

    boolean valid;

protected:
    static const int16_t invalid = -1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    static boolean getEnding(microseconds_t duration) {
        return duration > endingMin;
    }
};
#endif	/* DECODER_H */
