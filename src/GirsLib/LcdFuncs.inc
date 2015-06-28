#ifdef LCD

void lcdPrint(String str, bool clear = false, int x = 0, int y = -1) {
    if (clear)
        lcd.clear();
    lcd.display();
    LCD_BACKLIGHT_ON(lcd);
    if (y >= 0)
        lcd.setCursor(x, y);
    lcd.print(str);
    updateTurnoffTime();
}

#endif
