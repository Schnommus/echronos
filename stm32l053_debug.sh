#!/bin/bash

arm-none-eabi-gdb -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "monitor arm semihosting enable" -ex "load" -ex "set $sp=(*0)" -ex "j entry"  out/machine-tomu/rigel-test/rigel-test-system/system
