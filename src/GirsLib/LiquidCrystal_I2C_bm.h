/**
 * @file src/GirsLib/LiquidCrystal_I2C_bm.h
 * @brief Functions for deploying I2C LCD displays.
 *
 * This is a cleaned-up version of LiquidCrystal_I2C version 1.1.2 from
 * the Arduino library manager. It is originally by DFRobot, has been
 * maintained by Marco Schwarz, Frank de Brabander, and John Rickman.
 * I have not found any valid license, so I assume that my usage is legit.
 */

#pragma once

#include <Arduino.h>

class LiquidCrystal_I2C : public Print {
public:

    static constexpr uint8_t LCD_5x10DOTS = 0x04U;
    static constexpr uint8_t LCD_5x8DOTS  = 0x00;

    LiquidCrystal_I2C(uint8_t i2c_Addr, uint8_t lcd_cols, uint8_t lcd_rows);

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void printLeft();
    void printRight();
    void leftToRight();
    void rightToLeft();
    void shiftIncrement();
    void shiftDecrement();
    void noBacklight();
    void backlight();
    void autoscroll();
    void noAutoscroll();
    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t);
    void init();
    size_t write(uint8_t);

#if 0
    ////Unsupported API functions (not implemented in this library)
    uint8_t status();
    void setContrast(uint8_t new_val);
    uint8_t keypad();
    void setDelay(int, int);
    void on();
    void off();
    uint8_t init_bargraph(uint8_t graphtype);
    void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
    void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len, uint8_t pixel_col_end);
#endif

private:
    void command(uint8_t);
    void init_priv();
    void send(uint8_t, uint8_t);
    void write4bits(uint8_t);
    void expanderWrite(uint8_t);
    void pulseEnable(uint8_t);
    uint8_t _Addr;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _backlightval;
};
