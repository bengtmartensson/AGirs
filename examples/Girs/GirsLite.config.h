// Configuration options

#pragma once

// This is GirsLite, a configuration optimized for IrScrutinizer and Lirc.

// Displayed in greeting messages
#define PROGNAME "GirsLite"

// VERSION is defined in GirsLib/version.h,
// (which is generated from library.properties),
// and should normally not be changed here.

// Define Girs modules to implement, see http://www.harctoolbox.org/Girs.html

// Transmit IR signals, requires IR Leds.
#define TRANSMIT

// Render some known protocols, presently NEC1 and RC%
#define RENDERER

// Capture IR signals, requires non-demodulating IR sensor (TSMP58000, QSE159, etc)
#define CAPTURE

// Receive demodulated IR signals, require demodulating IR sensor (TSMP* or equivalent)
#define RECEIVE

//#define DECODER
//#define DECODELED

// LCD display with I2C connection. Defines a command "lcd".
//#define LCD

// Use LEDs & define command "led"
#define LED

// Allow to change the parameters, like timeouts
#define PARAMETERS
//#define NAMED_COMMANDS

// Define to have the receive command report the duration, even if DECODE is defined
// and a decode was found. Use this for Lirc.
//#define DONT_REPORT_DECODES

// Have parameters for transmitled etc.
//#define CONFIGURABLE_LEDS

// Support sending signals without modulation, e.g. with RF module.
//#define NON_MOD

// Reset command, use at own risk
//#define RESET

// Defines command "memory" reporting free (SRAM) memory.
//#define FREEMEM

// Command "info"
//#define INFO

// Command "hex", sending Pronto hex signals
//#define PRONTO

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

// Define an LCD symbol to use
#ifdef LCD
#define LCD_0x3F_20_4
//#define LCD_0x27_20_4
//#define LCD_0x27_16_2
#endif // LCD

// Include one file describing the pin configuration
// Use one of the provided, or write your own.
#include <girs_hw_config.h> // Generic

// Without PARAMETERS, these are really not defaults,
// they are the non-changeable values.
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds
#define DEFAULT_RECEIVE_ENDINGTIMEOUT 50L // milliseconds

// Size of capture and receive arrays
#define DEFAULT_CAPTURESIZE 400U // must be even

#ifdef RECEIVE
// This quantity is added to all gaps and subtracted from all marks when receiving.
#define IRRECEIVER_MARK_EXCESS 50
#endif

#ifdef CAPTURE
// This quantity is added to all gaps and subtracted from all marks when capturing.
#define IRSENSOR_MARK_EXCESS -10
#endif

#if !defined(ETHERNET) | defined(SERIAL_DEBUG)
#define serialBaud 115200
#define serialTimeout 5000L
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)
