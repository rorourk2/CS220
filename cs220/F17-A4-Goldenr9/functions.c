#include "functions.h"
#include <stdio.h>
int cryptic_calculator(void *payload){
	char *i=(char *)payload;
	/*short *c1=(short *)(*(&payload+2));
	short *c2=(short *)(*(&payload+4));
	short *c3=(short *)(*(&payload+6));*/
	short *c1;
	short *c2;
	short *c3;
	c1= (payload+2);
	c2= (payload+4);
	c3= (payload+6);
	/*printf("%d%d%d\n",*c1,*c2,*c3);*/
	/*return *c1;*/
	if(*i=='*'){
		/*printf("i\n");*/
		int x= (int )((*c1)*(*c2)*(*c3));
		return x;
	}
	if(*i=='/' && *c1!=0){
		int x= (int )((*c2)/(*c1));
		return x;
	}
	return 0xBAD;
}

void my_memcpy(void *dst, void *src, unsigned int num_bytes){
  	int i=0;
	
	for(i=0;i<num_bytes;i++){
		char *dst=(char *)(src+i);
		dst=dst+1;
	}
}

void swap_strs(char *s1, char *s2){
	char *t;
  	while(*s1!='\0'){
		*t=*s1;
		*s1=*s2;
		*s2=*t;
		s1=s1+1;
		s2=s2+1;
	}
}
