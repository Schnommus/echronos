#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "rtos-acamar.h"
#include "debug.h"

#define LED_GREEN_PIN GPIO4
#define LED_GREEN_PORT GPIOB
#define LED_RED_PIN GPIO5
#define LED_RED_PORT GPIOA

void fn_a(void)
{
    debug_println("Started task A");
    for (;;)
    {
        gpio_set(LED_GREEN_PORT, LED_GREEN_PIN);
        gpio_clear(LED_RED_PORT, LED_RED_PIN);
        debug_println("-> in task A");
        rtos_yield_to(RTOS_TASK_ID_B);
    }
}

void fn_b(void)
{
    debug_println("Started task B");
    for (;;)
    {
        gpio_clear(LED_GREEN_PORT, LED_GREEN_PIN);
        gpio_set(LED_RED_PORT, LED_RED_PIN);
        debug_println("-> in task B");
        rtos_yield_to(RTOS_TASK_ID_A);
    }
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	/* set pins to output mode, push pull */
	gpio_mode_setup(LED_RED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_RED_PIN);
	gpio_mode_setup(LED_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GREEN_PIN);
}

int main(void)
{
    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* Setup clocks, initialize peripherals etc. here */

    gpio_setup();

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}
