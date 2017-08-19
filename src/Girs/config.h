// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Define for IrScrutinizer/Lirc optimized version
//#define GIRSLITE

// Define for pure listener (not yet implemented)
//#define LISTENER

// Define Girs modules to implement, see http://www.harctoolbox.org/Girs.html
#ifdef GIRSLITE

// Displayed in greeting messages
#define PROGNAME "GirsLite"

// Transmit IR signals, requires IR Leds.
#define TRANSMIT

// Capture IR signals, requires non-demodulating IR sensor (TSMP58000, QSE159, etc)
#define CAPTURE

// Receive demodulated IR signals, require demodulating IR sensor (TSMP* or equivalent)
#define RECEIVE

// Use LEDs, define command "led!
#define LED

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

#elif defined(LISTENER) // ! GIRSLITE

#error LISTENER not yet implemented
#define PROGNAME Listener

#else // ! LISTENER

#define PROGNAME "AGirs"

#define TRANSMIT
#define RENDERER
#define CAPTURE
#define RECEIVE
#define DECODER
#define DECODELED
#define PARAMETERS
#define LISTEN
#define NAMED_COMMANDS

// Define to have the receive command report the duration, even if DECODE is defined
// and a decode was found. Use this for Lirc.
#define DONT_REPORT_DECODES

// Have parameters for transmitled etc.
#define CONFIGURABLE_LEDS

// LCD display with I2C connection. Defines a command "lcd".
#define LCD

// implements a command "led".
#define LED

// Support sending signals without modulation, e.g. with RF module.
#define NON_MOD

// Reset command, use at own risk
#define RESET

// Report free memory (for debugging). Command "memory".
#define FREEMEM

// Command "info"
#define INFO

// Command "hex", sending Pronto hex signals
#define PRONTO

#ifdef LED

#ifdef TRANSMIT
// Light this led when transmission is taking place
#define TRANSMITLED 8
//#define TRANSMITLED 4
#endif

#ifdef RECEIVE
// Light this when receive is pending
#define RECEIVELED 7
//#define RECEIVELED 3
#endif

#ifdef CAPTURE
// Light this when capture is pending
#define CAPTURELED 6
//#define CAPTURELED 2
#endif

// Light this when listening for command on the input stream
#define COMMANDLED 5
//#define COMMANDLED 1

#endif // LED

#endif // ! LISTENER

// Define if using Ethernet (TCP or UDP) as the communication channel
//#define ETHERNET

// Print received commands on the LCD display
//#define DEBUG_CMD

#ifdef ETHERNET

// Issues some debug messages on the serial port
#define SERIAL_DEBUG

// If SERVER but not USEUDP are defined, the progam runs in "telnet" mode,
// waiting for incoming tcp connections on port PORT.  If none is
// defined, try to initiate tcp connection to PEER_IP, port
// PEER_PORT. In both cases, connection is interrupted when input is
// received.  If USEUDP is defined, use UDP instead. Input is then ignored.
// For some reason I do not understand, when SERVER is defined, the first line
// is discarded by server.available().

// multi-command session, otherwise close after one command. Ignored if USEUDP defined,
// while meaningless.
#define SESSION

// Wait for incoming connections, for "telnet-ting" to the Arduino
// If not defined, the Arduino tries to establish a connection to PEER_IP at port PEER_PORT
#define SERVER

// NOTE: do NOT use UDP as preprocessor symbol, it makes Ethernet.h go haywire.
//#define USEUDP

#define SDCARD_ON_ETHERSHIELD_PIN 4 // there is a pin4 to be selected low

#define DHCP // Consumes rather much memory

// Invoke the AMX style beacon, as per https://github.com/bengtmartensson/ABeacon
//#define BEACON

#endif // ETHERNET

// Hardware configuration

// Include one file describing the pin configuration
// Use one in GirsLib, or write your own.
#ifdef ARDUINO
#ifdef ARDUINO_AVR_MEGA2560
#include <girs_pins_mega2560_rear.h>
#else
#include <girs_pins.h> // Generic
#endif
#else // ! ARDUINO
// For compiling a version to test on the PC, not to flash onto the Arduino.
#include <girs_pins_dummy.h>
#endif

// If LCD support desired, include appropriate hardware description
#ifdef LCD
#include <lcd_0x3F_20_4.h>
#endif // ! LCD

#ifdef ETHERNET
#ifndef SERVER
// Host the program tries to contact
#define PEER_IP 192,168,1,3

// Port to be listened on
#define PEER_PORT 44444
#endif

#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // ! DHCP
#define PORT       33333
#endif // ETHERNET

#if defined(ARDUINO_AVR_MEGA2560)
#define LARGE_RAM
#endif

// Defaults

// Character that ends the command lines
#define EOLCHAR '\r'

#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds

#if !defined(ETHERNET) | defined(SERIAL_DEBUG)
#define serialBaud 115200
#define serialTimeout 5000L
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)

#ifdef DECODER
// If using the decoder, be sure to end a capture before the repeat sequence.
#define DEFAULT_RECEIVE_ENDINGTIMEOUT 30L // milliseconds
#else
#define DEFAULT_RECEIVE_ENDINGTIMEOUT 100L // milliseconds
#endif

#ifdef CAPTURE
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds
// Size of capture and receive arrays
#endif

#if defined(CAPTURE) | defined(RECEIVE)
#ifdef LARGE_RAM
#define DEFAULT_CAPTURESIZE 500U // must be even
#else
#define DEFAULT_CAPTURESIZE 100U // must be even
#endif
#endif

#ifdef RECEIVE
// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50
#endif

#ifdef CAPTURE
// This quantity is added to all gaps and subtracted from all marks.
#define IRSENSOR_MARK_EXCESS -10
#endif

#endif // ! CONFIG_H
