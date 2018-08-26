#ifndef DEFINEMISSINGSTUFF_H
#define	DEFINEMISSINGSTUFF_H

#ifdef IRRECEIVER_1_PULLUP
#define IRRECEIVER_1_PULLUP_VALUE true
#else
#define IRRECEIVER_1_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_2_PULLUP
#define IRRECEIVER_2_PULLUP_VALUE true
#else
#define IRRECEIVER_2_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_3_PULLUP
#define IRRECEIVER_3_PULLUP_VALUE true
#else
#define IRRECEIVER_3_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_4_PULLUP
#define IRRECEIVER_4_PULLUP_VALUE true
#else
#define IRRECEIVER_4_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_5_PULLUP
#define IRRECEIVER_5_PULLUP_VALUE true
#else
#define IRRECEIVER_5_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_6_PULLUP
#define IRRECEIVER_6_PULLUP_VALUE true
#else
#define IRRECEIVER_6_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_7_PULLUP
#define IRRECEIVER_7_PULLUP_VALUE true
#else
#define IRRECEIVER_7_PULLUP_VALUE false
#endif

#ifdef IRRECEIVER_8_PULLUP
#define IRRECEIVER_8_PULLUP_VALUE true
#else
#define IRRECEIVER_8_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_1_PULLUP
#define IRSENSOR_1_PULLUP_VALUE true
#else
#define IRSENSOR_1_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_2_PULLUP
#define IRSENSOR_2_PULLUP_VALUE true
#else
#define IRSENSOR_2_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_3_PULLUP
#define IRSENSOR_3_PULLUP_VALUE true
#else
#define IRSENSOR_3_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_4_PULLUP
#define IRSENSOR_4_PULLUP_VALUE true
#else
#define IRSENSOR_4_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_5_PULLUP
#define IRSENSOR_5_PULLUP_VALUE true
#else
#define IRSENSOR_5_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_6_PULLUP
#define IRSENSOR_6_PULLUP_VALUE true
#else
#define IRSENSOR_6_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_7_PULLUP
#define IRSENSOR_7_PULLUP_VALUE true
#else
#define IRSENSOR_7_PULLUP_VALUE false
#endif

#ifdef IRSENSOR_8_PULLUP
#define IRSENSOR_8_PULLUP_VALUE true
#else
#define IRSENSOR_8_PULLUP_VALUE false
#endif

#ifndef SIGNAL_LED_1
#define SIGNAL_LED_1 invalidPin
#endif

#ifndef SIGNAL_LED_2
#define SIGNAL_LED_2 invalidPin
#endif

#ifndef SIGNAL_LED_3
#define SIGNAL_LED_3 invalidPin
#endif

#ifndef SIGNAL_LED_4
#define SIGNAL_LED_4 invalidPin
#endif

#ifndef SIGNAL_LED_5
#define SIGNAL_LED_5 invalidPin
#endif

#ifndef SIGNAL_LED_6
#define SIGNAL_LED_6 invalidPin
#endif

#ifndef SIGNAL_LED_7
#define SIGNAL_LED_7 invalidPin
#endif

#ifndef SIGNAL_LED_8
#define SIGNAL_LED_8 invalidPin
#endif

#ifndef RECEIVELED
#define receiveled invalidPin
#endif

#ifndef CAPTURELED
#define captureled invalidPin
#endif

#ifndef TRANSMITLED
#define transmitled invalidPin
#endif

#ifndef COMMANDLED
#define commandled invalidPin
#endif

#ifndef UNUSED
#define UNUSED __attribute__ ((unused))
#endif

#ifdef TRANSMIT
#define TRANSMIT_NAME Transmit
#else
#define TRANSMIT_NAME
#endif

#ifdef CAPTURE
#define CAPTURE_NAME Capture
#else
#define CAPTURE_NAME
#endif

#ifdef RECEIVE
#define RECEIVE_NAME Receive
#else
#define RECEIVE_NAME
#endif

#ifdef RENDERER
#define RENDERER_NAME Renderer
#else
#define RENDERER_NAME
#endif

#ifdef DECODER
#define DECODER_NAME Decoder
#else
#define DECODER_NAME
#endif

#ifdef LED
#define LED_NAME Led
#else
#define LED_NAME
#endif

#ifdef LCD
#define LCD_NAME Lcd
#else
#define LCD_NAME
#endif

#ifdef NAMED_COMMANDS
#define NAMED_COMMANDS_NAME NamedCommands
#else
#define NAMED_COMMANDS_NAME
#endif

#ifdef PRONTO
#define PRONTO_NAME Pronto
#else
#define PRONTO_NAME
#endif

#ifdef TRANSMITTERS
#define TRANSMITTERS_NAME Transmitters
#else
#define TRANSMITTERS_NAME
#endif

#ifdef PARAMETERS
#define PARAMETERS_NAME Parameters
#define PARAMETER_CONST
#else
#define PARAMETERS_NAME
#define PARAMETER_CONST const
#endif

#ifdef CONFIGURABLE_LEDS
#define LED_PARAMETER_CONST
#else
#define LED_PARAMETER_CONST const
#endif

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

#ifndef LCD_I2C_ADDRESS
#define LCD_I2C_ADDRESS -1
#endif

#ifndef LCD_WIDTH
#define LCD_WIDTH -1
#endif

#ifndef LCD_HEIGHT
#define LCD_HEIGHT -1
#endif

#endif	/* DEFINEMISSINGSTUFF_H */

