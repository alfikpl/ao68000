/*
 * linux/arch/m68knommu/platform/ao68000/ints.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 *
 * Copyright 1996 Roman Zippel
 * Copyright 1999 D. Jeff Dionne <jeff@rt-control.com>
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/traps.h>
#include <asm/io.h>
#include <asm/machdep.h>

#if defined(CONFIG_M68328)
#include <asm/MC68328.h>
#elif defined(CONFIG_M68EZ328)
#include <asm/MC68EZ328.h>
#elif defined(CONFIG_M68VZ328)
#include <asm/MC68VZ328.h>
#endif

/* assembler routines */
asmlinkage void system_call(void);
asmlinkage void buserr(void);
asmlinkage void trap(void);
asmlinkage void trap3(void);
asmlinkage void trap4(void);
asmlinkage void trap5(void);
asmlinkage void trap6(void);
asmlinkage void trap7(void);
asmlinkage void trap8(void);
asmlinkage void trap9(void);
asmlinkage void trap10(void);
asmlinkage void trap11(void);
asmlinkage void trap12(void);
asmlinkage void trap13(void);
asmlinkage void trap14(void);
asmlinkage void trap15(void);
asmlinkage void trap33(void);
asmlinkage void trap34(void);
asmlinkage void trap35(void);
asmlinkage void trap36(void);
asmlinkage void trap37(void);
asmlinkage void trap38(void);
asmlinkage void trap39(void);
asmlinkage void trap40(void);
asmlinkage void trap41(void);
asmlinkage void trap42(void);
asmlinkage void trap43(void);
asmlinkage void trap44(void);
asmlinkage void trap45(void);
asmlinkage void trap46(void);
asmlinkage void trap47(void);
asmlinkage irqreturn_t bad_interrupt(int, void *);
asmlinkage irqreturn_t inthandler(void);
asmlinkage irqreturn_t inthandler1(void);
asmlinkage irqreturn_t inthandler2(void);
asmlinkage irqreturn_t inthandler3(void);
asmlinkage irqreturn_t inthandler4(void);
asmlinkage irqreturn_t inthandler5(void);
asmlinkage irqreturn_t inthandler6(void);
asmlinkage irqreturn_t inthandler7(void);

extern e_vector *_ramvec;

/* The number of spurious interrupts */
volatile unsigned int num_spurious;

/* The 68k family did not have a good way to determine the source
 * of interrupts until later in the family.  The EC000 core does
 * not provide the vector number on the stack, we vector everything
 * into one vector and look in the blasted mask register...
 * This code is designed to be fast, almost constant time, not clean!
 */
asmlinkage void process_int(int vec, struct pt_regs *fp)
{
	do_IRQ(vec, fp);
}

static void intc_irq_unmask(unsigned int irq)
{
}

static void intc_irq_mask(unsigned int irq)
{
}

static struct irq_chip intc_irq_chip = {
	.name		= "M68K-INTC",
	.mask		= intc_irq_mask,
	.unmask		= intc_irq_unmask,
};

/*
 * This function should be called during kernel startup to initialize
 * the machine vector table.
 */
void __init init_IRQ(void)
{
	int i;
	
	_ramvec[2] = (e_vector) buserr;
	
	/* set up the vectors */
	_ramvec[3] = (e_vector) trap3;
	_ramvec[4] = (e_vector) trap4;
	_ramvec[5] = (e_vector) trap5;
	_ramvec[6] = (e_vector) trap6;
	_ramvec[7] = (e_vector) trap7;
	_ramvec[8] = (e_vector) trap8;
	_ramvec[9] = (e_vector) trap9;
	_ramvec[10] = (e_vector) trap10;
	
	for (i = 11; i < 24; ++i) _ramvec[i] = (e_vector) trap;

	_ramvec[25] = (e_vector) inthandler1;
	_ramvec[26] = (e_vector) inthandler2;
	_ramvec[27] = (e_vector) inthandler3;
	_ramvec[28] = (e_vector) inthandler4;
	_ramvec[29] = (e_vector) inthandler5;
	_ramvec[30] = (e_vector) inthandler6;
	_ramvec[31] = (e_vector) inthandler7;

	_ramvec[32] = (e_vector) system_call;
	
	for(i=33; i<256; i++) _ramvec[i] = (e_vector) trap; //bad_interrupt
	
	for (i = 0; (i < NR_IRQS); i++) {
		irq_desc[i].status = IRQ_DISABLED;
		irq_desc[i].action = NULL;
		irq_desc[i].depth = 1;
		irq_desc[i].chip = &intc_irq_chip;
	}
}
