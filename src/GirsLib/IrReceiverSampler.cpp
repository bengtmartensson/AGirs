#include "IrReceiverSampler.h"
// Provides ISR
#include <avr/interrupt.h>
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define CLKFUDGE 5      // fudge factor for clock interrupt overhead
#ifdef F_CPU
#define SYSCLOCK F_CPU     // main Arduino clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif
#define PRESCALE 8      // timer clock prescale
#define CLKSPERUSEC (SYSCLOCK/PRESCALE/1000000)   // timer clocks per microsecond

#include <IRLibTimer.h>

IrReceiverSampler *IrReceiverSampler::instance = NULL;

IrReceiverSampler::IrReceiverSampler(uint16_t captureLength, pin_t pin_, boolean pullup,
        microseconds_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout) : IrReceiver(captureLength, pin_, pullup, markExcess) {
    setBeginningTimeout(beginningTimeout);
    setEndingTimeout(endingTimeout);
    rawbuf = new uint16_t[captureLength];
    rawlen = 0;
    timer = 0;
    rcvstate = STATE_IDLE;
}

IrReceiverSampler *IrReceiverSampler::newIrReceiverSampler(uint16_t captureLength, pin_t pin, boolean pullup,
        microseconds_t markExcess,
        milliseconds_t beginningTimeout,
        milliseconds_t endingTimeout) {
    if (instance != NULL)
        return NULL;
    instance = new IrReceiverSampler(captureLength, pin, pullup, markExcess, beginningTimeout, endingTimeout);
    return instance;
}

void IrReceiverSampler::deleteInstance() {
    delete instance;
    instance = NULL;
}

IrReceiverSampler::~IrReceiverSampler() {
    delete [] rawbuf;
}

/*
 * The original IRrecv which uses 50us timer driven interrupts to sample input pin.
 * was: resume()
 */
void IrReceiverSampler::reset() {
    // initialize state machine variables
    rcvstate = STATE_IDLE;
    rawlen = 0;
    //IRrecvBase::resume();
}

void IrReceiverSampler::enable() {
    reset();
    //IRrecvBase::enableIRIn();
    // setup pulse clock timer interrupt
    noInterrupts();
    IR_RECV_CONFIG_TICKS();
    IR_RECV_ENABLE_INTR;
    interrupts();
}

void IrReceiverSampler::disable() {
    IR_RECV_DISABLE_INTR;
}

//bool IRrecv::GetResults(IRdecodeBase *decoder) {
//    if (irparams.rcvstate != STATE_STOP) return false;
//    IRrecvBase::GetResults(decoder, USECPERTICK);
//    return true;
//}

void IrReceiverSampler::setEndingTimeout(milliseconds_t timeOut) {
    GAP_TICKS = (1000UL  * (uint32_t)timeOut) / USECPERTICK;
}

void IrReceiverSampler::setBeginningTimeout(milliseconds_t timeOut) {
    TIMEOUT_TICKS = (1000UL * (uint32_t)timeOut) / USECPERTICK;
}

milliseconds_t IrReceiverSampler::getEndingTimeout() const {
    return (milliseconds_t) (GAP_TICKS * USECPERTICK)/1000UL;
}

milliseconds_t IrReceiverSampler::getBeginningTimeout() const {
    return (milliseconds_t) (TIMEOUT_TICKS * USECPERTICK)/1000U;
}

/*
 * This interrupt service routine is only used by IRrecv and may or may not be used by other
 * extensions of the IRrecBase. It is timer driven interrupt code to collect raw data.
 * Widths of alternating SPACE, MARK are recorded in rawbuf. Recorded in ticks of 50 microseconds.
 * rawlen counts the number of entries recorded so far. First entry is the SPACE between transmissions.
 * As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
 * As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts.
 */
ISR(IR_RECV_INTR_NAME) {
    //digitalWrite(29, HIGH);
    enum irdata_t {
        IR_MARK = 0,
        IR_SPACE = 1
    };
    IrReceiverSampler *recv = IrReceiverSampler::getInstance();
    irdata_t irdata = (irdata_t) digitalRead(recv->getPin());
    recv->timer++; // One more 50us tick
    if (recv->rawlen >= recv->getBufferSize()) {
        // Buffer full
        recv->rcvstate = IrReceiverSampler::STATE_STOP;
    }
    switch (recv->rcvstate) {
        case IrReceiverSampler::STATE_IDLE: // In the middle of a gap
            if (irdata == IR_MARK) {
                if (recv->timer < recv->GAP_TICKS) {
                    // Not big enough to be a gap.
                    recv->timer = 0;
                } else {
                    // gap just ended, record duration and start recording transmission
                    recv->rawlen = 0;
                    // Do not save first space, it is meaningless
                    //recv->rawbuf[recv->rawlen++] = recv->timer;
                    recv->timer = 0;
                    recv->rcvstate = IrReceiverSampler::STATE_MARK;
                }
            } else {
                if (recv->timer >= recv->TIMEOUT_TICKS) {
                    recv->rawbuf[recv->rawlen++] = recv->timer;
                    recv->timer = 0;
                    recv->rcvstate = IrReceiverSampler::STATE_STOP;
                }
            }
            break;
        case IrReceiverSampler::STATE_MARK: // timing MARK
            if (irdata == IR_SPACE) { // MARK ended, record time
                recv->rawbuf[recv->rawlen++] = recv->timer;
                recv->timer = 0;
                recv->rcvstate = IrReceiverSampler::STATE_SPACE;
            }
            break;
        case IrReceiverSampler::STATE_SPACE: // timing SPACE
            if (irdata == IR_MARK) { // SPACE just ended, record it
                recv->rawbuf[recv->rawlen++] = recv->timer;
                recv->timer = 0;
                recv->rcvstate = IrReceiverSampler::STATE_MARK;
            } else { // SPACE
                if (recv->timer > recv->GAP_TICKS) {
                    // big SPACE, indicates gap between codes
                    // Mark current code as ready for processing
                    // Switch to STOP
                    // Don't reset timer; keep counting space width
                    recv->rawbuf[recv->rawlen++] = recv->timer;
                    recv->timer = 0;
                    recv->rcvstate = IrReceiverSampler::STATE_STOP;
                }
            }
            break;
        case IrReceiverSampler::STATE_STOP: // waiting, measuring gap
            if (irdata == IR_MARK) { // reset gap timer
                recv->timer = 0;
            }
            break;
        default:
            //nothing
            break;
    }
    //do_Blink();
    //digitalWrite(29, LOW);
}
