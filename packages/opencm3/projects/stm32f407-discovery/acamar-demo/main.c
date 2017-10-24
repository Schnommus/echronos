#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "rtos-acamar.h"
#include "debug.h"

void fn_a(void)
{
    debug_println("Started task A");
    for (;;)
    {
        gpio_set(GPIOD, GPIO12 | GPIO14);
        gpio_clear(GPIOD, GPIO13 | GPIO15);
        debug_println("-> in task A");
        rtos_yield_to(RTOS_TASK_ID_B);
    }
}

void fn_b(void)
{
    debug_println("Started task B");
    for (;;)
    {
        gpio_clear(GPIOD, GPIO12 | GPIO14);
        gpio_set(GPIOD, GPIO13 | GPIO15);
        debug_println("-> in task B");
        rtos_yield_to(RTOS_TASK_ID_A);
    }
}

/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* Enable GPIOD clock. */
	rcc_periph_clock_enable(RCC_GPIOD);
}

static void gpio_setup(void)
{
	/* Set GPIO12-15 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
}

int main(void) {

    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* Setup clocks, initialize peripherals etc. */

	clock_setup();
	gpio_setup();

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    while(1);

    return 0;
}
