#include "MultiDecoder.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"

MultiDecoder::MultiDecoder(IRdecodeBase& decoder) {
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
