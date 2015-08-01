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

    uint32_t GAP_TICKS;
    uint32_t TIMEOUT_TICKS;

    // information for the interrupt handler
    //typedef struct {
    //    unsigned char recvpin; // pin for IR data from detector
    volatile rcvstate_t rcvstate; // state machine
    // bool blinkflag; // TRUE to enable blinking of pin 13 on IR processing
    volatile uint32_t timer; // state timer, counts 50uS ticks.(and other uses)
    volatile uint16_t *rawbuf; //[RAWBUF]; // raw data
    volatile uint16_t rawlen; // counter of entries in rawbuf
    //}
    //irparams_t;

    //static volatile irparams_t irparams;

    static const milliseconds_t defaultBeginningTimeout = 2000UL; // 2 seconds
    static const milliseconds_t defaultEndingTimeout = 30UL; // 30 milliseconds
    static const uint16_t defaultCaptureLength = 100U;
    static const pin_t defaultPin = 5;

private:
    static IrReceiverSampler *instance; // Singleton class due to ISP
    //static const unsigned int USECPERTICK = 50;   // microseconds per clock interrupt tick
protected:
    virtual ~IrReceiverSampler();
public: // FIXME
    //unsigned long endingTimeout;
    //unsigned long beginningTimeout;
    ///uint16_t captureLength;

    IrReceiverSampler(uint16_t captureLength = defaultCaptureLength, pin_t pin = defaultPin, boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    // factory method
    static IrReceiverSampler *newIrReceiverSampler(uint16_t captureLength = defaultCaptureLength,
            pin_t pin = defaultPin, boolean pullup = false,
            microseconds_t markExcess = defaultMarkExcess,
            milliseconds_t beginningTimeout = defaultBeginningTimeout,
            milliseconds_t endingTimeout = defaultEndingTimeout);

    static void deleteInstance();

    static IrReceiverSampler *getInstance() {
        return instance;
    }
    void enable();
    void disable();
    void reset(); //resume();
    //const IrReceiver::duration_t *getData() const;
    //uint16_t getDataLength() const;

    void setEndingTimeout(milliseconds_t timeOut);

    milliseconds_t getEndingTimeout() const;

    void setBeginningTimeout(milliseconds_t timeOut);

    milliseconds_t getBeginningTimeout() const;

    //static unsigned long GAP_TICKS; //must be public & static for the ISR
    //static unsigned long TIMEOUT_TICKS; //must be public & static for the ISR
    //const duration_t *getRawData() const { return (const duration_t *) rawbuf; }
    uint16_t getDataLength() const { return rawlen; }
    microseconds_t getDuration(uint16_t i) const { return USECPERTICK * rawbuf[i] + (i&1 ? markExcess : -markExcess); }
    boolean isReady() const { return rcvstate == STATE_STOP; }

};

#endif	/* IRRECEIVERSAMPLE_H */

