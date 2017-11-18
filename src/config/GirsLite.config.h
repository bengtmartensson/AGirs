// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Define for IrScrutinizer/Lirc optimized version
#define GIRSLITE

// Properties
// Girs modules to implement, see http://www.harctoolbox.org/Girs.html

// Displayed in greeting messages
#define PROGNAME "GirsLite"

// VERSION is defined in Girs[Lite].cpp; should normally not be changed here.

// Transmit IR signals, requires IR Leds.
#define TRANSMIT

// Capture IR signals, requires non-demodulating IR sensor (TSMP58000, QSE159, etc)
#define CAPTURE

// Receive demodulated IR signals, require demodulating IR sensor (TSMP* or equivalent)
#define RECEIVE

// Use LEDs as debugging LEDs.
#define LED

// Allow to change the parameters, like timeouts
#define PARAMETERS

// Support sending signals without modulation, e.g. with RF module.
//#define NON_MOD

// Defines command "memory" reporting free (SRAM) memory.
//#define FREEMEM

#ifdef LED

#ifdef TRANSMIT
// Light this led when transmission is taking place
#define TRANSMITLED 4
#endif

#ifdef RECEIVE
// Light this when receive is pending
#define RECEIVELED 3
#endif

#ifdef CAPTURE
// Light this when capture is pending
#define CAPTURELED 3
#endif

// Light this when listening for command on the input stream
#define COMMANDLED 2
#endif // LED

// Character that ends the command lines
#define EOLCHAR '\r'

// Hardware configuration

// Include one file describing the pin configuration
// Use one in GirsLib, or write your own.
#include "hardware-config/girs_pins.h" // Generic

// Without PARAMETERS, these are really not defaults,
// they are the non-changeable values.
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds

#define DEFAULT_RECEIVE_ENDINGTIMEOUT 50L // milliseconds

// Size of capture and receive arrays
#define DEFAULT_CAPTURESIZE 400U // must be even

#define serialBaud 115200
#define serialTimeout 5000L


#ifdef RECEIVE
// This quantity is added to all gaps and subtracted from all marks when receiving.
#define IRRECEIVER_MARK_EXCESS 50
#endif

#ifdef CAPTURE
// This quantity is added to all gaps and subtracted from all marks when capturing.
#define IRSENSOR_MARK_EXCESS -10
#endif

#endif // ! CONFIG_H
