#!/bin/bash

arm-none-eabi-gdb -ex "target remote localhost:4242" -ex "monitor reset halt" -ex "monitor semihosting enable" -ex "load" -ex "set $sp=(*0)" -ex "j entry"  out/opencm3/projects/stm32l053-discovery/rigel-test/rigel-test-system/system
