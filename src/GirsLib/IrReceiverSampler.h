#ifndef IRRECEIVERSAMPLE_H
#define	IRRECEIVERSAMPLE_H

#include "IrReceiver.h"
//static const unsigned int USECPERTICK = 50;   // microseconds per clock interrupt tick
#define USECPERTICK 50  // microseconds per clock interrupt tick
class IrReceiverSampler : public IrReceiver {
public:

    //duration_t *data;
    //uint16_t dataLength;

    /*
     * The structure contains a variety of variables needed by the receiver routines.
     * Typically this data would be part of the IRrecv class however the interrupt service routine
     * must have access to it and you cannot pass a parameter to such a routine. The data must be global.
     * You need not include this file unless you are creating a custom receiver class or extending
     * the provided IRrecv class.
     */

    // receiver states
    enum rcvstate_t {
        STATE_UNKNOWN,
        STATE_IDLE,
        STATE_MARK,
        STATE_SPACE,
        STATE_STOP,
        STATE_RUNNING
    };
    
    unsigned long GAP_TICKS;
    unsigned long TIMEOUT_TICKS;
    
    // information for the interrupt handler
    //typedef struct {
    //    unsigned char recvpin; // pin for IR data from detector
    volatile rcvstate_t rcvstate; // state machine
    // bool blinkflag; // TRUE to enable blinking of pin 13 on IR processing
    volatile unsigned long timer; // state timer, counts 50uS ticks.(and other uses)
    volatile duration_t *rawbuf; //[RAWBUF]; // raw data
    volatile uint16_t rawlen; // counter of entries in rawbuf
    //}
    //irparams_t;

    //static volatile irparams_t irparams;

    static const unsigned long defaultBeginningTimeout = 2000000UL; // 2 seconds
    static const unsigned long defaultEndingTimeout = 30000UL; // 30 milliseconds
    static const uint16_t defaultCaptureLength = 100U;
    
private:
    static IrReceiverSampler *instance; // Singleton class due to ISP
    //static const unsigned int USECPERTICK = 50;   // microseconds per clock interrupt tick
public: // FIXME
    //unsigned long endingTimeout;
    //unsigned long beginningTimeout;
    uint16_t captureLength;

    IrReceiverSampler(pin_t pin, boolean pullup = false, duration_t markExcess = defaultMarkExcess,
            unsigned long beginningTimeout = defaultBeginningTimeout,
            uint16_t captureLength = defaultCaptureLength,
            unsigned long endingTimeout = defaultEndingTimeout);
    virtual ~IrReceiverSampler();

public:
    // factory method
    static IrReceiverSampler *newIrReceiverSampler(pin_t pin, boolean pullup = false, duration_t markExcess = defaultMarkExcess,
            unsigned long beginningTimeout = defaultBeginningTimeout,
            uint16_t captureLength = defaultCaptureLength,
            unsigned long endingTimeout = defaultEndingTimeout);
    
    static void deleteInstance();
    
    static IrReceiverSampler *getInstance() {
        return instance;
    }
public:
    //bool GetResults(IRdecodeBase *decoder);
    void enableIRIn();
    void disableIRIn();
    void resume();
    //const IrReceiver::duration_t *getData() const;
    //uint16_t getDataLength() const;

    void setEndingTimeout(unsigned long timeOut);

    unsigned long getEndingTimeout() const;

    void setBeginningTimeout(unsigned long timeOut);

    unsigned long getBeginningTimeout() const;

    //static unsigned long GAP_TICKS; //must be public & static for the ISR
    //static unsigned long TIMEOUT_TICKS; //must be public & static for the ISR
    //const duration_t *getRawData() const { return (const duration_t *) rawbuf; }
    uint16_t getDataLength() const { return rawlen; }
    duration_t getDuration(uint16_t i) const { return USECPERTICK * rawbuf[i]; }
    boolean isReady() const { return rcvstate == STATE_STOP; }

};

#endif	/* IRRECEIVERSAMPLE_H */

