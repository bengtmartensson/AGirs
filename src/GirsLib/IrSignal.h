#ifndef IRSIGNAL_H
#define	IRSIGNAL_H

#include <Arduino.h>

class IrSignal {
public:
    IrSignal();
    //IrSignal(const IrSignal& orig);
    virtual ~IrSignal();
    IrSignal(unsigned int frequency, unsigned int lengthIntro, unsigned int lengthRepeat, unsigned int lengthEnding,
    const uint16_t *intro, const uint16_t *repeat, const uint16_t *ending);
private:
    unsigned int frequency; // In Hz
    unsigned int lengthIntro;
    unsigned int lengthRepeat;
    unsigned int lengthEnding;
    const uint16_t *intro;
    const uint16_t *repeat;
    const uint16_t *ending;

public:
    unsigned int getFrequency() const { return frequency; }
    const uint16_t* getEnding() const { return ending; }
    const uint16_t* getRepeat() const { return repeat; }
    const uint16_t* getIntro() const { return intro; }
    unsigned int getLengthEnding() const {return lengthEnding; }
    unsigned int getLengthRepeat() const { return lengthRepeat; }
    unsigned int getLengthIntro() const { return lengthIntro; }

    void dump(Stream& stream);
    void dump(Stream& stream, unsigned int length, const uint16_t *data);
};

#endif	/* IRSIGNAL_H */

