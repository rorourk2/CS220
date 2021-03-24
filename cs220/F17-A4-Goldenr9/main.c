#include <stdio.h>
#include "functions.h"
#include "functions.c"

int main(int argc, char const *argv[]) {
  	short s[4];
	s[0]=(char)'*';
	s[1]=(short)6;
	s[2]=(short)7;
	s[3]=(short)7;
	short *p=&s[0];
	/*void *p;
	char *p=(char *)'*';
	char *(&p+2)=(short *)6;
	char *(&p+4)=(short *)7;
	char *(&p+6)=(short *)7;*/
	
	int x=cryptic_calculator(p);
	printf("%d\n",x);
  return 0;
}
