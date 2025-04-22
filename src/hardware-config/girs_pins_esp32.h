#pragma once

//Used by M5Capsule
//G15 - Grove SCL
//G13 - Grove SDA
//G10 - Internal SCL
//G8 - Internal SDA
//G40 - I2S CLK
//G41 - I2S DAT
//G4 - IR LED
//G11 - TF CS
//G12 - TF MOSI
//G14 - TF CLK
//G39 - TF MISO
#undef LED_BUILTIN

//#define SIGNAL_LED_1 13
//#define SIGNAL_LED_2 A1
//#define SIGNAL_LED_3 A2
//#define SIGNAL_LED_4 A3
//#define SIGNAL_LED_5 A0
//#define SIGNAL_LED_6 6
//#define SIGNAL_LED_7 7
//#define SIGNAL_LED_8 8

//#define IRSENSOR_1_PIN 3

#define IRRECEIVER_1_PIN G15
//#define IRRECEIVER_2_PIN 12

//Send pin 5?

//Change it to pin 4 for the M5Capsule
//#undef PWM_PIN
//#define PWM_PIN G4
//Actually, G15 for the Grove port
#define PWM_PIN G13

#define STRCPY_PF_CAST(x) (x)