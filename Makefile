.DEFAULT_GOAL := kochab

blinky:
	prj/app/prj.py build machine-stm32f4-discovery.example.blinky-system
	cp out/machine-stm32f4-discovery/example/blinky-system/system system.elf

kochab:
	prj/app/prj.py build machine-stm32f4-discovery.example.kochab-system
	cp out/machine-stm32f4-discovery/example/kochab-system/system system.elf

kochab-link:
	arm-none-eabi-ld -T out/machine-stm32f4-discovery/example/kochab-system/new.ld -o out/machine-stm32f4-discovery/example/kochab-system/system --print-memory-usage out/machine-stm32f4-discovery/example/kochab-system/armv7m.ctxt-switch-preempt.o out/machine-stm32f4-discovery/example/kochab-system/armv7m.exception-preempt.o out/machine-stm32f4-discovery/example/kochab-system/vectable.o out/machine-stm32f4-discovery/example/kochab-system/semihost-debug.o out/machine-stm32f4-discovery/example/kochab-system/rtos-kochab.o out/machine-stm32f4-discovery/example/kochab-system/generic.debug.o out/machine-stm32f4-discovery/example/kochab-system/machine-armv7m-common.example.machine-timer.o out/machine-stm32f4-discovery/example/kochab-system/kochab-test.o

debug_server:
	sudo openocd -f /home/seb/dev/openocd_profiles/stm32f4discovery.cfg
