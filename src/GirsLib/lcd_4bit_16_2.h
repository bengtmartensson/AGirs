// Configuration for LCD shield, 4bit.
// Makes very little sense on non-Uno's.

#ifndef LCD_4BIT_16_2_h
#define LCD_4BIT_16_2_h

#ifndef LCD_4BIT
#define LCD_4BIT
#endif

#define LCD_INIT_ARGS 8,9,4,5,6,7
#define LCD_BACKLIGHT_PIN 10 // Clashes with W5100 ethernet shield
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#endif // ! LCD_4BIT_16_2_h
