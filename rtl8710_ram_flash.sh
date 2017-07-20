#!/bin/bash

set -e

ECHRONOS_BASE=/home/seb/dev/echronos-sandbox
PROJECT=/out/machine-rtl8710/acamar-system
BIN_FILE="$ECHRONOS_BASE/$PROJECT/system.bin"
ELF_FILE="$ECHRONOS_BASE/$PROJECT/system.elf"
LOAD_ADDRESS=0x10001000

arm-none-eabi-gdb $ELF_FILE \
    -ex 'target extended localhost:3333' \
    -ex 'monitor arm semihosting enable'
    -ex 'monitor init' \
    -ex 'monitor reset halt' \
    -ex "monitor load_image $BIN_FILE $LOAD_ADDRESS bin" \
    -ex "monitor cortex_bootstrap $LOAD_ADDRESS" \
