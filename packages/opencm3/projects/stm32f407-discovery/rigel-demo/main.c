#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "rtos-rigel.h"
#include "debug.h"

/* Systick interrupt frequency, Hz */
#define SYSTICK_FREQUENCY 100


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

/* Mandelbrot calculations */

/* Maximum number of iterations for the escape-time calculation */
#define maxIter 32
/* This array converts the iteration count to a character representation. */
static char color[maxIter+1] = " .:++xxXXX%%%%%%################";

/* Main mandelbrot calculation */
static int iterate(float px, float py)
{
	int it = 0;
	float x = 0, y = 0;
	while (it < maxIter) {
		float nx = x*x;
		float ny = y*y;
		if ((nx + ny) > 4) {
			return it;
		}
		/* Zn+1 = Zn^2 + P */
		y = 2*x*y + py;
		x = nx - ny + px;
		it++;
	}
	return 0;
}

void fn_c(void)
{
	float scale = 0.25f, centerX = -0.5f, centerY = 0.0f;

    debug_println("task c: starting mandelbrot on USART2 (PA2)");

    for (;;)
    {
		gpio_toggle(GPIOD, GPIO14);	/* Toggle LED every time we render a frame */

        int x, y;
        for (x = -60; x < 60; x++) {
            for (y = -50; y < 50; y++) {
                int i = iterate(centerX + x*scale, centerY + y*scale);
                usart_send_blocking(USART2, color[i]);

                /* yielding here so we don't miss any deadlines */
                rtos_yield();
            }
            usart_send_blocking(USART2, '\r');
            usart_send_blocking(USART2, '\n');
        }

		/* Change scale and center */
		centerX += 0.175f * scale;
		centerY += 0.522f * scale;
		scale	*= 0.875f;

		usart_send_blocking(USART2, '\r');
		usart_send_blocking(USART2, '\n');
		gpio_toggle(GPIOD, GPIO13);
        rtos_sleep(10);
    }
}

void systick_start(uint32_t systick_frequency, uint32_t ahb_frequency)
{
    if(!ahb_frequency)
    {
        debug_println("systick_start failed (ahb_frequency not set)");
        for(;;);
    }

    bool succeeded = systick_set_frequency(systick_frequency, ahb_frequency);

    if(!succeeded)
    {
        debug_println("systick_start failed (systick_frequency is too low)");
        for(;;);
    }

	systick_counter_enable();
    systick_interrupt_enable();
}

/* Set STM32 to 168 MHz, returns the final clock speed */
uint32_t clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* Enable GPIO clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

    return rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ].ahb_frequency;
}

static void gpio_setup(void)
{
	/* Set GPIO12-15 (in GPIO port D) to 'output push-pull'. (LEDs) */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

static void usart_setup(void)
{
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}


int main(void)
{
    libopencm3_pre_main();

    debug_println("Initializing peripherals...");

    /* Setup clocks, initialize peripherals etc. here */

	uint32_t ahb_freq = clock_setup();
	gpio_setup();
    usart_setup();

    /* Start the systick interrupt based on clock settings above */

    systick_start(SYSTICK_FREQUENCY, ahb_freq);

    debug_println("Starting RTOS...");

    rtos_start();

    /* rtos_start should never return, spin forever */

    debug_println("rtos_start returned? ...");

    for(;;);

    return 0;
}
