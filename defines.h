/*
 * This file is part of 'pdp', a PDP-11 simulator.
 *
 * For information contact:
 *
 *   Computer Science House
 *   Attn: Eric Edwards
 *   Box 861
 *   25 Andrews Memorial Drive
 *   Rochester, NY 14623
 *
 * Email:  mag@potter.csh.rit.edu
 * FTP:    ftp.csh.rit.edu:/pub/csh/mag/pdp.tar.Z
 * 
 * Copyright 1994, Eric A. Edwards
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  Eric A. Edwards makes no
 * representations about the suitability of this software
 * for any purpose.  It is provided "as is" without expressed
 * or implied warranty.
 */

/*
 * defines.h
 */


/*
 * Stuff to maintain compatibility across platforms.
 */


#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>		/* COMMENT for vax-bsd */
#include <sgtty.h>
#include <ctype.h>
#include <fcntl.h>
/*#include <sys/select.h>	UNCOMMENT for AIX */


#if defined(sparc) || defined(m88k)	/* ADD AIX here */
#define SWAB
#endif


/*
 * Type definitions for PDP data types.
 */

typedef unsigned long c_addr;	/* core or unibus address (18 bit) */
typedef unsigned short l_addr;	/* logical address (16 bit) */
typedef unsigned short d_word;	/* data word (16 bit) */
typedef unsigned char d_byte;	/* data byte (8 bit) */


/*
 * PDP processor defines.
 */


#define SP	6	/* stack pointer */
#define PC	7	/* program counter */


typedef struct _pdp_regs {
	d_word regs[8];		/* general registers */
	d_word stacks[4];	/* stack pointers */
	d_word mmr0;		/* memory management register 0 */
	d_word mmr2;		/* memory management register 2 */
	d_word psw;		/* processor status word */
	d_word sr;		/* switch register */
	d_word kisa[8];		/* kernel instruction space address regs */
	d_word kisd[8];		/* kernel instruction space desc regs */
	d_word uisa[8];		/* user instruction space address regs */
	d_word uisd[8];		/* user instruction space desc regs */
	d_word ir;		/* current instruction register */
	d_word ea_addr;		/* stored address for dest modifying insts */
	unsigned long total;	/* count of instructions executed */
	unsigned look_time;	/* when to handle things, saves time */
} pdp_regs;


/*
 * Number of KL11s
 */

#define NUM_KL	9


/*
 * Definitions for the memory map and memory operations.
 */


#define MMAP_SIZE	100	/* number of devices */


#define OK		0	/* memory and instruction results */
#define ODD_ADDRESS	1
#define BUS_ERROR	2
#define MMU_ERROR	3
#define CPU_ILLEGAL	4
#define CPU_HALT	5
#define CPU_WAIT	6
#define CPU_NOT_IMPL	7
#define CPU_TRAP	8
#define CPU_EMT		9
#define CPU_BPT		10
#define CPU_IOT		11
#define CPU_RTT		12
#define CPU_TURBO_FAIL	13

#define MMU_NORMAL	0	/* mmu mapping modes */
#define MMU_PREVIOUS	1
#define MMU_KERNEL	2


/*
 * Unibus device addresses.
 */

#define MMU_KISD	0772300
#define MMU_KISD_SIZE	8
#define MMU_KISA	0772340
#define MMU_KISA_SIZE	8
#define MMU_UISD	0777600
#define MMU_UISD_SIZE	8
#define MMU_UISA	0777640
#define MMU_UISA_SIZE	8
#define MMU_MMR0	0777572
#define MMU_MMR0_SIZE	1
#define MMU_MMR2	0777576
#define MMU_MMR2_SIZE	1
#define KL11_CON	0777560
#define KL11_CON_SIZE	4
#define KL11_TTY	0776500
#define KL11_TTY_SIZE	32
#define	RL11		0774400
#define RL11_SIZE	4
#define TM11		0772520
#define TM11_SIZE	6
#define KW11		0777546
#define KW11_SIZE	1
#define PDP_SR		0777570
#define PDP_SR_SIZE	1
#define PDP_PS		0777776
#define PDP_PS_SIZE	1
#define BOOT		0773000
#define BOOT_SIZE	256
#define PDP_MEM		0
#define PDP_MEM_SIZE	(124 * 1024)
#define LP11		0777514
#define LP11_SIZE	2
#define RTC		0777340		/* should be moved */
#define RTC_SIZE	12
#define WD		0764000		/* should be moved */
#define WD_SIZE		10

extern int boot_read(), boot_write(), boot_bwrite();
extern int pdp_init(), pdp_read(), pdp_write(), pdp_bwrite();
extern int ub_null();
extern int kl_init(), kl_read(), kl_write(), kl_bwrite();
extern int kw_init(), kw_read(), kw_write(), kw_bwrite();
extern int rl_init(), rl_read(), rl_write(), rl_bwrite();
extern int tm_init(), tm_read(), tm_write(), tm_bwrite();
extern int lp_init(), lp_read(), lp_write(), lp_bwrite();
extern int rtc_read(), rtc_write(), rtc_bwrite();
extern int wd_init(), wd_read(), wd_write(), wd_bwrite();


/*
 * Defines for the event handling system.
 */

#define NUM_PRI		8
#define NUM_EVENTS	100

typedef struct _event {
	unsigned priority;		/* priority */
	int (*handler)();		/* handler function */
	d_word info;			/* info or vector number */
	struct timeval when;		/* when to fire this event */
	struct _event *next[NUM_PRI];	/* next in the chain */
} event;


/*
 * Instruction Table for Fast Decode.
 */

struct _itab {
	int (*func)();
};


/*
 * Global variables.
 */

extern struct timeval real_time;
extern int ui_done;
extern pdp_regs pdp;
extern event *event_list[NUM_PRI];
extern char rl_gpath[BUFSIZ];
extern char tm_gpath[BUFSIZ];
extern char lp_gpath[BUFSIZ];
extern char kl_gpath[NUM_KL][BUFSIZ];
extern struct _itab itab[];


/*
 * Inline defines.
 */


#define CC_N	010
#define CC_Z	04
#define CC_V	02
#define CC_C	01

#define CLR_CC_V()	p->psw &= ~CC_V
#define CLR_CC_C()	p->psw &= ~CC_C
#define CLR_CC_Z()	p->psw &= ~CC_Z
#define CLR_CC_N()	p->psw &= ~CC_N
#define CLR_CC_ALL()	p->psw &= ~(CC_V|CC_C|CC_Z|CC_N)

#define SET_CC_V()	p->psw |= CC_V
#define SET_CC_C()	p->psw |= CC_C
#define SET_CC_Z()	p->psw |= CC_Z
#define SET_CC_N()	p->psw |= CC_N

#define SRC_MODE	(( p->ir & 07000 ) >> 9 )
#define SRC_REG		(( p->ir & 0700 ) >> 6 )
#define DST_MODE	(( p->ir & 070 ) >> 3 )
#define DST_REG		( p->ir & 07 )

#define USER( psw )	((( psw ) & 0140000 ) == 0140000 )
#define KERNEL( psw )	((( psw ) & 0140000 ) == 0 )

#define LSBIT	1		/*  least significant bit */

#define	MPI	0077777		/* most positive integer */
#define MNI	0100000		/* most negative integer */
#define NEG_1	0177777		/* negative one */
#define SIGN	0100000		/* sign bit */
#define CARRY   0200000		/* set if carry out */

#define	MPI_B	0177		/* most positive integer (byte) */
#define MNI_B	0200		/* most negative integer (byte) */
#define NEG_1_B	0377		/* negative one (byte) */
#define SIGN_B	0200		/* sign bit (byte) */
#define CARRY_B	0400		/* set if carry out (byte) */

#define LOW16( data )	(( data ) & 0177777 )	/* mask the lower 16 bits */
#define LOW8( data )	(( data ) & 0377 )	/* mask the lower 8 bits */

#define CHG_CC_N( d )	if ((d) & SIGN ) \
					SET_CC_N(); \
				else \
					CLR_CC_N()

#define CHGB_CC_N( d )	if ((d) & SIGN_B ) \
				SET_CC_N(); \
			else \
				CLR_CC_N()

#define CHG_CC_Z( d )	if ( d ) \
					CLR_CC_Z(); \
				else \
					SET_CC_Z()

#define CHGB_CC_Z( d )	if ( LOW8( d )) \
				CLR_CC_Z(); \
			else \
				SET_CC_Z()

#define CHG_CC_C( d )	if ((d) & CARRY ) \
					SET_CC_C(); \
				else \
					CLR_CC_C()

#define CHG_CC_IC( d )	if ((d) & CARRY ) \
					CLR_CC_C(); \
				else \
					SET_CC_C()

#define CHGB_CC_IC( d )	if ((d) & CARRY_B ) \
				CLR_CC_C(); \
			else \
				SET_CC_C()

#define CHG_CC_V( d1, d2, d3 )	\
				if ((( d1 & SIGN ) == ( d2 & SIGN )) \
				&& (( d1 & SIGN ) != ( d3 & SIGN ))) \
					SET_CC_V(); \
				else \
					CLR_CC_V()

#define CHG_CC_VC( d1, d2, d3 )	\
				if ((( d1 & SIGN ) != ( d2 & SIGN )) \
				&& (( d2 & SIGN ) == ( d3 & SIGN ))) \
					SET_CC_V(); \
				else \
					CLR_CC_V()

#define CHG_CC_VS( d1, d2, d3 )	\
				if ((( d1 & SIGN ) != ( d2 & SIGN )) \
				&& (( d1 & SIGN ) == ( d3 & SIGN ))) \
					SET_CC_V(); \
				else \
					CLR_CC_V()

#define CHGB_CC_V( d1, d2, d3 )	\
				if ((( d1 & SIGN_B ) == ( d2 & SIGN_B )) \
				&& (( d1 & SIGN_B ) != ( d3 & SIGN_B ))) \
					SET_CC_V(); \
				else \
					CLR_CC_V()

#define CHGB_CC_VC(d1,d2,d3)	\
				if ((( d1 & SIGN_B ) != ( d2 & SIGN_B )) \
				&& (( d2 & SIGN_B ) == ( d3 & SIGN_B ))) \
					SET_CC_V(); \
				else \
					CLR_CC_V()

#define CHG_CC_V_XOR_C_N()	\
				if ((( p->psw & CC_C ) && \
				   ( p->psw & CC_N )) \
				|| ((!( p->psw & CC_C )) && \
				   ( ! ( p->psw & CC_N )))) \
					CLR_CC_V(); \
				else \
					SET_CC_V()
