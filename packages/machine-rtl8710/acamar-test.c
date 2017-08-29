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
#include <stdarg.h>

#include "cortex.h"
#include "rtl8710.h"
#include "mask.h"

#include "rtos-acamar.h"

// vectable.py has been modified to match
#define CORTEX_INTERRUPT_MAX 32

// Linker symbol, used to relocate vector table
extern uint32_t vectors_virt_addr;

// Registers required to configure the systick interrupt
#define SYST_CSR_REG 0xE000E010
#define SYST_RVR_REG 0xE000E014
#define SYST_CVR_REG 0xE000E018

#define SYST_CSR_WRITE(x) (*((volatile uint32_t*)SYST_CSR_REG) = x)
#define SYST_RVR_WRITE(x) (*((volatile uint32_t*)SYST_RVR_REG) = x)
#define SYST_CVR_WRITE(x) (*((volatile uint32_t*)SYST_CVR_REG) = x)

extern void debug_println(const char *msg);

void fn_a(void);
void fn_b(void);

int variable=42;

void fatal(int error) { for(;;); }
void nmi_handler() { for(;;); }
void hardfault_handler() { for(;;); }
void memmanage_handler() { for(;;); }
void busfault_handler() { for(;;); }
void usagefault_handler() { for(;;); }
void svcall_handler() { for(;;); }
void debug_monitor_handler() { for(;;); }
void pendsv_handler() { for(;;); }

extern __attribute__ ((long_call)) uint32_t
DiagPrintf(
    const char *fmt, ...
);

void systick_handler() {
    DiagPrintf("Tick...\n");
}

void
fn_a(void)
{
    for (;;)
    {
        rtos_yield_to(1);
        DiagPrintf("Task A\n");
    }
}

void
fn_b(void)
{
    for (;;)
    {
        rtos_yield_to(0);
        DiagPrintf("Task B\n");
    }
}

int
main(void)
{
	uint32_t i;

	cortex_interrupts_disable();

    // In case the bootloader has enabled some IRQs we can't yet handle
	for(i = 0; i < CORTEX_INTERRUPT_MAX; i++)cortex_interrupt_disable(i);

	SCB->VTOR = (uint32_t)&vectors_virt_addr;

    // Configure the SysTick interrupt
    // Note that 0x00FFFFFF is 10Hz, we tick at 100Hz
    // (Still need to measure precisely, these are rough)
    SYST_RVR_WRITE(0x001a0000);
    SYST_CVR_WRITE(0);
    SYST_CSR_WRITE((1 << 2) | (1 << 1) | 1);

	interrupts_enable();

    DiagPrintf("Starting RTOS...\n");

    rtos_start();
    for (;;) ;
}
