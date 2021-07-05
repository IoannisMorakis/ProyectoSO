#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
	int *fd;
	pid_t pid;
	char string[52];
	char readbuffer[56]; 
        char command[50];


	while(1){
		fd=(int*) calloc(2, sizeof(int));
		pipe(fd); // crea tuberia
		
		if((pid = fork()) == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if(pid) { // Parent
			close(fd[0]);
			
			fgets(string, 51, stdin);

			// funcion que detecta sie es un comado valido aqui

			// si no es un comando valido imprimir "Comando invalido" y seguir al siguiente comando
			//y si es valido ejecuta esto:

			if(strcmp(string, "salir\n")==0){
				kill(pid, SIGKILL);
				break;
			}else{
				write(fd[1], string, strlen(string) + 1 );
			}
			
			wait(NULL);
				
		} else { // Child
			close(fd[1]);
				read(fd[0], readbuffer, sizeof(readbuffer));
				
				char *arg[] = {
					"sh", 
					"-c",
					readbuffer, 
					NULL
				};

				execvp(arg[0], arg);

		
		}
	}
	exit(EXIT_SUCCESS);
}
