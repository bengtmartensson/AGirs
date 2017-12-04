// Configuration options

#pragma once

// LCD display with I2C connection.
#define LCD

// Hardware configuration

#define LCD_0x27_16_2

// Include one file describing the pin configuration
#include "girs_hw_config.h"

#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds

#define serialBaud 115200
#define serialTimeout 5000L

// Must split a NEC1 between the payload and the repeat
#define DEFAULT_ENDINGTIMEOUT 35L // milliseconds

// This quantity is added to all gaps and subtracted from all marks.
#define IRRECEIVER_MARK_EXCESS 50U

// Remote the thing is reacting to
#define selectedD 108 //114
#define selectedS 104 //205
