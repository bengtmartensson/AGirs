#include "MultiDecoder.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"

MultiDecoder::MultiDecoder(const IRdecodeBase& decoder) {
    if (decoder.rawlen == 1) {
        type = timeout;
        decode = F(".");
        return;
    }

    if (decoder.rawlen < 4) {
        type = noise;
        decode = F(":");
        return;
    }

    Nec1Decoder nec1decoder(decoder);
    if (nec1decoder.isValid()) {
        decode = nec1decoder.toString();
        type = nec1decoder.isDitto() ? nec_ditto : nec;
        return;
    }

    Rc5Decoder rc5decoder(decoder);
    if (rc5decoder.isValid()) {
        decode = rc5decoder.toString();
        type = rc5;
        return;
    }

    // Giving up
    decode = String(F("*** ")) + String(decoder.rawlen);
    type = undecoded;
}

MultiDecoder::MultiDecoder(const IrReceiverSampler &irReceiverSampler) {
    if (irReceiverSampler.rawlen == 0U) {
        type = timeout;
        decode = F(".");
        return;
    }

    if (irReceiverSampler.rawlen < 3) {
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
    decode = String(F("*** ")) + String(irReceiverSampler.rawlen);
    type = undecoded;
}
