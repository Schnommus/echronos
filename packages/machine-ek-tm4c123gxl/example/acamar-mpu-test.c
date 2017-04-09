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

extern void debug_println(const char *msg);

void fn_a(void);
void fn_b(void);
void fatal(RtosErrorId error_id);

/* domX_variable_2 is unused in this example, but remains to
 * illustrate usage of multiple symbols per domain */

uint32_t dom1_variable_1 = 0;
uint32_t dom1_variable_2 = 0;

uint32_t dom2_variable_1 = 0;
uint32_t dom2_variable_2 = 0;

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

void nmi() { for(;;); }

void hardfault() { for(;;); }

void busfault() { for(;;); }

void usagefault() { for(;;); }

extern uint8_t rtos_internal_current_task;

void iteration() {
    debug_print("current task (API): ");
    debug_printhex32(rtos_task_current());
    debug_println("");


    /* One of these increments should fault depending on
     * which task is currently executing */

    debug_println("Increment dom1_variable_1...");
    ++dom1_variable_1;

    debug_println("Increment dom2_variable_1...");
    ++dom2_variable_1;

    /* Both of these reads should succeed, both tasks have read access */

    debug_print("dom1_variable_1=");
    debug_printhex32(dom1_variable_1);
    debug_println("");

    debug_print("dom2_variable_1=");
    debug_printhex32(dom2_variable_1);
    debug_println("");
}

void
fn_a(void)
{

    debug_print("current task (KERNEL DATA): ");
    debug_printhex32(rtos_internal_current_task); /* should fault */
    debug_println("");

    for (;;)
    {
        iteration();
        rtos_yield_to(1);
    }
}

void
fn_b(void)
{
    for (;;)
    {
        iteration();
        rtos_yield_to(0);
    }
}

int
main(void)
{
    rtos_start();
    for (;;) ;
}
