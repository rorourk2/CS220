#include <stdio.h>
#include <stdlib.h>

extern unsigned long *bt(void);
extern int main;
void print_backtrace(int count){
	unsigned long *x= (unsigned long *)&main;
	int t=0;
	unsigned char *v=0;
	int c=1;
	while(*v!=0xc3){
		v=(char *)(x+t);
		t++;
	}
	unsigned long *y= (x+t);
	unsigned long *curr_rbp= bt();
	unsigned long *ret_addr;
	while(count>0){
		ret_addr=(unsigned long*)*(curr_rbp +8);
		printf("#%i\t0x%16x\n",c, ret_addr);
		if(ret_addr>=x && ret_addr<=y){
			return;
		}
		curr_rbp=(unsigned long*)*(curr_rbp);
		count--;
		c++;
	}
}
