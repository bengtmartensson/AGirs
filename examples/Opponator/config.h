// Configuration options

#pragma once

// Properties
// Girs modules to implement, see http://www.harctoolbox.org/Girs.html
#define TRANSMIT
#define RENDERER
#define CAPTURE
#define RECEIVE
#define DECODER
#define PARAMETERS
#define LISTEN

// Additional modules
// Support sending signals without modulation
#define NON_MOD

// Try to decode also captures, normally not sensible
//#define DECODE_CAPTURES

// Have parameters for transmitled etc.
#define CONFIGURABLE_LEDS

// Reset command
#define RESET

// Report free memory (for debugging). Command "info".
#define FREEMEM

// Leds connected, implements a command "led".
#define LED

// Light this led when transmission is taking place
#define TRANSMITLED 8

// Light this when receive is pending
#define RECEIVELED 7

// Light this when capture is pending
#define CAPTURELED 6

// Light this when listening for command on the input stream
#define COMMANDLED 5

// Define as an expression that takes MultiDecoder decodes (as per enum Type
// in Multidecoder.h) to logical leds.
#define DECODELED(decode) ((decode)+1)

// Other properties

// LCD display with parallel connection, 4 bit mode. Defines a command "lcd".
//#define LCD_4BIT

// LCD display with I2C connection. Defines a command "lcd".
#define LCD

// Hardware configuration

#define LCD_0x27_16_2

// Include one file describing the pin configuration
#include "girs_hw_config.h"

// Defaults

// Character that ends the command lines
#define EOLCHAR '\r'

#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds

#define serialBaud 115200
#define serialTimeout 5000L

#define DEFAULT_ENDINGTIMEOUT 35L // milliseconds

// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50U
