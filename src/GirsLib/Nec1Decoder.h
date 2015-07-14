#ifndef NEC1DECODER_H
#define	NEC1DECODER_H

#include "Decoder.h"
#include "IrReader.h"

class Nec1Decoder : public Decoder {
private:
    static const microseconds_t timebase = 564;
    static const microseconds_t timebaseUpper = 650;
    static const microseconds_t timebaseLower = 450;
    
    // NOTE: use a signed type to be able to return the value invalid.
    int16_t F;
    int16_t D;
    int16_t S;
    bool ditto;
    
    static boolean getDuration(microseconds_t duration, uint16_t time) {
        return duration <= time * timebaseUpper
                && duration >= time * timebaseLower;
    }
    static int decode(const IrReader &irCapturer, unsigned int index);
    static int decodeFlashGap(microseconds_t flash, microseconds_t gap);
public:
    Nec1Decoder();
    Nec1Decoder(const IrReader& irCapturer);
    virtual ~Nec1Decoder() {};

    int16_t getF() const {
        return F;
    }
    int16_t getD() const {
        return D;
    }
    int16_t getS() const {
        return S;
    }
    boolean isDitto() const {
        return ditto;
    }
    //static microseconds_t getTimebase() const { return timebase; }
    //static microseconds_t getTimebaseLower() const { return timebaseLower; }
    //static microseconds_t getTimebaseUpper() const { return timebaseUpper; }

    String toString() const;
    
    static boolean tryDecode(const IrReader &irCapturer, Stream& string);
};

#endif	/* NEC1DECODER_H */
