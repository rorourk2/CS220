#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "process.h"
#include "global.h"
#include "swapfile.h"
#include "tlb.h"
#include "macros.h"

/*******************************************************************************
 * TASK 5: Implement the pagefault handler.
 * Page fault handler. When the CPU encounters an invalid address mapping in a
 * process' page table, it invokes OS through this handler. The OS then
 * allocates a physical frame for the requested page (either by using a free
 * frame or evicting one), changes the process' page table to reflect the
 * mapping and then restarts the interrupted process.
 *
 * @param vpn The virtual page number requested.
 * @param write If the CPU is writing to the page, this is 1. Otherwise, it's 0.
 * @return The physical frame the OS has mapped to the virtual page.
 */
pfn_t pagefault_handler(vpn_t request_vpn, int write)
{
	pfn_t victim_pfn;
	vpn_t v;
	victim_pfn = 0;
	/* TASK 5a: Find a victim frame. Hint: Refer page-replacement.c.  */
	/* Use the reverse lookup table to find the victim's owner process and corresponding virtual page. */
	/*if((rlt[victim_pfn].pcb)!=NULL){
		while(x<CPU_NUM_PTE){
			if(rlt[victim_pfn].pcb->pagetable[x].vpn==v){
				if(IS_SET(rlt[victim_pfn].pcb->pagetable[x].flags,1)){
					page_to_disk(victim_pfn, v, rlt[victim_pfn].pcb->pid);
				}
				CLEAR_BIT(rlt[victim_pfn].pcb->pagetable[x].flags,0);
				tlb_clearone(v);
			}
			x++;
		}
	}*/
	/*while(victim_pfn<NUM_PHYS_PAGES){
		if((rlt[victim_pfn].vpn)==request_vpn){
			break;
		}
		victim_pfn++;
	}*/
	victim_pfn = get_free_frame();
	v=rlt[victim_pfn].vpn;
	
	if((rlt[victim_pfn].pcb)!=NULL){
		if(IS_SET(rlt[victim_pfn].pcb->pagetable[v].flags,1)){
			page_to_disk(victim_pfn, v, rlt[victim_pfn].pcb->pid);
		}
		CLEAR_BIT(rlt[victim_pfn].pcb->pagetable[v].flags,0);
		tlb_clearone(v);
	}
	/*
	 * TASK 5b: If victim page is occupied:
	 *
	 * 1) If it's dirty, save it to disk with page_to_disk()
	 * 2) Invalidate the page's entry in the victim's page table.
	 * 3) Clear the victim page's TLB entry (hint: tlb_clearone()).
	 */
	
	/* TASK 5c: Update the reverse lookup table so that the frame's ownership is updated to the requesting process (which is nothing but the current process!). 
	 * Update the requesting process' page table. Set the page table entry as used and valid. Also, if writing to the page, set the dirty bit. 
	 *
	 * Finally, retrieve the page from disk. Note that is really a lie: we save pages in
	 * memory (since doing file I/O for this simulation would be annoying and
	 * wouldn't add that much to the learning). Also, if the page technically
	 * does't exist yet (i.e., the page has never been accessed yet, we return a
	 * blank page. Real systems would check for invalid pages and possibly read
	 * stuff like code pages from disk. For purposes of this simulation, we won't
	 * worry about that. =)
	 * Hint: page_from_disk() 
	 */
	rlt[victim_pfn].pcb=current;
	/*while(x<CPU_NUM_PTE){
			if(current->pagetable[x].vpn==request_vpn){
				SET_BIT(current->pagetable[x].flags,0);
				SET_BIT(current->pagetable[x].flags,2);
				if(write==1){
					SET_BIT(current->pagetable[x].flags,1);
				}
			}
			x++;
	}*/
	SET_BIT(current->pagetable[request_vpn].flags,0);
	SET_BIT(current->pagetable[request_vpn].flags,2);
	if(write==1){
		SET_BIT(current->pagetable[request_vpn].flags,1);
	}
	page_from_disk(victim_pfn, request_vpn, current->pid);
	return victim_pfn;
}

