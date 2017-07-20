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

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "cortex.h"
#include "rtl8710.h"
#include "mask.h"
#include "serial.h"

#include "rtos-acamar.h"

extern void debug_println(const char *msg);

void fn_a(void);
void fn_b(void);

int variable=42;

void
fn_a(void)
{
    for (;;)
    {
        rtos_yield_to(1);
        debug_println("task A");
        printf("In task A...");
    }
}

void
fn_b(void)
{
    for (;;)
    {
        rtos_yield_to(0);
        debug_println("task b");
        printf("In task B...");
    }
}

#define CORTEX_INTERRUPT_MAX 256

int
main(void)
{

	cortex_interrupts_disable();

    int i;
	for(i = 0; i < CORTEX_INTERRUPT_MAX; i++)cortex_interrupt_disable(i);

	SCB->VTOR = 0x10001000;

    SCB->CPACR |= ((((uint32_t)0x03) << 20) | (((uint32_t)0x03) << 22));  /* set CP10 and CP11 Full Access */
	__asm__("dsb"); // wait for store to complete
	__asm__("isb"); // reset pipeline

	__asm__("mov r2, %0": : "r" (0x10001000));
	__asm__("ldr r3, [r2, #0]");
	__asm__("mov sp, r3");

	PERI_ON->CPU_PERIPHERAL_CTRL |= PERI_ON_CPU_PERIPHERAL_CTRL_SWD_PIN_EN; // re-enable SWD

	PERI_ON->PESOC_CLK_CTRL |= PERI_ON_CLK_CTRL_ACTCK_GPIO_EN | PERI_ON_CLK_CTRL_SLPCK_GPIO_EN; // enable gpio peripheral clock
	PERI_ON->SOC_PERI_FUNC1_EN |= PERI_ON_SOC_PERI_FUNC1_EN_GPIO; // enable gpio peripheral

    serial_init();

    // set system clock
	mask32_set(SYS->CLK_CTRL1, SYS_CLK_CTRL1_PESOC_OCP_CPU_CK_SEL, 1);

	//PERI_ON->GPIO_SHTDN_CTRL = 0xFF;
	PERI_ON->GPIO_DRIVING_CTRL = 0xFF;

	GPIO->SWPORTA_DDR |= GPIO_PORTA_GC4;

	cortex_interrupts_enable();

    debug_println("Starting RTOS...");
    rtos_start();
    for (;;) ;
}

ssize_t write_stdout(const void *buf, size_t count){
	size_t i;
	for(i = 0; i < count; i++){
		if((((uint8_t *)buf)[i]) == '\n')while(!serial_write("\r", 1));
		while(!serial_write(&(((uint8_t *)buf)[i]), 1));
	}
	return(count);
}
