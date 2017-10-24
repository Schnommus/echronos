#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "rtos-rigel.h"
#include "debug.h"

void fn_a(void)
{
    debug_println("task a: starting slow toggle");
    for (;;)
    {
		gpio_toggle(GPIOD, GPIO12);
        rtos_sleep(50);
    }
}

void fn_b(void)
{
    debug_println("task b: starting fast toggle");
    for (;;)
    {
		gpio_toggle(GPIOD, GPIO13);
        rtos_sleep(10);
    }
}
