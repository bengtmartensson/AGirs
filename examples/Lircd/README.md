# Lirc compatible server Lircd

This sketch implements a Lirc server, in the context of AGirs,
(mostly) compatible with lircd from the Lirc distribution. It
implements most of the commands described by the man page of `irsend`,
plus a few forgotten on the man page (`VERSION`), plus some only
present here.

It is currently meant as a proof of concept; but can be taken as a basis for
future deployment programs. It has only been tested on a Mega2650 with
W5100 Ethernet card. For resource reasons, it will probably not run on
a 2kB RAM board like UNO or Nano.

The data base of remotes and commands is presently compiled in. This
is not necessarily a final solution, but makes sense on a Harvard architecture
processor, with very limited RAM memory.

The `SEND_CCF_ONCE` command of
http://www.harctoolbox.org/lirc_ccf.html is presently not implemented,
but this could easily be done.
