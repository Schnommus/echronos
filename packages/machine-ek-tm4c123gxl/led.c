/*
 * eChronos Real-Time Operating System
 * Copyright (C) 2015  National ICT Australia Limited (NICTA), ABN 62 102 206 173.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3, provided that these additional
 * terms apply under section 7:
 *
 *   No right, title or interest in or to any trade mark, service mark, logo
 *   or trade name of of National ICT Australia Limited, ABN 62 102 206 173
 *   ("NICTA") or its licensors is granted. Modified versions of the Program
 *   must be plainly marked as such, and must not be distributed using
 *   "eChronos" as a trade mark or product name, or misrepresented as being
 *   the original Program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @TAG(NICTA_AGPL)
 */

#include <stdint.h>
#include "led.h"

#define ROM_API_TABLE            ((uint32_t *)0x01000010)
#define ROM_GPIO_TABLE           ((uint32_t *)(ROM_API_TABLE[4]))
#define ROM_SYSCTL_TABLE         ((uint32_t *)(ROM_API_TABLE[13]))

#define PERIPH_ENABLE \
        ((void (*)(uint32_t peripheral))ROM_SYSCTL_TABLE[6])

#define PERIPH_READY \
        ((int (*)(uint32_t peripheral))ROM_SYSCTL_TABLE[35])

#define GPIO_PORT_IS_OUTPUT \
        ((void (*)(uint32_t port, \
                   uint8_t pins))ROM_GPIO_TABLE[15])

#define GPIO_PORT_WRITE \
        ((void (*)(uint32_t port, \
                   uint8_t pins, \
                   uint8_t val))ROM_GPIO_TABLE[0])

#define PERIPH_GPIOF            0xf0000805  // GPIO F (Peripheral)
#define GPIO_PORTF              0x40025000  // GPIO F (GPIO reference)

#define RED_LED                 0x00000002
#define BLUE_LED                0x00000004
#define GREEN_LED               0x00000008

#define ALL_LEDS (RED_LED|BLUE_LED|GREEN_LED)

void
led_green_on(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, GREEN_LED, GREEN_LED);
}

void
led_green_off(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, GREEN_LED, 0);
}

void
led_blue_on(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, BLUE_LED, BLUE_LED);
}

void
led_blue_off(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, BLUE_LED, 0);
}

void
led_red_on(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, RED_LED, RED_LED);
}

void
led_red_off(void)
{
    GPIO_PORT_WRITE(GPIO_PORTF, RED_LED, 0);
}

void
led_init(void)
{
    PERIPH_ENABLE(PERIPH_GPIOF); /* Initialize the GPIO peripheral */
    while(!PERIPH_READY(PERIPH_GPIOF)); /* Wait for it to be ready */
    GPIO_PORT_IS_OUTPUT(GPIO_PORTF, ALL_LEDS); /* Set the LED pins to outputs */
    GPIO_PORT_WRITE(GPIO_PORTF, ALL_LEDS, 0); /* Turn all the LEDs off */
}
