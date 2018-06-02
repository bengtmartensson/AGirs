# Translator

This is more of a demonstration than a directly usable program. It just runs in an infinite loop, receiving IR signals, decodes them,
and, possibly sending out other signals. It is hoped that this program can serve as an
inspiration to be adapted to your own needs.

The program recognizes the Yamaha volume up and volume down commands, (Nec1 122/133/26 and 122/133/27 respectively),
and sending instead the volume up and down commands for a Philips TV (RC5 0/16 and 0/17 respectively). Repeats are presently not recognized.

It uses a demodulating IR receiver (e.g. TSOP-*).
If an LCD display is connected, the recognized and translated commands are written to it.
Also, LEDs can be connected, providing some status information.

To configure, just edit config.h. See the build instructions in top level README.md.

Strictly speaking, this program is not "Girs". However, it uses the code of AGirs (and thus, indirectly, Infrared4Arduino).
