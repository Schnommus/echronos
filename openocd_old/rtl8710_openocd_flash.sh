#!/bin/bash

ECHRONOS_BASE=/home/seb/dev/echronos-sandbox
PROJECT=out/machine-rtl8710/acamar-system
BIN_FILE="$ECHRONOS_BASE/$PROJECT/system.bin"
ADDRESS=0x10001000

sudo openocd -f interface/stlink-v2.cfg -f packages/machine-rtl8710/rtl8710.ocd -f packages/machine-rtl8710/cortex.ocd -c "init" -c "reset halt" -c "arm semihosting enable" -c "load_image $BIN_FILE $ADDRESS bin" -c "cortex_bootstrap $ADDRESS"
