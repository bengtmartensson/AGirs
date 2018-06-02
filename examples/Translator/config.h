// Configuration options

#pragma once

#define DEBUG

// Use LEDs
#define LED

#ifdef LED
//#define DECODELED

// Light this when receive is pending
#define RECEIVELED 2
#define TRANSMITLED 3

#endif // LED

// LCD display with I2C connection.
#define LCD

// If LCD support desired, include appropriate hardware description
#ifdef LCD
#define LCD_0x27_16_2
#endif

#include <girs_hw_config.h>

// Character that ends the command lines
#define EOLCHAR '\n'

#define ENDINGTIMEOUT 35L // milliseconds
#define CAPTURESIZE 200U // must be even
#define BEGINTIMEOUT 100UL // milliseconds
// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50

// Parameters, (not really defaults, but actual values))
#define SERIALBAUD 115200UL
#define SERIALTIMEOUT 5000ULL

#define RECV_NEC1_DEVICE 122
#define RECV_NEC1_SUBDEVICE (255 - 122)
#define RECV_VOLUME_UP_CMD 26
#define RECV_VOLUME_DOWN_CMD 27
#define SEND_DEVICE 0
#define SEND_VOLUME_UP_CMD 16
#define SEND_VOLUME_DOWN_CMD 17