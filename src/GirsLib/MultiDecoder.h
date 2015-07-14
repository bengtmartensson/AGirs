#ifndef _MULTIDECODER_H
#define _MULTIDECODER_H

#include <Arduino.h>
#include "IrReader.h"

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
  MultiDecoder(const IrReader &irReader);
  virtual ~MultiDecoder() { }
};

#endif // ! _MULTIDECODER_H
