#ifndef IRRECEIVER_H
#define	IRRECEIVER_H

#include "IRLib.h"
#include "IRLibRData.h"
#include "IRLibMatch.h"

extern volatile irparams_t irparams;

class IrReceiver : public IRrecv {
public:

    IrReceiver(unsigned char recvpin) : IRrecv(recvpin) {
    };

    bool hasContent() const {
        return irparams.rcvstate == STATE_STOP;
    }

    unsigned char getCaptureCount() const {
        return irparams.rawlen;
    }

    unsigned long getTime(unsigned char i) const {
        return i == irparams.rawlen - 1
                ? getEndingTimeout()
                : irparams.rawbuf[i + 1] * USECPERTICK + ((i % 2) ? Mark_Excess : -Mark_Excess);
    }
    void dump(Stream &stream) const;
};

#endif	/* IRRECEIVER_H */

