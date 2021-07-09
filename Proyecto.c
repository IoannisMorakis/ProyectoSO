#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
/* #include <sys/types.h> */




int main(void) {
	int *fd;
	pid_t pid;
	char string[22];
	char readbuffer[26]; 
        


	while(1){
		fd=(int*) calloc(2, sizeof(int));
		pipe(fd); // crea tuberia
		
		if((pid = fork()) == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if(pid) { // Parent
			close(fd[0]);
			
				
			fgets(string, 20, stdin);

			
			if(strcmp(string, "salir\n")==0){ // termina programa
				
				kill(pid, SIGKILL);
				break;
			}else{
				write(fd[1], string, strlen(string) + 1 ); // manda comando
			}
			
			
			wait(NULL);
				
		} else { // Child
			close(fd[1]);

			read(fd[0], readbuffer, sizeof(readbuffer));
			
			char *nstr= strtok(readbuffer, "\n");

			//
			
			char *arg[21]={
				NULL, NULL
				
			};
			
			
			int oper=0;
			char *str1;
			char *str2;
			char *nstr2 = malloc(sizeof(char) * 20);
			//

			//
			//detecta operacion
			strcpy(nstr2,nstr);
			str2= strstr(nstr2, "&&");
			if(str2!=NULL){
				oper=1;
				str1= strtok(nstr, "&&");

			}else{
		
				str2= strstr(nstr2, "||");
				if(str2!=NULL){
					oper=2;
					str1= strtok(nstr, "||");
				}else{
					str2= strstr(nstr2, "|");
					if(str2!=NULL){
						oper=3;
						str1= strtok(nstr, "|");
					}
				}
			}
			if(oper==0){ strcpy(str1, nstr); }
			//
			
			//
			

			int i=0;
			char *token = strtok(str1, " ");
			while( token != NULL ) {
				arg[i]= (char *) malloc(20);
				strcpy(arg[i], token);
				token = strtok(NULL, " ");
				i++;
			}

			
			//
			if(oper==0){
				if(execvp(arg[0], arg)==-1){printf( "error\n");}

			}
			else{

				//
				pid_t pid2;
				int j, cont; 
				
				if(oper==1){
					pid2 = fork();
					if(pid2){
						if(execvp(arg[0], arg)==-1){printf( "error\n"); kill(pid2, SIGKILL);}	
					}
					else{
						//
						j=0;
						token = strtok(str2, " ");
						token = strtok(NULL, " ");
						while( token != NULL ) {
							arg[j]= (char *) calloc(20, sizeof(char));
							strcpy(arg[j], token);
							token = strtok(NULL, " ");
							j++;	
						}
						cont =j;
						
						while( j<i ) {
							arg[j]= (char *) calloc(20, sizeof(char));
							
							arg[j]= 0x00;
							j++;	
						}
						//
						
						if(execvp(arg[0], arg)==-1){printf( "error\n");}
					}

				}
				else if(oper==2){
					if(execvp(arg[0], arg)==-1){printf( "error\n"); }

					//
					j=0;
					token = strtok(str2, " ");
					token = strtok(NULL, " ");
					while( token != NULL ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						strcpy(arg[j], token);
						token = strtok(NULL, " ");
						j++;	
					}
					cont =j;
					while( j<i ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						arg[j]= 0x00;
						j++;	
					}
					//
					
					if(execvp(arg[0], arg)==-1){printf( "error\n"); }

				}else{
					
					int sfd;
					int rfd= open ("pipe",O_CREAT| O_TRUNC |O_RDWR, S_IRWXU);
					
					sfd= dup(STDOUT_FILENO);
					if(dup2(rfd, STDOUT_FILENO)== -1) {printf("error\n");}
					close(rfd);

					pid2 = fork();
					if(!pid2){
						if(execvp(arg[0], arg)==-1){printf( "error\n"); exit(EXIT_FAILURE);}	
					}
					wait(NULL);

					//
					j=0;
					token = strtok(str2, " ");
					token = strtok(NULL, " ");
					while( token != NULL ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						strcpy(arg[j], token);
						token = strtok(NULL, " ");
						j++;	
					}
					cont =j;
					arg[j]= (char *) calloc(20, sizeof(char));
					strcpy(arg[j], "pipe");
					j++;
					
					while( j<i ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						arg[j]= 0x00;
						j++;	
					}
					//
					
					dup2(sfd, STDOUT_FILENO);
					if(execvp(arg[0], arg)==-1){printf( "error\n"); exit(EXIT_FAILURE);}
				}
				
			}
			//
			
			
			exit(EXIT_SUCCESS);
		
		}
	}
	exit(EXIT_SUCCESS);
}
