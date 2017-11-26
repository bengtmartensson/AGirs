#pragma once

#include "hardware-config/girs_pins.h"

#ifdef LCD_0x3F_20_4
#include "hardware-config/lcd_0x3F_20_4.h"
#elif defined (LCD_0x27_20_4)
#include "hardware-config/lcd_0x27_20_4.h"
#elif defined (LCD_0x27_16_2)
#include "hardware-config/lcd_0x27_16_2.h"
#endif
