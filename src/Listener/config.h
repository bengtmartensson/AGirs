// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Use LEDs
#define LED

#ifdef LED
#define DECODELED

// Light this when receive is pending
#define RECEIVELED 1
#endif // LED

// LCD display with I2C connection.
#define LCD

// Define if using Ethernet (TCP or UDP) as the communication channel
#define ETHERNET

#ifdef ETHERNET

#define USEUDP
#define SDCARD_ON_ETHERSHIELD // there is a pin4 to be selected low

#define SDCARD_ON_ETHERSHIELD_PIN 4 // there is a pin4 to be selected low
#define DHCP // Consumes rather much memory

// Invoke the AMX style beacon, as per https://github.com/bengtmartensson/ABeacon
#define BEACON

#endif // ETHERNET

// Hardware configuration
// Include one file describing the pin configuration
// Use one in GirsLib, or write your own.
#define RECEIVE // necessary for the hardware include files
#include <girs_pins.h> // Generic

// If LCD support desired, include appropriate hardware description
#ifdef LCD
#include <lcd_0x27_16_2.h>
//#include <lcd_0x27_20_4.h>
#endif

// Character that ends the command lines
#define EOLCHAR '\n'

#ifdef ETHERNET
#ifdef USEUDP
#define BROADCAST_IP 192,168,1,3
#define BROADCAST_PORT 55555
#else // !USEUDP
#error only UDP supported
#endif // !USEUDP

#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // ! DHCP
#endif // ETHERNET

#define DEFAULT_RECEIVE_ENDINGTIMEOUT 35L // milliseconds
#define DEFAULT_CAPTURESIZE 200U // must be even
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds
// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50

#ifndef ETHERNET
// Parameters, (not really defaults, but actual values))
#define SERIALBAUD 115200UL
#define SERIALTIMEOUT 5000ULL
#endif // ! ETHERNET

#endif // ! CONFIG_H
