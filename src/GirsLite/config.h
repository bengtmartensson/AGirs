// Configuration options

// Define for IrScrutinizer optimized version
#define GIRSLITE

// Except for the hardware configuration, there are mainly two interesting options here.
// They are: LED (define if desired), and RECEIVE/CAPTURE.
// Defining RECEIVE instead of CAPTURE makes the capture command use the demodulating receiver.
// (A future version of IrScrutinizer will support selecting "receive" as alternative
// to "capture".)

#ifndef CONFIG_H
#define CONFIG_H

// Properties
// Girs modules to implement, see http://www.harctoolbox.org/Girs.html
#define PROGNAME "GirsLite"
#define TRANSMIT

// Use one of these two, normally CAPTURE
// Define RECEIVE to us a demodulating receiver (TSOP*) instead.
#define CAPTURE
#define RECEIVE

// Use LEDs as debugging LEDs.
#define LED

#ifdef LED
// Light this led when transmission is taking place
#define TRANSMITLED 4

#ifdef RECEIVE
// Light this when receive is pending
#define RECEIVELED 3
#endif

#ifdef CAPTURE
// Light this when capture is pending
#define CAPTURELED 3
#endif // ! RECEIVE

// Light this when listening for command on the input stream
#define COMMANDLED 2
#endif // LED

// Character that ends the command lines
#define EOLCHAR '\r'

// Hardware configuration

// Has a non-modulating sender, typically RF.
//#define NON_MOD

// Include one file describing the pin configuration
#ifdef ARDUINO_AVR_NANO
//#include <girs_pins_nano_shield.h>
#include <girs_pins_nano.h>
#else
#include <girs_pins.h> // Generic
#endif

// These are really not defaults, they are the non-changeable values.
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds

#define DEFAULT_RECEIVE_ENDINGTIMEOUT 30L // milliseconds

// Size of capture and receive arrays
#define DEFAULT_CAPTURESIZE 300U // must be even

#define serialBaud 115200
#define serialTimeout 5000L


#ifdef RECEIVE
// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50U
#endif

// Stream to read and write, must be a (sub)class of Stream
#define STREAM Serial

#endif // ! CONFIG_H
