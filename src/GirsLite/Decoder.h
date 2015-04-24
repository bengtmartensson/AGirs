#ifndef DECODER_H
#define	DECODER_H

#include <Arduino.h>

class Decoder {
public:
    Decoder() {
        valid = false;
    }
    //Decoder(const Decoder& orig);
    //virtual ~Decoder();

    virtual String toString() const = 0;

    bool isValid() const {
        return valid;
    }
private:
    //const static double tolerance = 0.1; //10% relative tolerance
    const static uint32_t endingMin = 20000U;

    bool valid;

protected:
    static const int unsigned invalid = (unsigned int)-1;
    void setValid(bool valid_) {
        valid = valid_;
    }

    virtual uint32_t getTimebase() const = 0;
    virtual uint32_t getTimebaseLower() const = 0;
    virtual uint32_t getTimebaseUpper() const = 0;

    bool getDuration(uint32_t duration, int time) const {
        return duration <= time * getTimebaseUpper()
                && duration >= time * getTimebaseLower();
    }

    static bool getEnding(uint32_t duration) {
        return duration > endingMin;
    }
};
#endif	/* DECODER_H */
