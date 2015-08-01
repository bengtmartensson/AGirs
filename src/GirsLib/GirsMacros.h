#ifndef _GIRS_MACROS_H
#define _GIRS_MACROS_H

#if defined(LED_DEBUG) & ! defined(LED)
#error Cannot define LED_DEBUG without defining LED
#endif

#if defined(LCD_4BIT) & defined(LCD_I2C)
#error Both LCD_4BIT and LCD_I2C cannot be defined
#endif

#if defined(LCD_4BIT) | defined(LCD_I2C)
#define LCD
#endif

#ifdef LCD_4BIT

#define LCD_DEFINE(lcd) LiquidCrystal lcd(LCD_INIT_ARGS)
#ifdef LCD_BACKLIGHT_SILLY
// See http://forum.arduino.cc/index.php?topic=96747.0
#define LCD_BACKLIGHT_ON(lcd) pinMode(LCD_BACKLIGHT_PIN, INPUT_PULLUP)
/*
//#define LCD_BACKLIGHT_ON(lcd)
//#define LCD_INIT(lcd) pinMode(LCD_BACKLIGHT_PIN, OUTPUT);	\
//  digitalWrite(LCD_BACKLIGHT_PIN, LOW);			\
//  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
 * */
#define LCD_INIT(lcd)  lcd.begin(LCD_WIDTH, LCD_HEIGHT)
#define LCD_OFF(lcd) lcd.noDisplay(); pinMode(LCD_BACKLIGHT_PIN, OUTPUT); digitalWrite(LCD_BACKLIGHT_PIN, LOW)
//#define LCD_OFF(lcd) lcd.noDisplay()
#else // !LCD_BACKLIGHT_SILLY
#define LCD_BACKLIGHT_ON(lcd) digitalWrite(LCD_BACKLIGHT_PIN, HIGH)
#define LCD_INIT(lcd) pinMode(LCD_BACKLIGHT_PIN, OUTPUT); \
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
#define LCD_OFF(lcd) lcd.noDisplay(); digitalWrite(LCD_BACKLIGHT_PIN, LOW)
#endif // !LCD_BACKLIGHT_SILLY

#elif defined(LCD_I2C) // !LCD_4BIT

#define LCD_DEFINE(lcd) LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT)
#define LCD_BACKLIGHT_ON(lcd) lcd.backlight()
#define LCD_INIT(lcd) lcd.begin()
#define LCD_OFF(lcd) lcd.noDisplay(); lcd.noBacklight()

#else   // ! defined(LCD_I2C)

#define LCD_DEFINE(lcd)
#define LCD_BACKLIGHT_ON(lcd)
#define LCD_INIT(lcd)
#define LCD_OFF(lcd)

#endif  // ! defined(LCD_I2C)

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

#ifdef RECEIVE
#ifdef IRRECEIVER_1_PIN
#define TERNARY_RECEIVER_1(receiverNo)             (receiverNo) == 1 ? IRRECEIVER_1_PIN :
#else
#define TERNARY_RECEIVER_1(receiverNo)
#endif
#ifdef IRRECEIVER_2_PIN
#define TERNARY_RECEIVER_2(receiverNo)         (receiverNo) == 2 ? IRRECEIVER_2_PIN :
#else
#define TERNARY_RECEIVER_2(receiverNo)
#endif
#ifdef IRRECEIVER_3_PIN
#define TERNARY_RECEIVER_3(receiverNo)         (receiverNo) == 3 ? IRRECEIVER_3_PIN :
#else
#define TERNARY_RECEIVER_3(receiverNo)
#endif
#ifdef IRRECEIVER_4_PIN
#define TERNARY_RECEIVER_4(receiverNo)           (receiverNo) == 4 ? IRRECEIVER_4_PIN :
#else
#define TERNARY_RECEIVER_4(receiverNo)
#endif
#ifdef IRRECEIVER_5_PIN
#define TERNARY_RECEIVER_5(receiverNo)               (receiverNo) == 5 ? IRRECEIVER_5_PIN :
#else
#define TERNARY_RECEIVER_5(receiverNo)
#endif
#ifdef IRRECEIVER_6_PIN
#define TERNARY_RECEIVER_6(receiverNo)               (receiverNo) == 6 ? IRRECEIVER_6_PIN :
#else
#define TERNARY_RECEIVER_6(receiverNo)
#endif
#ifdef IRRECEIVER_7_PIN
#define TERNARY_RECEIVER_7(receiverNo)               (receiverNo) == 7 ? IRRECEIVER_7_PIN :
#else
#define TERNARY_RECEIVER_7(receiverNo)
#endif
#ifdef IRRECEIVER_8_PIN
#define TERNARY_RECEIVER_8(receiverNo)               (receiverNo) == 8 ? IRRECEIVER_8_PIN :
#else
#define TERNARY_RECEIVER_8(receiverNo)
#endif

#define RECEIVER2PIN(no) ( \
	      TERNARY_RECEIVER_1(no) \
              TERNARY_RECEIVER_2(no) \
              TERNARY_RECEIVER_3(no) \
              TERNARY_RECEIVER_4(no) \
              TERNARY_RECEIVER_5(no) \
              TERNARY_RECEIVER_6(no) \
              TERNARY_RECEIVER_7(no) \
              TERNARY_RECEIVER_8(no) \
              -1)

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

#define IRRECEIVER_PULLUP_VALUE(receiverNo) ( \
              (receiverNo) == 1 ? IRRECEIVER_1_PULLUP_VALUE : \
              (receiverNo) == 2 ? IRRECEIVER_2_PULLUP_VALUE : \
              (receiverNo) == 3 ? IRRECEIVER_3_PULLUP_VALUE : \
              (receiverNo) == 4 ? IRRECEIVER_4_PULLUP_VALUE : \
              (receiverNo) == 5 ? IRRECEIVER_5_PULLUP_VALUE : \
              (receiverNo) == 6 ? IRRECEIVER_6_PULLUP_VALUE : \
              (receiverNo) == 7 ? IRRECEIVER_7_PULLUP_VALUE : \
              (receiverNo) == 8 ? IRRECEIVER_8_PULLUP_VALUE : \
                  -1)

#endif // RECEIVE

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

#ifdef IRSENSOR_PIN_PULLUP
#define IRSENSOR_PIN_PULLUP_VALUE true
#else
#define IRSENSOR_PIN_PULLUP_VALUE false
#endif

#define MAX_LED 8

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

#ifdef SIGNAL_LED_1
#define SIGNAL_LED_1_OFF  digitalWrite(SIGNAL_LED_1, LOW)
#define BLINK_LED_1(f) f(SIGNAL_LED_1)
#define TERNARY_LED_1(led_no)  (led_no) == 1 ? SIGNAL_LED_1 :
#else
#define SIGNAL_LED_1_OFF
#define BLINK_LED_1(f)
#define TERNARY_LED_1(led_no)
#endif
#ifdef SIGNAL_LED_2
#define SIGNAL_LED_2_OFF  digitalWrite(SIGNAL_LED_2, LOW)
#define BLINK_LED_2(f) f(SIGNAL_LED_2)
#define TERNARY_LED_2(led_no)  (led_no) == 2 ? SIGNAL_LED_2 :
#else
#define SIGNAL_LED_2_OFF
#define BLINK_LED_2(f)
#define TERNARY_LED_2(led_no)
#endif
#ifdef SIGNAL_LED_3
#define SIGNAL_LED_3_OFF  digitalWrite(SIGNAL_LED_3, LOW)
#define BLINK_LED_3(f) f(SIGNAL_LED_3)
#define TERNARY_LED_3(led_no)  (led_no) == 3 ? SIGNAL_LED_3 :
#else
#define SIGNAL_LED_3_OFF
#define BLINK_LED_3(f)
#define TERNARY_LED_3(led_no)
#endif
#ifdef SIGNAL_LED_4
#define SIGNAL_LED_4_OFF  digitalWrite(SIGNAL_LED_4, LOW)
#define BLINK_LED_4(f) f(SIGNAL_LED_4)
#define TERNARY_LED_4(led_no)  (led_no) == 4 ? SIGNAL_LED_4 :
#else
#define SIGNAL_LED_4_OFF
#define BLINK_LED_4(f)
#define TERNARY_LED_4(led_no)
#endif
#ifdef SIGNAL_LED_5
#define SIGNAL_LED_5_OFF  digitalWrite(SIGNAL_LED_5, LOW)
#define BLINK_LED_5(f) f(SIGNAL_LED_5)
#define TERNARY_LED_5(led_no)  (led_no) == 5 ? SIGNAL_LED_5 :
#else
#define SIGNAL_LED_5_OFF
#define BLINK_LED_5(f)
#define TERNARY_LED_5(led_no)
#endif
#ifdef SIGNAL_LED_6
#define SIGNAL_LED_6_OFF  digitalWrite(SIGNAL_LED_6, LOW)
#define BLINK_LED_6(f) f(SIGNAL_LED_6)
#define TERNARY_LED_6(led_no)  (led_no) == 6 ? SIGNAL_LED_6 :
#else
#define SIGNAL_LED_6_OFF
#define BLINK_LED_6(f)
#define TERNARY_LED_6(led_no)
#endif
#ifdef SIGNAL_LED_7
#define SIGNAL_LED_7_OFF  digitalWrite(SIGNAL_LED_7, LOW)
#define BLINK_LED_7(f) f(SIGNAL_LED_7)
#define TERNARY_LED_7(led_no)  (led_no) == 7 ? SIGNAL_LED_7 :
#else
#define SIGNAL_LED_7_OFF
#define BLINK_LED_7(f)
#define TERNARY_LED_7(led_no)
#endif
#ifdef SIGNAL_LED_8
#define SIGNAL_LED_8_OFF  digitalWrite(SIGNAL_LED_8, LOW)
#define BLINK_LED_8(f) f(SIGNAL_LED_8)
#define TERNARY_LED_8(led_no)  (led_no) == 8 ? SIGNAL_LED_8 :
#else
#define SIGNAL_LED_8_OFF
#define BLINK_LED_8(f)
#define TERNARY_LED_8(led_no)
#endif

#define ALL_LEDS_OFF \
  SIGNAL_LED_1_OFF; SIGNAL_LED_2_OFF; SIGNAL_LED_3_OFF; SIGNAL_LED_4_OFF; \
  SIGNAL_LED_5_OFF; SIGNAL_LED_6_OFF; SIGNAL_LED_7_OFF; SIGNAL_LED_8_OFF

#define BLINK_ALL_LEDS \
  BLINK_LED_1(blinkAck); BLINK_LED_2(blinkAck); BLINK_LED_3(blinkAck); BLINK_LED_4(blinkAck); \
  BLINK_LED_5(blinkAck); BLINK_LED_6(blinkAck); BLINK_LED_7(blinkAck); BLINK_LED_8(blinkAck)

#define LED2PIN(led_no) ( \
	      TERNARY_LED_1(led_no)	\
              TERNARY_LED_2(led_no) \
              TERNARY_LED_3(led_no) \
              TERNARY_LED_4(led_no) \
              TERNARY_LED_5(led_no) \
              TERNARY_LED_6(led_no) \
              TERNARY_LED_7(led_no) \
              TERNARY_LED_8(led_no) \
              -1)

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

#endif // ! _GIRS_MACROS_H
