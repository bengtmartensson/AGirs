#ifndef _GIRS_MACROS_H
#define _GIRS_MACROS_H

#if defined(LCD) & defined(LCD_I2C)
#error Both LCD and LCD_I2C cannot be defined
#endif

#define DEFINE_IRRECEIVER DEFINE_IRRECEIVER_GND DEFINE_IRRECEIVER_VSS
#ifdef IRRECEIVER_GND
#define DEFINE_IRRECEIVER_GND \
    pinMode(IRRECEIVER_GND, OUTPUT); \
    digitalWrite(IRRECEIVER_GND, LOW);
#else
#define DEFINE_IRRECEIVER_GND
#endif

#ifdef IRRECEIVER_VSS
#define DEFINE_IRRECEIVER_VSS \
    pinMode(IRRECEIVER_VSS, OUTPUT); \
    digitalWrite(IRRECEIVER_VSS, HIGH);
#else
#define DEFINE_IRRECEIVER_VSS
#endif

#define DEFINE_IRSENSOR DEFINE_IRSENSOR_GND DEFINE_IRSENSOR_VSS
#ifdef SENSOR_GND
#define DEFINE_IRSENSOR_GND \
     pinMode(SENSOR_GND, OUTPUT); \
     digitalWrite(SENSOR_GND, LOW);
#else
#define DEFINE_IRSENSOR_GND 
#endif

#ifdef SENSOR_VSS
#define DEFINE_IRSENSOR_VSS \
     pinMode(SENSOR_VSS, OUTPUT); \
     digitalWrite(SENSOR_VSS, HIGH);
#else
#define DEFINE_IRSENSOR_VSS
#endif


#define DEFINE_LED_1 DEFINE_LED_PIN_1 DEFINE_LED_GND_1

#ifdef SIGNAL_LED_1
#define DEFINE_LED_PIN_1 \
     pinMode(SIGNAL_LED_1, OUTPUT); \
     digitalWrite(SIGNAL_LED_1, LOW);
#else
#define DEFINE_LED_PIN_1
#endif

#ifdef SIGNAL_LED_1_GND
#define DEFINE_LED_GND_1 \
     pinMode(SIGNAL_LED_1_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_1_GND, LOW);
#else
#define DEFINE_LED_GND_1
#endif

#define DEFINE_LED_2 DEFINE_LED_PIN_2 DEFINE_LED_GND_2

#ifdef SIGNAL_LED_2
#define DEFINE_LED_PIN_2 \
     pinMode(SIGNAL_LED_2, OUTPUT); \
     digitalWrite(SIGNAL_LED_2, LOW);
#else
#define DEFINE_LED_PIN_2
#endif

#ifdef SIGNAL_LED_2_GND
#define DEFINE_LED_GND_2 \
     pinMode(SIGNAL_LED_2_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_2_GND, LOW);
#else
#define DEFINE_LED_GND_2
#endif

#define DEFINE_LED_3 DEFINE_LED_PIN_3 DEFINE_LED_GND_3

#ifdef SIGNAL_LED_3
#define DEFINE_LED_PIN_3 \
     pinMode(SIGNAL_LED_3, OUTPUT); \
     digitalWrite(SIGNAL_LED_3, LOW);
#else
#define DEFINE_LED_PIN_3
#endif

#ifdef SIGNAL_LED_3_GND
#define DEFINE_LED_GND_3 \
     pinMode(SIGNAL_LED_3_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_3_GND, LOW);
#else
#define DEFINE_LED_GND_3
#endif

#define DEFINE_LED_4 DEFINE_LED_PIN_4 DEFINE_LED_GND_4

#ifdef SIGNAL_LED_4
#define DEFINE_LED_PIN_4 \
     pinMode(SIGNAL_LED_4, OUTPUT); \
     digitalWrite(SIGNAL_LED_4, LOW);
#else
#define DEFINE_LED_PIN_4
#endif

#ifdef SIGNAL_LED_4_GND
#define DEFINE_LED_GND_4 \
     pinMode(SIGNAL_LED_4_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_4_GND, LOW);
#else
#define DEFINE_LED_GND_4
#endif

#define DEFINE_LED_5 DEFINE_LED_PIN_5 DEFINE_LED_GND_5

#ifdef SIGNAL_LED_5
#define DEFINE_LED_PIN_5 \
     pinMode(SIGNAL_LED_5, OUTPUT); \
     digitalWrite(SIGNAL_LED_5, LOW);
#else
#define DEFINE_LED_PIN_5
#endif

#ifdef SIGNAL_LED_5_GND
#define DEFINE_LED_GND_5 \
     pinMode(SIGNAL_LED_5_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_5_GND, LOW);
#else
#define DEFINE_LED_GND_5
#endif

#define DEFINE_LED_6 DEFINE_LED_PIN_6 DEFINE_LED_GND_6

#ifdef SIGNAL_LED_6
#define DEFINE_LED_PIN_6 \
     pinMode(SIGNAL_LED_6, OUTPUT); \
     digitalWrite(SIGNAL_LED_6, LOW);
#else
#define DEFINE_LED_PIN_6
#endif

#ifdef SIGNAL_LED_6_GND
#define DEFINE_LED_GND_6 \
     pinMode(SIGNAL_LED_6_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_6_GND, LOW);
#else
#define DEFINE_LED_GND_6
#endif

#define DEFINE_LED_7 DEFINE_LED_PIN_7 DEFINE_LED_GND_7

#ifdef SIGNAL_LED_7
#define DEFINE_LED_PIN_7 \
     pinMode(SIGNAL_LED_7, OUTPUT); \
     digitalWrite(SIGNAL_LED_7, LOW);
#else
#define DEFINE_LED_PIN_7
#endif

#ifdef SIGNAL_LED_7_GND
#define DEFINE_LED_GND_7 \
     pinMode(SIGNAL_LED_7_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_7_GND, LOW);
#else
#define DEFINE_LED_GND_7
#endif

#define DEFINE_LED_8 DEFINE_LED_PIN_8 DEFINE_LED_GND_8

#ifdef SIGNAL_LED_8
#define DEFINE_LED_PIN_8 \
     pinMode(SIGNAL_LED_8, OUTPUT); \
     digitalWrite(SIGNAL_LED_8, LOW);
#else
#define DEFINE_LED_PIN_8
#endif

#ifdef SIGNAL_LED_8_GND
#define DEFINE_LED_GND_8 \
     pinMode(SIGNAL_LED_8_GND, OUTPUT); \
     digitalWrite(SIGNAL_LED_8_GND, LOW);
#else
#define DEFINE_LED_GND_8
#endif

#define DEFINE_LEDS \
  DEFINE_LED_1 DEFINE_LED_2 DEFINE_LED_3 DEFINE_LED_4	\
  DEFINE_LED_5 DEFINE_LED_6 DEFINE_LED_7 DEFINE_LED_8

#endif

#ifdef SIGNAL_LED_1
#define SIGNAL_LED_1_OFF  digitalWrite(SIGNAL_LED_1, LOW)
#define BLINK_LED_1(f) f(SIGNAL_LED_1)
#else
#define SIGNAL_LED_1_OFF
#define BLINK_LED_1(f)
#endif
#ifdef SIGNAL_LED_2
#define SIGNAL_LED_2_OFF  digitalWrite(SIGNAL_LED_2, LOW)
#define BLINK_LED_2(f) f(SIGNAL_LED_2)
#else
#define SIGNAL_LED_2_OFF
#define BLINK_LED_2(f)
#endif
#ifdef SIGNAL_LED_3
#define SIGNAL_LED_3_OFF  digitalWrite(SIGNAL_LED_3, LOW)
#define BLINK_LED_3(f) f(SIGNAL_LED_3)
#else
#define SIGNAL_LED_3_OFF
#define BLINK_LED_3(f)
#endif
#ifdef SIGNAL_LED_4
#define SIGNAL_LED_4_OFF  digitalWrite(SIGNAL_LED_4, LOW)
#define BLINK_LED_4(f) f(SIGNAL_LED_4)
#else
#define SIGNAL_LED_4_OFF
#define BLINK_LED_4(f)
#endif
#ifdef SIGNAL_LED_5
#define SIGNAL_LED_5_OFF  digitalWrite(SIGNAL_LED_5, LOW)
#define BLINK_LED_5(f) f(SIGNAL_LED_5)
#else
#define SIGNAL_LED_5_OFF
#define BLINK_LED_5(f)
#endif
#ifdef SIGNAL_LED_6
#define SIGNAL_LED_6_OFF  digitalWrite(SIGNAL_LED_6, LOW)
#define BLINK_LED_6(f) f(SIGNAL_LED_6)
#else
#define SIGNAL_LED_6_OFF
#define BLINK_LED_6(f)
#endif
#ifdef SIGNAL_LED_7
#define SIGNAL_LED_7_OFF  digitalWrite(SIGNAL_LED_7, LOW)
#define BLINK_LED_7(f) f(SIGNAL_LED_7)
#else
#define SIGNAL_LED_7_OFF
#define BLINK_LED_7(f)
#endif
#ifdef SIGNAL_LED_8
#define SIGNAL_LED_8_OFF  digitalWrite(SIGNAL_LED_8, LOW)
#define BLINK_LED_8(f) f(SIGNAL_LED_8)
#else
#define SIGNAL_LED_8_OFF
#define BLINK_LED_8(f)
#endif

#define ALL_LEDS_OFF \
  SIGNAL_LED_1_OFF; SIGNAL_LED_2_OFF; SIGNAL_LED_3_OFF; SIGNAL_LED_4_OFF; \
  SIGNAL_LED_5_OFF; SIGNAL_LED_6_OFF; SIGNAL_LED_7_OFF; SIGNAL_LED_8_OFF

#define ALL_LEDS_ON(f) \
  BLINK_LED_1(f); BLINK_LED_2(f); BLINK_LED_3(f); BLINK_LED_4(f); \
  BLINK_LED_5(f); BLINK_LED_6(f); BLINK_LED_7(f); BLINK_LED_8(f)

#ifdef  LCD_I2C
#define LCD_OFF(lcd) lcd.noDisplay(); lcd.noBacklight()
#elif defined(LCD)
#define LCD_OFF(lcd) lcd.noDisplay(); digitalWrite(LCD_BACKLIGHT_PIN, LOW)
#else
#define LCD_OFF(lcd)
#endif
