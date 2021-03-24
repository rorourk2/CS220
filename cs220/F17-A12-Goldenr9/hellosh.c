/* hellosh.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main() {
	char line[1024];
	int pid, i;
	char *args[] = {&line, 0};
	int x=0;
	int y=0;
	char *v=NULL;
	while(1) {
		printf("Hello!!>");
		x=0;
		y=0;
		if(!fgets(line, 1023, stdin)) {
			break;
		}
		if(strcmp(line, "exit\n") == 0) break;
		/*args[0]=(char *)malloc(sizeof(char)*strlen(line));*/
		
		for(i = 0; i< strlen(line); i++) {
			/*args[x][y]=line[i];
			printf("1")*/;
			if(line[i] == '\n'){
				line[i] = '\0';
				/*args[x][y]=line[i];*/
			}
			/*y++;
			if(line[i] == ' ') {
				line[i] = '\0';
				strcpy(args[x],&line[y]);
				x++;
				y=i+1;
				args[x]=(char *)malloc(sizeof(char)*strlen(line));
			}*/
		}
		v=strtok(line," ");
		while(v!=NULL){
			args[x]=(char *)malloc(sizeof(char)*strlen(v));
			strcpy(args[x], v);
			v=strtok(NULL," ");
			x++;
		}
		args[x]=NULL;
		pid = fork();
		if(pid == 0) {
			/* This is the child */
			execvp(args[0], args);
			fprintf(stderr, "Hello!!: %s\n", strerror(errno));
			exit(errno);
		} else {
			/* This is the parent */
			wait(NULL);
		}
	}
	return 0;
}
