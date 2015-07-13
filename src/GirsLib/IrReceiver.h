#ifndef IRRECEIVER_H
#define	IRRECEIVER_H

#include <Arduino.h>

class IrReceiver {
public:
    
    typedef uint16_t duration_t;
    typedef uint8_t pin_t;
    
    static const duration_t defaultMarkExcess = 50U;
    
    IrReceiver(pin_t pin, boolean pullup = false, duration_t markExcess = defaultMarkExcess);
    virtual ~IrReceiver();
public:
    //IRrecvBase(void) {};
    //IRrecvBase(uint8_t pin);
    //void No_Output(void);
    //void blink13(bool blinkflag);
    //bool GetResults(IRdecodeBase *decoder, const unsigned int Time_per_Ticks = 1);
    
    virtual void enableIRIn() = 0;
    virtual void resume() = 0;
    pin_t getPin() const { return pin; }
    virtual void setMarkExcess(duration_t markExcess_) { markExcess = markExcess_; }

//protected:
    pin_t pin;
    duration_t markExcess;
    
protected:
    //static unsigned long endingTimeout;
    //static unsigned long beginningTimeout;
    
    
public:
    //static void IrReceiver::setEndingTimeout(unsigned long eto) {
    //    endingTimeout = eto;
    //}

    //static void setBeginningTimeout(unsigned long bto) {
    //    beginningTimeout = bto;
    //}
    
    //virtual const duration_t *getRawData() const = 0;//{ return data; }
    
    virtual boolean isReady() const = 0;
    
    virtual duration_t getDuration(uint16_t i) const = 0;
    
    virtual unsigned int getDataLength() const = 0;//{ return dataLength; }

};

#endif	/* IRRECEIVER_H */

