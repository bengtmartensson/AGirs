#ifndef RAWCOMMAND_H
#define	RAWCOMMAND_H

#include <Arduino.h>
#include "GirsTypes.h"
#include "IrSignal.h"

class RawCommand {
public:
    //RawCommand();
    //RawCommand(const RawCommand& orig);
    RawCommand(const String& name, frequency_t frequency,
    uint16_t lengthIntro, uint16_t lengthRepeat, uint16_t lengthEnding,
    const microseconds_t *intro, const microseconds_t *repeat, const microseconds_t *ending);
    virtual ~RawCommand() {};
    IrSignal* getIrSignal() const;

private:
    frequency_t frequency;
    uint16_t introLength;
    uint16_t repeatLength;
    uint16_t endingLength;
    microseconds_t *intro;
    microseconds_t *repeat;
    microseconds_t *ending;
};

#endif	/* RAWCOMMAND_H */

