# Adjust to point to where InfraredTypes.h etc are located.
ARDUINO_LIBS=$(HOME)/Arduino/libraries
INFRARED4ARDUINO=$(ARDUINO_LIBS)/Infrared
SUBDIRS=$(INFRARED4ARDUINO) GirsLib IrNamedCommand Girs

#BOARDDEFINES=-D ARDUINO_AVR_NANO
BOARDDEFINES=

default:
	$(foreach subdir, $(SUBDIRS), $(MAKE) -C $(subdir) BOARDDEFINES="$(BOARDDEFINES)";)

clean:
	$(foreach subdir, $(SUBDIRS), $(MAKE) -C $(subdir) clean;)
	$(foreach subdir, $(SUBDIRS), rm -f $(subdir)/*.hex;)
