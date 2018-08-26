/**
 * @file GirsLite.config.h
 * @brief Configuration optimized for IrScrutinizer and Lirc.
 */

#pragma once

/**
 * Program name, as displayed in greeting messages etc.
 */
#define PROGNAME "GirsLite"

// Define Girs modules to implement, see http://www.harctoolbox.org/Girs.html

/**
 * Define to support transmission of IR signals. Requires IR LEDs.
 */
#define TRANSMIT

/**
 * Define to support the rendering of some known protocols, presently NEC1 and RC5.
 */
//#define RENDERER

/**
 * Define to support capturing of IR signals.
 * Requires non-demodulating IR sensor (TSMP58000, QSE159, etc).
 */
#define CAPTURE

/**
 * Define to support reception of demodulated IR signals.
 * Require demodulating IR sensor (TSMP* or equivalent).
 */
#define RECEIVE

/**
 * Define to support decoding of the received signal.
 */
//#define DECODER
/**
 * Define to support blinking with LED according to decoding outcome.
 */
//#define DECODELED

/**
 * Define to support an LCD display with I2C connection. Defines a command "lcd".
 */
//#define LCD

/**
 * Define to use signaling LEDs. Defines command "led".
 */
#define LED

/**
 * Define to allow to change the parameters, like timeouts.
 * Defines command "parameters". The Lirc driver uses this, if available.
 */
#define PARAMETERS

/**
 * Define to support commands called by names, grouped in sets, "remotes".
 * (Not yet implemented.)
 */
#ifdef DOXYGEN
#define NAMED_COMMANDS
#endif
//#define NAMED_COMMANDS

/**
 * Define to have the receive command report the durations, even if DECODE is defined
 * and a decode was found.
 * Use this for Lirc.
 */
//#define DONT_REPORT_DECODES

/**
 * Define to be able to reconfigure signaling LEDs during runtime.
 * Have parameters for transmitled etc.
 */
//#define CONFIGURABLE_LEDS

/**
 * Define to support sending signals without modulation, e.g. with RF module.
 */
//#define NON_MOD

/**
 * Define to support a reset command, Experimental!!
 */
//#define RESET

/**
 * Define to support command "memory", reporting free (SRAM) memory.
 */
//#define FREEMEM

/**
 * Define to support command "info".
 */
//#define INFO

/**
 * Define to support command "hex", sending Pronto hex signals.
 */
//#define PRONTO

#ifdef LED

#ifdef TRANSMIT
/**
 * Number of logical LED to light when transmission takings place.
 */
#define TRANSMITLED 4
#endif

#ifdef RECEIVE
/**
 * Number of logical LED to light when waiting for reception.
 */
#define RECEIVELED 3
#endif

#ifdef CAPTURE
/**
 * Number of logical LED to light when waiting for capture.
 */
#define CAPTURELED 3
#endif

/**
 * Number of logical LED to light when waiting for command.
 */
#define COMMANDLED 2

#endif // LED


/**
 * Character that ends the command lines.
 */
#define EOLCHAR '\r'

// Hardware configuration

// Define an LCD symbol to use, if desired.
#ifdef LCD

/**
 * Define to use LCD display at I2C address 0x3F,
 * 4 lines, 20 columns.
 */
#define LCD_0x3F_20_4

/**
 * Define to use LCD display at I2C address 0x27,
 * 4 lines, 20 columns.
 */
#ifdef DOXYGEN
#define LCD_0x27_20_4
#endif
//#define LCD_0x27_20_4

/**
 * Define to use LCD display at I2C address 0x27,
 * 2 lines, 16 columns.
 */
#ifdef DOXYGEN
#define LCD_0x27_16_2
#endif
//#define LCD_0x27_16_2

#endif // LCD

// Include one file describing the pin configuration
// Use one of the provided, or write your own.
#include <girs_hw_config.h> // Generic

// Without PARAMETERS, these are really not defaults,
// they are the non-changeable values.
/**
 * Time to wait for signal to start.
 */
#define DEFAULT_BEGINTIMEOUT 10000UL // milliseconds

/**
 * By receive, time to wait before signal is considered ended.
 * If using the decoder, be sure to end a capture before the repeat sequence.
 */
#define DEFAULT_RECEIVE_ENDINGTIMEOUT 50L // milliseconds
#ifdef CAPTURE
/**
 * By capture, time to wait before signal is considered ended.
 */
#define DEFAULT_CAPTURE_ENDINGTIMEOUT 100L // milliseconds
#endif

/**
 * Size of capture and receive arrays.
 */
#define DEFAULT_CAPTURESIZE 400U // must be even

#ifdef RECEIVE
/**
 * This quantity is added to all gaps and subtracted from all marks when receiving.
 */
#define IRRECEIVER_MARK_EXCESS 50
#endif

#ifdef CAPTURE
/**
 * This quantity is added to all gaps and subtracted from all marks when capturing.
 */
#define IRSENSOR_MARK_EXCESS -10
#endif

#if !defined(ETHERNET) | defined(SERIAL_DEBUG)
/**
 * Configured speed of the serial port.
 */
#define SERIALBAUD 115200

/**
 * Timeout for serial line.
 */
#define SERIALTIMEOUT 5000L
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)
