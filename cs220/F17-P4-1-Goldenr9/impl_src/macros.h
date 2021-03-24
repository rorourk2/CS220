#ifndef _MACROS_H
#define _MACROS_H

/* Bit positions */
#define VALID			0
#define DIRTY			1
#define USED			2
#define BIT(n) (1<<(n))
#define BIT_MASK(n)		(BIT(n))/*(BIT(n)-1)*//* TASK 1a: Define a mask with nth bit set */

/* bit corresponds to the bit number in flags. Least significant bit is 0 */
#define IS_SET(flags,bit)	((flags)&(BIT(bit)))/* TASK 1b: Test if bit is set in flags */
#define SET_BIT(flags,bit)	((flags) |= (1 << (bit)))/* TASK 1c: Set the bit in flags to 1 */
#define CLEAR_BIT(flags,bit)	((flags) &= ~(1 << (bit)))/* TASK 1d: Set the bit in flags to 0 */

#define VADDR_PAGENUM(addr) 	((addr)/(page_size))/*(addr >> 6) & BIT_MASK(10)   TASK 1e: Get the page number from a virtual address. */

#define VADDR_OFFSET(addr)  ((addr)%(page_size))/*(addr) & BIT_MASK(6)    TASK 1f: Get the page offset from a virtual address. */

#endif
