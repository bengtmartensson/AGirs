// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Properties
// Girs modules
#define TRANSMIT
#define RENDERER
#define CAPTURE
#define RECEIVE
#define DECODER
#define PARAMETERS
#define RESET
#define FREEMEM
#define LED
//#define LISTEN

// Light this led when transmission is taking place
#define TRANSMITLED 8

// Light this when receive is pending
#define RECEIVELED 7

// Light this when capture is pending
#define CAPTURELED 6

// Light this when listening for command on the input stream
#define COMMANDLED 5

// Define as an expression that takes MultiDecoder decodes to logical leds.
#define DECODELED(decode) ((decode)+1)

// Other properties
//#define LCD_4BIT
#define LCD_I2C

#define SERIAL_DEBUG
#define ETHERNET

#ifdef ETHERNET
// If SERVER but not USEUDP are defined, runs in "telnet" mode,
// waiting for incoming tcp connections on port PORT.  If none is
// defined, try to initiate tcp connection to PEER_IP, port
// PEER_PORT. In both cases, connection is interrupted when input is
// received.  If USEUDP is defined, use UDP instead. Input is then ignored.

#define SERVER // Wait for incoming connections, for "telnet-ting" to the Arduino
               // If not the Arduino tries to establish a connection to PEER_IP at port PEER_PORT
// do NOT use UDP as preprocessor symbol, it makes Ethernet.h go berserk
//#define USEUDP
//#define ETHER_W5100 // Use normal Arduino Ethernet library, usin W5100 and pin 10
//#define SDCARD_ON_ETHERSHIELD // there is a pin4 to be selected low
//#define ETHER_ENC28J60 // Cheapie ENC 28J60 chip, not yet supported
#define ETHERNET_SESSION // multi-command session, otherwise close after one command
//#define DHCP
#endif // ETHERNET

// Hardware configuration

// Include one file describing the pin configuration
//#include <girs_pins_lcdshield.h>
//#include <girs_pins_nano_shield.h>
#include <girs_pins.h> // Generic

// If LCD support desired, include appropriate hardware description
#ifdef LCD_I2C
//#include <lcd_0x27_16_2.h>
#include <lcd_0x3F_20_4.h>
#endif
#ifdef LCD_4BIT
#include <lcd_4bit_16_2.h>
#endif

#ifdef ETHERNET
#ifndef SERVER
#define PEER_IP 192,168,1,3
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

#endif // ! CONFIG_H
