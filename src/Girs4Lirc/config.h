// Configuration options

// Except for the hardware configuration, there are really only two interesting options here.
// They are: LED (define if desired), and RECEIVE/CAPTURE (define exactly one).
// Defining RECEIVE makes the capture command use the demodulating receiver.

#ifndef CONFIG_H
#define CONFIG_H

// Properties
// Girs modules to implement, see http://www.harctoolbox.org/Girs.html
#define TRANSMIT

// Define RECEIVE to us a demodulating receiver (TSOP*).
#define RECEIVE

// Invoke the decoder
#define DECODER

// Presently just dummy, not yet implemented
#define TRANSMITTERS

// Use LEDs as debugging LEDs.
// Note: different semantic from Girs, where it means a command is enabled.
#define LED

#ifdef LED
// Light this led when transmission is taking place
#ifdef TRANSMIT
#define TRANSMITLED 4
#endif

#ifdef RECEIVE
// Light this when receive is pending
#define RECEIVELED 3
#endif // ! RECEIVE

//#define DEBUG

// Light this when listening for command on the input stream
#define COMMANDLED 2

#endif // LED

// Use an LCD display
#define LCD_I2C

// If LCD support desired, include appropriate hardware description
#ifdef LCD_I2C
#ifdef ARDUINO_AVR_MEGA2560
#include <lcd_0x3F_20_4.h>
#else
#include <lcd_0x27_16_2.h>
#endif
#endif

// Support sending signals without modulation
#define NON_MOD

// Character that ends the command lines
#define EOLCHAR '\r'

// Hardware configuration

// Include one file describing the pin configuration
//#include <girs_pins_nano_shield.h>
//#include <girs_pins_nano.h>
#include <girs_pins.h> // Generic


// These are really not defaults, they are the non-changeable values.
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
#define DEFAULT_ENDINGTIMEOUT 30L // milliseconds

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
