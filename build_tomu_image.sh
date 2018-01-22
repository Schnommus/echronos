#!/bin/bash
prj/app/prj.py build machine-tomu.rigel-test.rigel-test-system
arm-none-eabi-objcopy -O binary out/machine-tomu/rigel-test/rigel-test-system/system tomu-rigel-test.bin
