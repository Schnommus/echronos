#!/bin/bash

sudo openocd -f interface/stlink-v2.cfg -f packages/machine-rtl8710/rtl8710.ocd -f packages/machine-rtl8710/cortex.ocd
