#include "swapfile.h"
#include "stats.h"
#include "pagetable.h"
#include "macros.h"
/*******************************************************************************
 * Looks up an address in the current page table. If the entry for the given
 * page is not valid, traps to the OS.
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t pagetable_lookup(vpn_t vpn, int write) {
   pfn_t ret;
  /* int i;*/
   ret = 0;
   /* TASK 3: Within the current process's pagetable, look for the entry corresponding to vpn. 
    * 	      If the entry is valid, return the physical page (or frame). Otherwise, you have 
    *	      encountered a pagefault. Increment the counter and invoke the pagefault_handler to
    * 	      get the physical page. Finally return the page.
    */
	/*i=0;*/
	/*while(i<CPU_NUM_PTE){
		if(current_pagetable[i].vpn==vpn){
			if(IS_SET(current_pagetable[i].flags,0)){
				ret=current_pagetable[i].pfn;
			}
			else{
				pagefaults_count++;
				ret=pagefault_handler(vpn,write);
			}
		}
	}*/
	if(IS_SET(current_pagetable[vpn].flags,0)){
			ret=current_pagetable[vpn].pfn;
	}
	else{
		pagefaults_count++;
		ret=pagefault_handler(vpn,write);
	}
   return ret;
}
