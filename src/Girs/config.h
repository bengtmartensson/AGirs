// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Define for IrScrutinizer optimized version
//#define GIRSLITE

// GirsLitePlus is LCD and NON_MOD added tp GirsLite.
//#define GIRSLITEPLUS

// Define for Lirc optimized version
//#define GIRS4LIRC

// Define for pure listener (not yet implemented)
//#define LISTENER

// Define Girs modules to implement, see http://www.harctoolbox.org/Girs.html
#ifdef GIRSLITE

#define PROGNAME "GirsLite"
#define TRANSMIT
#define CAPTURE
#define RECEIVE
#define LED
//#define FREEMEM

// Light this led when transmission is taking place
#define TRANSMITLED 4

// Light this when receive is pending
#define RECEIVELED 3

// Light this when capture is pending
#define CAPTURELED 3

// Light this when listening for command on the input stream
#define COMMANDLED 2

#elif defined(GIRSLITEPLUS)

#define PROGNAME "GirsLite+"
#define TRANSMIT
#define RECEIVE
#define CAPTURE
#define LED
#define LCD
#define FREEMEM
// Support sending signals without modulation, e.g. with RF module.
#define NON_MOD

#define TRANSMITLED 4
#define RECEIVELED  3
#define CAPTURELED  2
#define COMMANDLED  1

#elif defined(GIRS4LIRC)

#define PROGNAME "Girs4Lirc"
#define TRANSMIT
#define RECEIVE
#define LED
#define LCD

#elif defined(LISTENER)

#error LISTENER not yet implemented
#define PROGNAME Listener

#else

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

// Light this led when transmission is taking place
#define TRANSMITLED 8
//#define TRANSMITLED 4

// Light this when receive is pending
#define RECEIVELED 7
//#define RECEIVELED 3

// Light this when capture is pending
#define CAPTURELED 6
//#define CAPTURELED 2

// Light this when listening for command on the input stream
#define COMMANDLED 5
//#define COMMANDLED 1
#endif

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
#ifdef ARDUINO_AVR_MEGA2560
#ifdef ETHERNET
#include <girs_pins_mega2560_rear.h>
#else
#include <girs_pins_mega2560.h>
#endif
#elif defined(ARDUINO_AVR_NANO) // ! ARDUINO_AVR_MEGA2560

#include <girs_pins_nano.h>
//#include <girs_pins_nano_lcd.h>
//#include <girs_pins_nano_shield.h>

#elif defined(ARDUINO)

#include <girs_pins.h> // Generic
#else // ! ARDUINO

#include <girs_pins_dummy.h>

#endif

// If LCD support desired, include appropriate hardware description
#ifdef LCD
#if defined(ARDUINO_AVR_NANO)
#include <lcd_0x27_16_2.h>
#else  // ! ARDUINO_AVR_NANO
#ifdef ETHERNET
#include <lcd_0x3F_20_4.h>
#else  // ! ETHERNET
#include <lcd_0x27_20_4.h>
//#include <lcd_0x3F_20_4.h>
//#include <lcd_0x27_16_2.h>
#endif // ! ETHERNET
#endif // ! ARDUINO_AVR_NANO
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
