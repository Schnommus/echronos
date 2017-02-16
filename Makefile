.DEFAULT_GOAL := mpu_rigel

mpu_blinky:
	prj/app/prj.py build machine-ek-tm4c123gxl.example.blinky-system

mpu_rigel:
	prj/app/prj.py build machine-ek-tm4c123gxl.example.rigel-mpu

mpu_acamar:
	prj/app/prj.py build machine-ek-tm4c123gxl.example.acamar-mpu

debug_server_stm32:
	sudo openocd -f /home/seb/dev/openocd_profiles/stm32f4discovery.cfg

debug_server_tm4c:
	sudo openocd -f board/ek-tm4c1294xl.cfg
