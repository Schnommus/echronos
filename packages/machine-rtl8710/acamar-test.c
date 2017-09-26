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

//#include "cortex.h"
//#include "rtl8710.h"

#include "device.h"
#include "gpio_api.h"

#include "rtos-acamar.h"

// vectable.py has been modified to match
#define CORTEX_INTERRUPT_MAX 32

// Linker symbol, used to relocate vector table
extern uint32_t vectors_virt_addr;

#define GPIO_LED_PIN PC_2

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

extern __attribute__ ((long_call)) uint32_t
DiagPrintf(
    const char *fmt, ...
);

int led_on = 0;
gpio_t gpio_led;

void fatal(int error) { for(;;); }
void nmi_handler() { for(;;); }
void memmanage_handler() { for(;;); }
void busfault_handler() { for(;;); }
void usagefault_handler() { for(;;); }
void svcall_handler() { for(;;); }
void debug_monitor_handler() { for(;;); }
void pendsv_handler() { for(;;); }

enum { r0, r1, r2, r3, r12, lr, pc, psr};

void stackDump(uint32_t stack[])
{
   DiagPrintf("r0  = 0x%08x\n", stack[r0]);
   DiagPrintf("r1  = 0x%08x\n", stack[r1]);
   DiagPrintf("r2  = 0x%08x\n", stack[r2]);
   DiagPrintf("r3  = 0x%08x\n", stack[r3]);
   DiagPrintf("r12 = 0x%08x\n", stack[r12]);
   DiagPrintf("lr  = 0x%08x\n", stack[lr]);
   DiagPrintf("pc  = 0x%08x\n", stack[pc]);
   DiagPrintf("psr = 0x%08x\n", stack[psr]);
}

void hardfault_handler_c(uint32_t *stack) {
    stackDump(stack);
    for(;;);
}

__attribute__((naked))
    void hardfault_handler(void){
        /*
         * Get the appropriate stack pointer, depending on our mode,
         * and use it as the parameter to the C handler. This function
         * will never return
         */

        __asm( ".syntax unified\n"
                "MOVS R0, #4 \n"
                "MOV R1, LR \n"
                "TST R0, R1 \n"
                "BEQ _MSP \n"
                "MRS R0, PSP \n"
                "B hardfault_handler_c\n"
                "_MSP: \n"
                "MRS R0, MSP \n"
                "B hardfault_handler_c \n"
                ".syntax divided\n") ;

}

void systick_handler() {
    DiagPrintf("Tick...\n");

    led_on ^= 1;
    gpio_write(&gpio_led, led_on);
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
	//uint32_t i;
    HalCommonInit();

    __disable_irq();

    // In case the bootloader has enabled some IRQs we can't yet handle
	//for(i = 0; i < CORTEX_INTERRUPT_MAX; i++)cortex_interrupt_disable(i);

	SCB->VTOR = (uint32_t)&vectors_virt_addr;


    // Initialize the LED pin
    gpio_init(&gpio_led, GPIO_LED_PIN);
    gpio_dir(&gpio_led, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_led, PullNone);     // No pull

    // Configure the SysTick interrupt
    // Note that 0x00FFFFFF is 10Hz, we tick at 100Hz
    // (Still need to measure precisely, these are rough)
    SYST_RVR_WRITE(0x001a0000);
    SYST_CVR_WRITE(0);
    SYST_CSR_WRITE((1 << 2) | (1 << 1) | 1);

    __enable_irq();

    DiagPrintf("Starting RTOS...\n");

    init_event_callback_list();
    fATWS();

    rtos_start();
    for (;;) ;
}
