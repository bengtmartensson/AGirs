#ifndef RC5DECODER_H
#define	RC5DECODER_H

#include "Decoder.h"
#include "IrCapturer.h"
#include "IrReceiver.h"

class Rc5Decoder : public Decoder {
public:
    //Rc5Decoder();
    //Rc5Decoder(const Rc5Decoder& orig);
    Rc5Decoder(IrCapturer& irCapturer);
    Rc5Decoder(IrReceiver& irReceiver);
    //virtual ~Rc5Decoder();
    String toString() const;

    unsigned int getF() const {
        return F;
    }

    unsigned int getD() const {
        return D;
    }

    unsigned int getT() const {
        return T;
    }
private:
    const static uint32_t timebase = 889;
    const static uint32_t timebaseLower = 800;
    const static uint32_t timebaseUpper = 1000;
    unsigned int F;
    unsigned int D;
    unsigned int T;

    enum Length {
        invalid = 0,
        half = 1,
        full = 2
    };

    Length decode(uint32_t t) const;
    unsigned int decode(uint32_t flash, uint32_t gap) const;
    unsigned int decode(IrCapturer &irCapture, unsigned int index) const;

    uint32_t getTimebase() const { return timebase; }
    uint32_t getTimebaseLower() const { return timebaseLower; }
    uint32_t getTimebaseUpper() const { return timebaseUpper; }
};

#endif	/* RC5DECODER_H */