#ifndef IRSIGNAL_H
#define	IRSIGNAL_H

#include <Arduino.h>
#include "GirsTypes.h"

class IrSignal {
public:
    IrSignal();
    //IrSignal(const IrSignal& orig);
    virtual ~IrSignal();
    IrSignal(frequency_t frequency, uint16_t lengthIntro, uint16_t lengthRepeat, uint16_t lengthEnding,
    const microseconds_t *intro, const microseconds_t *repeat, const microseconds_t *ending);
private:
    frequency_t frequency; // In Hz
    unsigned int lengthIntro;
    unsigned int lengthRepeat;
    unsigned int lengthEnding;
    const microseconds_t *intro;
    const microseconds_t *repeat;
    const microseconds_t *ending;

public:
    frequency_t getFrequency() const { return frequency; }
    const microseconds_t *getEnding() const { return ending; }
    const microseconds_t *getRepeat() const { return repeat; }
    const microseconds_t *getIntro() const { return intro; }
    uint16_t getLengthEnding() const {return lengthEnding; }
    uint16_t getLengthRepeat() const { return lengthRepeat; }
    uint16_t getLengthIntro() const { return lengthIntro; }

    void dump(Stream& stream);
    void dump(Stream& stream, uint16_t, const microseconds_t *data);
};

#endif	/* IRSIGNAL_H */

