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

extern uint32_t vectors_virt_addr;
extern uint32_t all_start;
extern uint32_t all_size;

#define FW_VERSION          (0x0100)
#define FW_SUBVERSION       (0x0001)
#define FW_CHIP_ID          (0x8195)
#define FW_CHIP_VER         (0x01)
#define FW_BUS_TYPE         (0x01)          // the iNIC firmware type: USB/SDIO
#define FW_INFO_RSV1        (0x00)          // the firmware information reserved
#define FW_INFO_RSV2        (0x00)          // the firmware information reserved
#define FW_INFO_RSV3        (0x00)          // the firmware information reserved
#define FW_INFO_RSV4        (0x00)          // the firmware information reserved

__attribute__((section(".image2.header1")))
const uint32_t image2_header1[] = {
    (uint32_t)&all_size, // Size of data to be copied from flash to ram
    (uint32_t)&all_start, // Where to start copying to ram
};

__attribute__((section(".image2.header2")))
const uint8_t image2_header2[] = {
    '8', '1', '9', '5', '8', '7', '1', '1',
};


void crt0(void);

__attribute__((section(".image2.header3")))
const uint32_t image2_header3[] = {
    (uint32_t)&crt0 + 1 //start by executing crt0, in thumb mode (this was previously InfraStart)
};

__attribute__((section(".image2.header4")))
const uint8_t image2_header4[] = {
	'R', 'T', 'K', 'W', 'i', 'n', 0x0, 0xff, 
	(FW_VERSION&0xff), ((FW_VERSION >> 8)&0xff),
	(FW_SUBVERSION&0xff), ((FW_SUBVERSION >> 8)&0xff),
	(FW_CHIP_ID&0xff), ((FW_CHIP_ID >> 8)&0xff),
	(FW_CHIP_VER),
	(FW_BUS_TYPE),
	(FW_INFO_RSV1),
	(FW_INFO_RSV2),
	(FW_INFO_RSV3),
	(FW_INFO_RSV4)
};


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
void systick_handler() { for(;;); }

extern __attribute__ ((long_call)) uint32_t
DiagPrintf(
    const char *fmt, ...
);

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

#define CORTEX_INTERRUPT_MAX 256

extern void entry(void);

int
main(void)
{

	uint32_t i;

	cortex_interrupts_disable();
	for(i = 0; i < CORTEX_INTERRUPT_MAX; i++)cortex_interrupt_disable(i);

    // Relocate vector table (This doesn't currently work?!)
	SCB->VTOR = (uint32_t)&vectors_virt_addr;

	interrupts_enable();

    DiagPrintf("Starting RTOS...\n");

    rtos_start();
    for (;;) ;
}

void crt0(void){

    // Warning! Data section is uninitialized before 'entry' here

    // Warning2! entry does not return here, it jumps to main
    entry();
}
