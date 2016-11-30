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

#include "rtos-acamar-mpu.h"
#include "debug.h"

extern void debug_println(const char *msg);

void fn_a(void);
void fn_b(void);
void fatal(RtosErrorId error_id);

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
fn_a(void)
{
    for (;;)
    {
        debug_println("task a");
        rtos_yield_to(1);
    }
}

#define HWREG(x) (*((volatile uint32_t *)(x)))

void
fn_b(void)
{
    for (;;)
    {
        debug_println("task b");
        rtos_yield_to(0);
        HWREG(0x100) = 0;
    }
}

int
main(void)
{
    rtos_start();
    for (;;) ;
}
