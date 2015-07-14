#include "MultiDecoder.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"

MultiDecoder::MultiDecoder(const IrReader &irReceiverSampler) {
    if (irReceiverSampler.getDataLength() == 0U) {
        type = timeout;
        decode = F(".");
        return;
    }

    if (irReceiverSampler.getDataLength() < 3) {
        type = noise;
        decode = F(":");
        return;
    }

    Nec1Decoder nec1decoder(irReceiverSampler);
    if (nec1decoder.isValid()) {
        decode = nec1decoder.toString();
        type = nec1decoder.isDitto() ? nec_ditto : nec;
        return;
    }

    Rc5Decoder rc5decoder(irReceiverSampler);
    if (rc5decoder.isValid()) {
        decode = rc5decoder.toString();
        type = rc5;
        return;
    }

    // Giving up
    decode = String(F("*** ")) + String(irReceiverSampler.getDataLength());
    type = undecoded;
}
