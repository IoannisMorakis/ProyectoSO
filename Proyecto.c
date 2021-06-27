#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
	int fd[2];
	pid_t pid;
	char string[51];// 21
	char readbuffer[55]; // 25
        char command[50];

	pipe(fd);

	if((pid = fork()) == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(pid) {
		close(fd[0]);
		while(1){
			fgets(string, 50, stdin);
			
			if(strcmp(string, "salir\n")==0){
				break;
			}else{
				write(fd[1], string, strlen(string)+1);
			}
		}
		kill(pid, SIGKILL);
		
			
	} else {
		close(fd[1]);
		while(1){
			read(fd[0], readbuffer, sizeof(readbuffer));
			system(readbuffer);
		}
		exit(EXIT_SUCCESS);
	
		
	}

	exit(EXIT_SUCCESS);
}
