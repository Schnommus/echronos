.DEFAULT_GOAL := mpu_acamar

mpu_acamar:
	prj/app/prj.py build machine-ek-tm4c123gxl.example.acamar-mpu

link_map:
	arm-none-eabi-ld -T out/machine-ek-tm4c123gxl/example/acamar-mpu/default.ld -o out/machine-ek-tm4c123gxl/example/acamar-mpu/system --print-memory-usage -M out/machine-ek-tm4c123gxl/example/acamar-mpu/armv7m.ctxt-switch.o out/machine-ek-tm4c123gxl/example/acamar-mpu/vectable.o out/machine-ek-tm4c123gxl/example/acamar-mpu/semihost-debug.o out/machine-ek-tm4c123gxl/example/acamar-mpu/rtos-acamar.o out/machine-ek-tm4c123gxl/example/acamar-mpu/generic.debug.o out/machine-ek-tm4c123gxl/example/acamar-mpu/acamar-mpu-test.o

debug_server_stm32:
	sudo openocd -f /home/seb/dev/openocd_profiles/stm32f4discovery.cfg

debug_server_tm4c:
	sudo openocd -f board/ek-tm4c1294xl.cfg
