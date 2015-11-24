#! /usr/bin/env m4
changecom()dnl
define(`ledwait', 100)dnl
define(`begtimeout', 6000)dnl
# Tests/demo for Girs
# First get in sync by some empty lines



v
m
mem
remote
remote yamaha
# Send nec1 122/29 (yamaha power_on)
send 1 38400 68 4 0 9024 4512 564 564 564 1692 564 564 564 1692 564 1692 564 1692 564 1692 564 564 564 1692 564 564 564 1692 564 564 564 564 564 564 564 564 564 1692 564 1692 564 564 564 1692 564 1692 564 1692 564 564 564 564 564 564 564 564 564 1692 564 564 564 564 564 564 564 1692 564 1692 564 1692 564 39756 9024 2256 564 96156
send 4 0 0 50 0 388 1164 388 1164 388 1164 388 1164 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 388 1164 388 1164 388 1164 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 11364
p begintimeout begtimeout
lcd Now capturing...
a # Now fire an IR command at the sensor, or wait for timeout (eval(begtimeout/1000) s)
\sleep 2000 # stop sending IR
lcd Now receiving...
r # Now fire an IR command at the sensor, or wait for timeout (eval(begtimeout/1000) s)
\sleep 2000
p receiveendtimeout
p receiveendtimeout 70
lcd Now receiving...
r # Now fire an IR command at the sensor, or wait for timeout (eval(begtimeout/1000) s)
p commandled -1
led 1 on
\sleep ledwait
led 2 on
\sleep ledwait
led 3 on
\sleep ledwait
led 4 on
\sleep ledwait
led 5 on
\sleep ledwait
led 6 on
\sleep ledwait
led 7 on
\sleep ledwait
led 8 on
\sleep ledwait
led 1 off
\sleep ledwait
led 2 off
\sleep ledwait
led 3 off
\sleep ledwait
led 4 off
\sleep ledwait
led 5 off
\sleep ledwait
led 6 off
\sleep ledwait
led 7 off
\sleep ledwait
led 8 off
p commandled 5
t 10 nec1 122 27
send 4 0 0 50 0 388 1164 388 1164 388 1164 388 1164 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 1164 388 388 1164 388 1164 388 1164 388 1164 388 1164 1164 388 388 1164 1164 388 388 1164 388 1164 388 11364
name 1 yamaha power_off
ifdef(`ethernet',quit,)
