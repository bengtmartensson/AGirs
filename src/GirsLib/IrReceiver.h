#ifndef IRRECEIVER_H
#define	IRRECEIVER_H

#include <Arduino.h>
#include "GirsTypes.h"
#include "IrReader.h"

/**
 * This class servers as an abstract base class for IR receivers.
 */
class IrReceiver : public IrReader {
private:
    pin_t pin;
protected:
    microseconds_t markExcess;
public:
    static const microseconds_t defaultMarkExcess = 50U;
    IrReceiver(uint16_t bufSize, pin_t pin, boolean pullup = false, microseconds_t markExcess = defaultMarkExcess);
    virtual ~IrReceiver() {};    
    virtual void enable() = 0;
    virtual void disable() = 0;
    // Use reset() instead
    // virtual void resume() = 0;
    pin_t getPin() const { return pin; }
    void setMarkExcess(microseconds_t markExcess_) { markExcess = markExcess_; }
    microseconds_t getMarkExcess() { return markExcess; }
    //virtual boolean isReady() const = 0;
    //virtual microseconds_t getDuration(uint16_t i) const = 0;
    //virtual unsigned int getDataLength() const = 0;
};

#endif	/* IRRECEIVER_H */

