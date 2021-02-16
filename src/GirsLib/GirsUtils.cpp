#include "GirsUtils.h"

//#ifdef FREEMEM
// Ref: http://playground.arduino.cc/Code/AvailableMemory#.U0EnzKogTzs
unsigned long GirsUtils::freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (unsigned long) &v - (__brkval == 0 ? (unsigned long) &__heap_start : (unsigned long) __brkval);
}
//#endif

//#ifdef RESET
// TODO: This is somewhat suspect. Use at your own risk.
// Works at least on atmega386 and atmega2560,

// Restarts program from beginning but does not reset the peripherals and registers
void GirsUtils::reset() {
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO) | defined(ARDUINO_ARCH_SAMD)
#warning RESET not working on this platform, generating empty function
#else
    asm volatile("  jmp 0");
#endif
}
