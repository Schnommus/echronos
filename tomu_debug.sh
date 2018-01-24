#!/bin/bash

arm-none-eabi-gdb -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "monitor arm semihosting enable" -ex "load" -ex "set $sp=(*0)" -ex "b main" -ex "b hardfault_isr" -ex "j entry"  out/opencm3/projects/tomu/rigel-usb/rigel-usb-system/system
