INFRARED4ARDUINO=../../Infrared4Arduino
SUBDIRS=$(INFRARED4ARDUINO) GirsLib IrNamedCommand Girs

#BOARDDEFINES=-D ARDUINO_AVR_NANO
BOARDDEFINES=

default:
	$(foreach subdir, $(SUBDIRS), $(MAKE) -C $(subdir) BOARDDEFINES="$(BOARDDEFINES)";)

clean:
	$(foreach subdir, $(SUBDIRS), $(MAKE) -C $(subdir) clean;)
	$(foreach subdir, $(SUBDIRS), rm -f $(subdir)/*.hex;)
