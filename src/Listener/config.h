// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Properties, needed for the *_pins.h files
#define RECEIVE
#define DECODER
#define LED
#define LCD

// Using Ethernet for communication
//#define ETHERNET

// Parameters, (not really defaults, but actual values))
#ifndef ETHERNET
#define SERIALBAUD 115200
#define SERIALTIMEOUT 5000L
#endif // !defined ETHERNET

#define DEFAULT_RECEIVE_ENDINGTIMEOUT 35L // milliseconds
#define DEFAULT_CAPTURESIZE 200U // must be even

// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50U


#ifdef ETHERNET

// Issues some debug messages on the serial port
//#define SERIAL_DEBUG

// If SERVER but not USEUDP are defined, the progam runs in "telnet" mode,
// waiting for incoming tcp connections on port PORT.  If none is
// defined, try to initiate tcp connection to PEER_IP, port
// PEER_PORT. In both cases, connection is interrupted when input is
// received.  If USEUDP is defined, use UDP instead. Input is then ignored.

#define SERVER // Wait for incoming connections, for "telnet-ting" to the Arduino
               // If not the Arduino tries to establish a connection to PEER_IP at port PEER_PORT
// NOTE: do NOT use UDP as preprocessor symbol, it makes Ethernet.h go haywire.
//#define USEUDP
//#define ETHER_W5100 // Use normal Arduino Ethernet library, usin W5100 and pin 10
//#define SDCARD_ON_ETHERSHIELD // there is a pin4 to be selected low
//#define ETHER_ENC28J60 // Cheapie ENC 28J60 chip, not yet supported
#define ETHERNET_SESSION // multi-command session, otherwise close after one command
//#define DHCP // Consumes rather much memory

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

// Hardware configuration
// Include one file describing the pin configuration
#ifdef ARDUINO
//#include <girs_pins_lcdshield.h>
#include <girs_pins_nano_shield.h>
//#include <girs_pins_nano_alt.h>
//#include <girs_pins.h> // Generic
#else // !  ARDUINO
#include <girs_pins_dummy.h>
#endif

// If LCD support desired, include appropriate hardware description
#ifdef LCD
#ifdef ARDUINO_AVR_NANO
#include <lcd_0x27_16_2.h>
#else
#include <lcd_0x3F_20_4.h>
#endif
#endif

#endif // ! CONFIG_H
