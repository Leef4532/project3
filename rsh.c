#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	for(int i=0;i<N;i++){
		if(strcmp(cmd, allowed[i])==0){
			return 1;
		}
	}
			
	return 0;
}

int main() {
  

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	char *argv[22];
	int argc =0;
	char *token = strtok(line, " \t");
	while(token!= NULL && argc < 21){
		argv[argc++] = token;
		token = strtok(NULL, " \t");
	}
	argv[argc] = NULL;

	if(argc ==0){
		continue;
	}
	

	if(!isAllowed(argv[0])){
		printf("NOT ALLOWED!\n");
		continue;
	}
	if(strcmp(argv[0],"exit")== 0){
		return 0;
	}

	if(strcmp(argv[0],"help")==0){
		printf("The allowed commands are:\n");
		for(int i=0;i < N; i++){
			printf("%s\n",allowed[i]);
		}
	continue;
	}

	if(strcmp(argv[0],"cd")==0){
		if(argc>2){
			printf("-rsh: cd: too many arguments\n");
		}else if(argc == 2){
			if(chdir(argv[1])!=0){
				printf("-rsh: cd: %s: No such file or directory\n", argv[1]);
			}
		}
		continue;
	}

	pid_t pid;
	int status;
	
	if(posix_spawnp(&pid,argv[0], NULL, NULL,argv, environ)!=0){
		printf("Spawn failed\n");
		continue;
	}

	if(waitpid(pid, &status, 0) == -1){
		printf("waitpid failed\n");
		continue;
	}
		

	
    }
    return 0;
}
		
	
