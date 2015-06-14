// Configuration options

#ifndef CONFIG_H
#define CONFIG_H

// Properties
//#define TRANSMIT
//#define RENDERER
//#define CAPTURE
#define RECEIVE
#define DECODER
//#define PARAMETERS
//#define RESET
//#define FREEMEM
#define LED
//#define LCD_4BIT
#define LCD_I2C
//#define ETHERNET
//#define ETHER_W5100
//#define SDCARD_ON_ETHERSHIELD
//#define ETHER_ENC28J60
//#define ETHERNET_SESSION
//#define DHCP

// Hardware configuration

// Include file with pin configuration
//#include <girs_pins.h>
//#include <girs_pins_lcdshield.h>
//#include <girs_pins_nano_shield.h>
#include <girs_pins.h>

// If LCD support desired, include appropriate include
#ifdef LCD_I2C
//#include <lcd_0x27_16_2.h>
#include <lcd_0x3F_20_4.h>
#endif
#ifdef LCD_4BIT
#include <lcd_4bit_16_2.h>
#endif

#ifdef ETHERNET
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

