#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/lm4f/systemcontrol.h>
#include <libopencm3/lm4f/rcc.h>
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/nvic.h>

#include "rtos-acamar.h"
#include "debug.h"

#define RGB_PORT	GPIOF
enum {
	LED_R	= GPIO1,
	LED_G	= GPIO3,
	LED_B	= GPIO2,
};

void fn_a(void)
{
    debug_println("Started task A");
    for (;;)
    {
		gpio_set(RGB_PORT, LED_R);
		gpio_clear(RGB_PORT, LED_G | LED_B);

        debug_println("-> in task A");
        rtos_yield_to(RTOS_TASK_ID_B);
    }
}

void fn_b(void)
{
    debug_println("Started task B");
    for (;;)
    {
		gpio_set(RGB_PORT, LED_G);
		gpio_clear(RGB_PORT, LED_B | LED_R);

        debug_println("-> in task B");
        rtos_yield_to(RTOS_TASK_ID_A);
    }
}


static void clock_setup(void)
{
    enum {
        PLL_DIV_80MHZ 	= 5,
        PLL_DIV_57MHZ 	= 7,
        PLL_DIV_40MHZ 	= 10,
        PLL_DIV_20MHZ 	= 20,
        PLL_DIV_16MHZ 	= 25,
    };

	rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, PLL_DIV_80MHZ);
}

static void gpio_setup(void)
{
	/*
	 * Configure GPIOF
	 * This port is used to control the RGB LED
	 */
	periph_clock_enable(RCC_GPIOF);
	const uint32_t outpins = (LED_R | LED_G | LED_B);

	gpio_mode_setup(RGB_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, outpins);
	gpio_set_output_config(RGB_PORT, GPIO_OTYPE_PP, GPIO_DRIVE_2MA, outpins);
}

int main(void)
{
    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* Setup clocks, initialize peripherals etc. here */

	clock_setup();
	gpio_setup();

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}

/* Some of this chip's API requires memcpy, so we define our own here rather than including
 * the entire C standard library */

void* memcpy(void* dest, const void* src, uint32_t count) {
    char* dst8 = (char*)dest;
    char* src8 = (char*)src;

    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

