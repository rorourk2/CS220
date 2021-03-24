#include <stdlib.h>
#include <stdio.h>
#include "tlb.h"
#include "pagetable.h"
#include "global.h" /* for tlb_size */
#include "stats.h"
#include "macros.h"

/*******************************************************************************
 * Looks up an address in the TLB. If no entry is found, attempts to access the
 * current page table via cpu_pagetable_lookup().
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */

pfn_t tlb_lookup(vpn_t vpn, int write)
{
	pfn_t pfn;
	int i;
	int x;
	pfn = 0;
	/*
	 * TASK 2a : Implement TLB lookup
	 * Search the TLB for the given VPN. Make sure to increment tlbhits counter if
	 * it was a hit!
	 */
	i=0;
	while (i<tlb_size){
		if( IS_SET(tlb[i].flags,0) && vpn==tlb[i].vpn){
			tlbhits_count++;
			if(write==1){
				SET_BIT(tlb[i].flags,1);
			}
			SET_BIT(tlb[i].flags,2);
			return tlb[i].pfn;
		}
		i++;
	}
	/*if( IS_SET(tlb[vpn].flags,0)){
			tlbhits_count++;
			if(write==1){
				SET_BIT(tlb[i].flags,1);
			}
			SET_BIT(tlb[i].flags,2);
			return tlb[i].pfn;
		}*/
	/* 
	 * STEPS: 
	 * Iterate through the TLB entries: 
	 *    If the entry is valid and the virtual page number is the same
	 * as what is provided as input, then:
	 *        Set the used bit (because we are using it)
	 * 	  If this is a write access, also set the dirty bit. 
	 * 	  We have found the physical page, so we are done. Return the physical page. 
	 */
	

	/* TASK 2b: If it was a TLB miss, call pagetable_lookup to obtain the physical page. */
	pfn=pagetable_lookup(vpn,write);
	/* TASK 2c: Evict an invalid entry and update the TLB with the new page */
	i=0;
	x=0;
	while (i<tlb_size){
		if( !IS_SET(tlb[i].flags,0) && vpn==tlb[i].vpn){
			SET_BIT(tlb[i].flags,0);
			tlb[i].pfn=pfn;
			x=1;
		}
		i++;
	}
	/* TASK 2d: If no invalid entry was found, we must kick out an unused entry */
	/* STEPS: 
	 * For each entry in the TLB:
	 *   If the entry is unused:
	 *      set the virtual and physical page mapping and mark the entry as used and valid.
	 * 	we have successfully completed the eviction process. we are done. Return the physical page. 
	 *   else:
	 * 	mark the entry as unused, so we can kick it out the next time it is encountered. This is called performing a clock-sweep, because we are mimicking a movement of time. 
	 * Repeat the for STEPS till we find a victim entry to kick out. 
	 */
	if(x==0){
		i=0;
		while (i<tlb_size){
			if( !IS_SET(tlb[i].flags,2)){
				SET_BIT(tlb[i].flags,0);
				SET_BIT(tlb[i].flags,2);
				tlb[i].pfn=pfn;
				tlb[i].vpn=vpn;
				break;
			}
			else{
				CLEAR_BIT(tlb[i].flags,2);
			}
			i++;
		}
	}
	return pfn;
}

