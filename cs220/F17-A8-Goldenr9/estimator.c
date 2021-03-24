#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
extern nop_asm();
extern ret_asm();
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}
/*void timeval_print(char *str, struct timeval *tv)
{
    printf("%s %ld sec, %06ld micro sec\n", str, tv->tv_sec, tv->tv_usec);
}*/
void estimator(){
	struct timeval tvDiff1, tvStart, tvEnd, tvDiff2, tvDiff;
	unsigned int i=0;
	unsigned int j=0;
	unsigned long long av1,av2,av=0;
	while(j<5){
		gettimeofday(&tvStart, NULL);
		while(i<0xffffffff){
			nop_asm();
			i++;
		}
		gettimeofday(&tvEnd, NULL);
		timeval_subtract(&tvDiff1, &tvEnd, &tvStart);
		/*timeval_print("nop: ", &tvDiff1);*/
	
		i=0;
		gettimeofday(&tvStart, NULL);
		while(i<0xffffffff){
			ret_asm();
			i++;
		}
		gettimeofday(&tvEnd, NULL);
		timeval_subtract(&tvDiff2, &tvEnd, &tvStart);
		/*timeval_print(": ", &tvDiff2);*/
		timeval_subtract(&tvDiff, &tvDiff1, &tvDiff2);
		/*timeval_print(": ", &tvDiff);*/
		av1=429496729500000000/(((unsigned long long)(tvDiff.tv_sec)*1000000)+((unsigned long long)(tvDiff.tv_usec)));
		/*printf("%llu\n",av1);*/
		av=av+av1;
		i=0;
		j++;
	}
	av=av/5;
	/*printf("%llu\n",av);*/
	unsigned long long z=10;
	unsigned int t=0;
	while(1==1){
		if(av<z){
			av1=av/(z/10);
			av2=av%(z/10);
			break;
		}
		z=z*10;
		t=t+1;
	}
	printf("%llu.%llu x 10^%u\n",av1,av2,t);
}

/*int main(){
	estimator();
	return 0;
}*/
