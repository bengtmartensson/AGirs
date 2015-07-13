#ifndef _MULTIDECODER_H
#define _MULTIDECODER_H

#include <Arduino.h>
#include <IRLib.h>
#include "IrReceiverSampler.h"

class MultiDecoder {
 public:
  enum Type {
    timeout,
    noise,
    undecoded,
    nec,
    nec_ditto,
    rc5
  };

 private:
  Type type;
  String decode;
  
 public:
  Type getType() const { return type; }
  const String& getDecode() const { return decode; }
  //const IrReceiver::duration_t *getRawData() const;
  //uint16_t getRawDataLength() const;

  MultiDecoder(const IRdecodeBase& decoder);
  MultiDecoder(const IrReceiverSampler &irReceiverSampler);
  ~MultiDecoder() { }
};

#endif // ! _MULTIDECODER_H
