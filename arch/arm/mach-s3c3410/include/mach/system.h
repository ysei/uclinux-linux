/*
 *  arch/arm/mach-s3c3410/include/mach/system.h
 *
 *  Copyright (C) 2002 SAMSUNG ELECTRONICS
 *                       Hyok S. Choi <hyok.choi@samsung.com>
 *
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/hardware.h>
#include <asm/io.h>


static void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode)
{
 	/* machine should reboot */
 	mdelay(5000);
 	panic("Watchdog timer reset failed!\n");
 	printk(" Jump to address 0 \n");
 	cpu_reset(0);
}

#endif
