/* loader.c */
#include <stdio.h>
#include <stdlib.h>
/*#include "func.bin"*/
/* TODO: Include appropriate headers */

int main(int argc, char *argv[]){
	/* TODO: Declare an array large enough to hold the raw bytes. Raw bytes are best stored in byte-addressable arrays. Pick the appropriate type. Call it "raw_bytes"*/
	/* TODO: Declare a function pointer type that matches the calc function's type. Call it  "Calc_fptr" */
	if(argc!=5){
		printf("Usage %s <filename> <uint> <operation> <uint>\n", argv[0]);
		return 0;
	}
	FILE *fp;
	fp=fopen("func.bin", "rb");
	fseek(fp, 0, SEEK_END);
	int l=ftell(fp);
	fclose(fp);
	
	unsigned int *raw_bytes=(unsigned int *)malloc(l);
	typedef int (*Calc_fptr)(char a,int x,int y);
	
	unsigned int i;
	Calc_fptr calculator;
	
	/* TODO if number of arguments is not 4 (5 including program name)
		print ("Usage %s <filename> <uint> <operation> <uint>\n", argv[0]) and exit */
	fp=fopen("func.bin", "rb");
	fread(raw_bytes,l,1,fp);
	fclose(fp);
	/* TODO: Open and read the binary file into raw_bytes. Use fopen and fread. */
	int x=atoi(argv[2]);
	int y=atoi(argv[4]);
	calculator = (Calc_fptr) raw_bytes;
	/* TODO: Print the result. Refer to sample input and output. */
	printf("%i %c %i = %i\n",x,argv[3][0],y,calculator((char)argv[3][0], x, y));
	return 0;
}
