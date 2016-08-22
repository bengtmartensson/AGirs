// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Define for pure listener
#define LISTENER

// Displayed in greeting messages
//#define PROGNAME "Listener"

// Receive demodulated IR signals, require demodulating IR sensor
#define RECEIVE

// Use LEDs
#define LED

#ifdef LED
#define DECODELED

// Light this when receive is pending
#define RECEIVELED 1
#define COMMANDLED 5
#endif // LED

// LCD display with I2C connection.
#define LCD

// Define if using Ethernet (TCP or UDP) as the communication channel
#define ETHERNET

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
//#define SERVER
// NOTE: do NOT use UDP as preprocessor symbol, it makes Ethernet.h go haywire.
//#define USEUDP
#define SDCARD_ON_ETHERSHIELD // there is a pin4 to be selected low

#define SDCARD_ON_ETHERSHIELD_PIN 4 // there is a pin4 to be selected low
#define DHCP // Consumes rather much memory

// Invoke the AMX style beacon, as per https://github.com/bengtmartensson/ABeacon
//#define BEACON

#endif // ETHERNET

// Hardware configuration
// Include one file describing the pin configuration
// Use one in GirsLib, or write your own.
#ifdef ARDUINO

#include <girs_pins.h> // Generic

#else // ! ARDUINO
// For compiling a version to test on the PC, not to flash onto the Arduino.
#include <girs_pins_dummy.h>
#endif // ! ARDUINO

// If LCD support desired, include appropriate hardware description
#ifdef LCD
//#include <lcd_0x27_16_2.h>
#include <lcd_0x27_20_4.h>
#endif

// Character that ends the command lines
#define EOLCHAR '\n'

#ifdef ETHERNET
#ifdef USEUDP
#define BROADCAST_IP 192,168,1,3
#define BROADCAST_PORT 55555
#else // !USEUDP
#ifndef SERVER
// Host the program tries to contact
#define PEER_IP 192,168,1,3

// Port to be listened on
#define PEER_PORT 44444
#endif
#endif // !USEUDP

#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // ! DHCP
#define PORT       44444
#endif // ETHERNET

#define DEFAULT_RECEIVE_ENDINGTIMEOUT 35L // milliseconds
#define DEFAULT_CAPTURESIZE 200U // must be even
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50

#if !defined(ETHERNET) | defined(SERIAL_DEBUG)
// Parameters, (not really defaults, but actual values))
#define SERIALBAUD 115200UL
#define SERIALTIMEOUT 5000ULL
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)

#endif // ! CONFIG_H
