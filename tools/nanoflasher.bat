@echo off
REM Flashing of the Arduino Nano, using avrdude, for example installed
REM by Arduino.

REM Usage:
REM ${OUTFILE} [<PORT>]

REM where -n is for boards with the "new" bootloader, called optiboot.
REM <PORT> is the device, default COM5.

rem Use 115200 for the new bootloader ("optiboot"), otherwise 57600.
rem set BAUD=115200
set BAUD=57600

if "%2" == "" (
   set PORT=COM5
) else (
   set PORT=%2
)

set ARDUINO_ROOT=C:\Program Files\Arduino
set TOOLS_ROOT=%ARDUINO_ROOT%\hardware\tools\avr
set AVRDUDE=%TOOLS_ROOT%\bin\avrdude
set AVRDUDE_CONF=%TOOLS_ROOT%\etc\avrdude.conf
set PART=atmega328p
set PROGRAMMER_ID=arduino

set HEXFILE=%1

"%AVRDUDE%" -C "%AVRDUDE_CONF%" -v -p %PART% -c %PROGRAMMER_ID% -P %PORT% -b %BAUD% -D -U flash:w:%HEXFILE%:i
