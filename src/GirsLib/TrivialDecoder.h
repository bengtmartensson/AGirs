// Need a trivial IRdecodeBase instance, but that class is abstract...
class TrivialIrDecoder : public IRdecodeBase {
  public:
  boolean isTimeOut() {
    return rawlen == 1;
  }
  unsigned char getRawlen() {
    return rawlen;
  }
};
