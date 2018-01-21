#!/bin/bash

arm-none-eabi-gdb -ex "target remote localhost:4242" -ex "monitor reset halt" -ex "load" -ex "b main" -ex "set \$sp=(*0)" -ex "j entry"  out/machine-smartfork/firmware/smartfork-system/system
