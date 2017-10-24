#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "rtos-rigel.h"
#include "debug.h"

/* Maximum number of iterations for the escape-time calculation */
#define maxIter 32

/* This array converts the iteration count to a character representation. */
static char color[maxIter+1] = " .:++xxXXX%%%%%%################";

/* Main mandelbrot calculation */
static int iterate(float px, float py)
{
	int it = 0;
	float x = 0, y = 0;
	while (it < maxIter)
    {
		float nx = x*x;
		float ny = y*y;
		if ((nx + ny) > 4)
        {
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
        for (x = -60; x < 60; x++)
        {
            for (y = -50; y < 50; y++)
            {
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
        rtos_sleep(10);
    }
}
