/*
 * uCbootstrap.h:  uCbootstrap and uCbootloader system call interface
 *
 * (c) Copyright 1999 Rt-Control Inc.
 * (c) Copyright 2001 Arcturus Networks Inc.
 * Maintained by Michael Durrant <mdurrant@arcturusnetworks.com>
 *
 * Release notes:
 * 7 Nov 2001 by mdurrant@arcturusnetworks.com
 *            - moved old uCbootloader.h file from the 
 *              uCdimm platform directory into linux/asm-m68knommu/
 *            - combined the bootstd.h and flash.h file from uCsimm
 *              and uCdimm into a common uCbootloader.h file.
 *
 */

#ifndef __UCBOOTSTRAP_H__
#define __UCBOOTSTRAP_H__

#define NR_BSC 23            /* last used bootloader system call */

#define __BN_reset		0  /* reset and start the bootloader */
#define __BN_test		1  /* tests the system call interface */
#define __BN_exec		2  /* executes a bootloader image */
#define __BN_exit		3  /* terminates a bootloader image */
#define __BN_program		4  /* program FLASH from a chain */
#define __BN_erase		5  /* erase sector(s) of FLASH */
#define __BN_open		6
#define __BN_write		7
#define __BN_read		8
#define __BN_close		9
#define __BN_mmap		10 /* map a file descriptor into memory */
#define __BN_munmap		11 /* remove a file to memory mapping */
#define __BN_gethwaddr		12 /* get the hardware address of my interfaces */
#define __BN_getserialnum	13 /* get the serial number of this board */
#define __BN_getbenv		14 /* get a bootloader envvar */
#define __BN_setbenv		15 /* get a bootloader envvar */
#define __BN_setpmask		16 /* set the protection mask */
#define __BN_readbenv		17 /* read environment variables */
#define __BN_flash_chattr_range	18
#define __BN_flash_erase_range	19
#define __BN_flash_write_range	20
#define __BN_ramload		21 /* load kernel into RAM and exec */
#define __BN_program2		22 /* program second FLASH from a chain */


/* Calling conventions compatible to (uC)linux/frv
 * We use simmilar macros to call into the bootloader as for uClinux
 */

#define __bsc_return(type, res)						\
do {									\
	if ((unsigned long)(res) >= (unsigned long)(-64)) {		\
		/* let errno be a function, preserve res in %d0 */	\
		int __err = -(res);					\
		errno = __err;						\
		res = -1;						\
	}								\
	return (type)(res);						\
} while (0)

/*
 * zero argument boot loader call stub
 */
#define _bsc0(type, name)				\
type name(void)						\
{							\
	register long __err;				\
	register type __res asm("gr7");			\
							\
	asm volatile("	tira	gr0,#126	\n"	\
		     "	setlos	#4095,%1	\n"	\
		     "	add	%0,%1,%1	\n"	\
		     : "=r"(__res), "=r"(__err)		\
		     : "0" (__NR_##name)		\
		     );					\
	if (unlikely((long) __err < 4095))		\
		goto error_handler;			\
							\
	return __res;					\
							\
 error_handler:						\
	errno = 4095 - __err;				\
	return (type) 0xffffffffUL;			\
}

/*
 * one argument boot loader call stub
 */
#define _bsc1(type, name, atype, _a)			\
type name(atype _a)					\
{							\
	register long __err;				\
	register type __res asm("gr7");			\
	register atype a asm("gr8") = _a;		\
							\
	asm volatile("	tira	gr0,#126	\n"	\
		     "	setlos	#4095,%1	\n"	\
		     "	add	%0,%1,%1	\n"	\
		     : "=r"(__res), "=r"(__err)		\
		     : "0" (__NR_##name),		\
		     "r" (a)				\
		     );					\
	if (unlikely((long) __err < 4095))		\
		goto error_handler;			\
							\
	return __res;					\
							\
 error_handler:						\
	errno = 4095 - __err;				\
	return (type) 0xffffffffUL;			\
}

/*
 * two argument boot loader call stub
 */
#define _bsc2(type, name, atype, _a, btype, _b)		\
type name(atype _a,					\
	  btype _b)					\
{							\
	register long __err;				\
	register type __res asm("gr7");			\
	register atype a asm("gr8") = _a;		\
	register atype b asm("gr9") = _b;		\
							\
	asm volatile("	tira	gr0,#126	\n"	\
		     "	setlos	#4095,%1	\n"	\
		     "	add	%0,%1,%1	\n"	\
		     : "=r"(__res), "=r"(__err)		\
		     : "0" (__NR_##name),		\
		     "r" (a),				\
		     "r" (b)				\
		     );					\
	if (unlikely((long) __err < 4095))		\
		goto error_handler;			\
							\
	return __res;					\
							\
 error_handler:						\
	errno = 4095 - __err;				\
	return (type) 0xffffffffUL;			\
}

/*
 * three argument boot loader call stub
 */
#define _bsc3(type, name, atype, _a, btype, _b, ctype, _c)	\
type name(atype _a,						\
	  btype _b,						\
	  ctype _c)						\
{								\
	register long __err;					\
	register type __res asm("gr7");				\
	register atype a asm("gr8") = _a;			\
	register btype b asm("gr9") = _b;			\
	register ctype c asm("gr10") = _c;			\
								\
	asm volatile("	tira	gr0,#126	\n"		\
		     "	setlos	#4095,%1	\n"		\
		     "	add	%0,%1,%1	\n"		\
		     : "=r"(__res), "=r"(__err)			\
		     : "0" (__NR_##name),			\
		     "r" (a),					\
		     "r" (b),					\
		     "r" (c)					\
		     );						\
	if (unlikely((long) __err < 4095))			\
		goto error_handler;				\
								\
	return __res;						\
								\
 error_handler:							\
	errno = 4095 - __err;					\
	return (type) 0xffffffffUL;				\
}

/*
 * four argument boot loader call stub
 */
#define _bsc4(type, name, atype, _a, btype, _b, ctype, _c, dtype, _d)		\
type name(atype _a,								\
	  btype _b,								\
	  ctype _c,								\
	  dtype _d)								\
{										\
	register long __err;							\
	register type __res asm("gr7");						\
	register atype a asm("gr8") = _a;					\
	register btype b asm("gr9") = _b;					\
	register ctype c asm("gr10") = _c;					\
	register dtype d asm("gr11") = _d;					\
										\
	asm volatile("	tira	gr0,#126	\n"				\
		     "	setlos	#4095,%1	\n"				\
		     "	add	%0,%1,%1	\n"				\
		     : "=r"(__res), "=r"(__err)					\
		     : "0" (__NR_##name),					\
		     "r" (a),							\
		     "r" (b),							\
		     "r" (c),							\
		     "r" (d)							\
		     );								\
	if (unlikely((long) __err < 4095))					\
		goto error_handler;						\
										\
	return __res;								\
										\
 error_handler:									\
	errno = 4095 - __err;							\
	return (type) 0xffffffffUL;						\
}

/*
 * five argument boot loader call stub
 */
#define _bsc5(type, name, atype, _a, btype, _b, ctype, _c, dtype, _d, etype, _e)	\
type name(atype _a,									\
	  btype _b,									\
	  ctype _c,									\
	  dtype _d,									\
	  etype _e)									\
{											\
	register long __err;								\
	register type __res asm("gr7");							\
	register atype a asm("gr8") = _a;						\
	register btype b asm("gr9") = _b;						\
	register ctype c asm("gr10") = _c;						\
	register dtype d asm("gr11") = _d;						\
	register etype e asm("gr12") = _e;						\
											\
	asm volatile("	tira	gr0,#126	\n"					\
		     "	setlos	#4095,%1	\n"					\
		     "	add	%0,%1,%1	\n"					\
		     : "=r"(__res), "=r"(__err)						\
		     : "0" (__NR_##name),						\
		     "r" (a),								\
		     "r" (b),								\
		     "r" (c),								\
		     "r" (d),								\
		     "r" (e)								\
		     );									\
	if (unlikely((long) __err < 4095))						\
		goto error_handler;							\
											\
	return __res;									\
											\
 error_handler:										\
	errno = 4095 - __err;								\
	return (type) 0xffffffffUL;							\
}

/* Command codes for the flash_command routine */
#define FLASH_SELECT    0       /* no command; just perform the mapping */
#define FLASH_RESET     1       /* reset to read mode */
#define FLASH_READ      1       /* reset to read mode, by any other name */
#define FLASH_AUTOSEL   2       /* autoselect (fake Vid on pin 9) */
#define FLASH_PROG      3       /* program a word */
#define FLASH_CERASE    4       /* chip erase */
#define FLASH_SERASE    5       /* sector erase */
#define FLASH_ESUSPEND  6       /* suspend sector erase */
#define FLASH_ERESUME   7       /* resume sector erase */
#define FLASH_UB        8       /* go into unlock bypass mode */
#define FLASH_UBPROG    9       /* program a word using unlock bypass */
#define FLASH_UBRESET   10      /* reset to read mode from unlock bypass mode */
#define FLASH_LASTCMD   10      /* used for parameter checking */

/* Return codes from flash_status */
#define STATUS_READY    0       /* ready for action */
#define STATUS_BUSY     1       /* operation in progress */
#define STATUS_ERSUSP   2       /* erase suspended */
#define STATUS_TIMEOUT  3       /* operation timed out */
#define STATUS_ERROR    4       /* unclassified but unhappy status */

/* manufacturer ID */
#define AMDPART   	0x01
#define TOSPART		0x98
#define SSTPART		0xbf
#define ATPART		0x1f

/* A list of 4 AMD device ID's - add others as needed */
#define ID_AM29DL800T   0x224A
#define ID_AM29DL800B   0x22CB
#define ID_AM29LV800T   0x22DA
#define ID_AM29LV800B   0x225B
#define ID_AM29LV160B   0x2249
#define ID_AM29LV160T   0x22C4
#define ID_TC58FVT160	0x00C2
#define ID_TC58FVB160	0x0043
#define ID_TC58FVB160X2	0x00430043
#define ID_AM29LV400B   0x22BA
#define ID_AT49BV1614   0x00C0
#define ID_SST39VF160   0x2782
/* definition for arm7tdni */
#define ID_AT27LV1024	0x0000
#define ID_AT29LV1024	0x0001

#define SECTOR_DIRTY   0x01
#define SECTOR_ERASED  0x02
#define SECTOR_PROTECT 0x04

#define PGM_ERASE_FIRST 0x0001
#define PGM_RESET_AFTER 0x0002
#define PGM_EXEC_AFTER  0x0004
#define PGM_HALT_AFTER  0x0008
#define PGM_DEBUG       0x0010

/* an mnode points at 4k pages of data through an offset table. */
typedef struct _memnode {
	int	len;
	int	*offset;
} mnode_t;

#endif /* __UCBOOTSTRAP_H__ */
