#include <stdlib.h>

#include "types.h"
#include "pagetable.h"
#include "global.h"
#include "process.h"
#include "macros.h"

/*******************************************************************************
 * Finds a free physical frame. If none are available, uses a clock sweep
 * algorithm to find a used frame for eviction.
 *
 * @return The physical frame number of a free (or evictable) frame.
 */

/* A physical page is also called a physical frame or frame */
pfn_t get_free_frame(void)
{
	/* TASK 4a: Iterate through the reverse lookup table to see if there is a free frame. If so, return it. 
	 * HINT: The pcb (or the owning process) for a free frame will be NULL. 
	 */
	int i;
	int x;
	i=0;
	x=0;
	while(i<NUM_PHYS_PAGES){
		if((rlt[i].pcb)==NULL){
			return (pfn_t) i;
		}
		i++;
	}
	/* TASK 4b: If none of the frames are free, we must evict a frame.
	 * For each frame, if it is used, set it as unused. If it is already unused, return the frame. 
	 * Repeat the previous step till you find an unused frame. 
	 */ 
	i=0;
	/*rlt[i].pcb->pagetable[x].pfn;*/
	/*while(i<NUM_PHYS_PAGES){
		while(x<CPU_NUM_PTE){
			if(!IS_SET(rlt[i].pcb->pagetable[x].flags,2)){
				return (pfn_t) x;
			}
			else{
				CLEAR_BIT(rlt[i].pcb->pagetable[x].flags,2);
			}
			x++;
		}
		i++;
	}*/
	while(1){
		while(i<NUM_PHYS_PAGES){
			if(!IS_SET(rlt[i].pcb->pagetable->pfn,USED)){
				return (pfn_t) x;
			}
			else{
				CLEAR_BIT(rlt[i].pcb->pagetable->pfn,USED);
			}
			x++;
		}
	}
	/* This final return is just to satisfy the compiler. 
	 * Control should never reach here. We should have found a frame earlier. 
	 */
	return 0;

}
