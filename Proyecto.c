#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
	int fd[2];
	pid_t pid;
	char string[21];
	char readbuffer[25]; 
        char command[50];

	pipe(fd); // crea tuberia

	if((pid = fork()) == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(pid) {
		close(fd[0]);
		while(1){
			fgets(string, 21, stdin); // lee comando
			
			// funcion que detecta sie es un comado valido aqui
			
			// si no es un comando valido imprimir "Comando invalido" y seguir al siguiente comando
			//y si es valido ejecuta esto:
			if(strcmp(string, "salir\n")==0){ // si el comando es "salir" termina proceso padre
				break;
			}else{
				write(fd[1], string, strlen(string) +1); // envia comando a hijo
			}
			
		}
		kill(pid, SIGKILL); // terminar proceso hijo
		
			
	} else {
		close(fd[1]);
		while(1){
			read(fd[0], readbuffer, sizeof(readbuffer)); // recibe comando
			system(readbuffer); // ejecuta comando
		}
		exit(EXIT_SUCCESS);
	
		
	}

	exit(EXIT_SUCCESS);
}
