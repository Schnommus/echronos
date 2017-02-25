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

#include <stddef.h>
#include <stdint.h>

#include "rtos-acamar.h"
#include "debug.h"

#define REGISTER(x) (*((volatile uint32_t *)(x)))

#define DWT_CYCCNT  0xE0001004
#define DWT_CONTROL 0xE0001000
#define SCB_DEMCR   0xE000EDFC

#define PRIVILEGED_ACTION(x) \
    rtos_internal_elevate_privileges(); \
    (x); \
    rtos_internal_drop_privileges();

extern void debug_println(const char *msg);
extern void rtos_internal_elevate_privileges();
extern void rtos_internal_drop_privileges();

void fn_a(void);
void fn_b(void);
void fatal(RtosErrorId error_id);

uint32_t dom1_variable_1;
uint32_t dom1_variable_2 = 42;
uint32_t dom2_variable_1;
uint32_t dom2_variable_2 = 7;

void nmi() { for(;;); }
void hardfault() { for(;;); }
void busfault() { for(;;); }
void usagefault() { for(;;); }

void
fatal(const RtosErrorId error_id)
{
    debug_print("FATAL ERROR: ");
    debug_printhex32(error_id);
    debug_println("");
    for (;;)
    {
    }
}

void
reset_timer() {
    REGISTER(SCB_DEMCR) = REGISTER(SCB_DEMCR) | 0x01000000;
    REGISTER(DWT_CYCCNT) = 0;
    REGISTER(DWT_CONTROL) = 0;
}

void
start_timer() {
    REGISTER(DWT_CONTROL) = REGISTER(DWT_CONTROL) | 1;
}

void
stop_timer() {
    REGISTER(DWT_CONTROL) = REGISTER(DWT_CONTROL) & (~1);
}

uint32_t
get_cycles() {
    return REGISTER(DWT_CYCCNT);
}

#define NUM_ITERATIONS 10000

void
fn_a(void)
{
    dom1_variable_1 = 3;
    ++dom1_variable_2;

    uint32_t start = 0, end = 0;

    PRIVILEGED_ACTION(start = get_cycles());

    for (uint32_t i = 0; i != NUM_ITERATIONS; ++i)
    {
        rtos_yield_to(1);
    }

    PRIVILEGED_ACTION(end = get_cycles());

    debug_print("start: ");
    debug_printhex32(start);
    debug_print(" - end: ");
    debug_printhex32(end);
    debug_print(" - delta is (per context switch): ");
    debug_printhex32((end - start)/(NUM_ITERATIONS*2));
    debug_println("");

    for(;;);
}

void
fn_b(void)
{
    dom2_variable_1 = 3;
    ++dom2_variable_2;
    for (;;)
    {
        rtos_yield_to(0);
    }
}

int
main(void)
{
    reset_timer();
    start_timer();
    rtos_start();
    for (;;) ;
}
