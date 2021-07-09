#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(void) {
	int *fd;
	pid_t pid;
	char string[22];
	char readbuffer[26]; 
        

	while(1){
		fd=(int*) calloc(2, sizeof(int));
		pipe(fd); // crea tuberia
		
		if((pid = fork()) == -1) { // Crea otro procesos
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if(pid) { // Parent
			// El proceso padre leer los comando a ejecutar y los manda al proceso hijo
			
			close(fd[0]);
				
			fgets(string, 20, stdin); // leer comando a ejecutar
			
			if(strcmp(string, "salir\n")==0){ // termina programa
				kill(pid, SIGKILL);
				break;
			}else{
				write(fd[1], string, strlen(string) + 1 ); // manda comando
			}
			
			
			wait(NULL);
				
		} else { // Child
			// El proceso hijo ejecuta el comando usando la funcion execvp()

			close(fd[1]);

			read(fd[0], readbuffer, sizeof(readbuffer)); // recibe comando
			
			char *nstr= strtok(readbuffer, "\n"); //
			
			//
			
			char *arg[21]={ // Crear arreglo de string vacio
				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
			};
			
			
			int oper=0;
			char *str1;
			char *str2;
			char *nstr2 = malloc(sizeof(char) * 20);
			//

			//
			//detecta operador y si encuentra un operador separa los comandos en dos strings diferentes
			strcpy(nstr2,nstr);
			str2= strstr(nstr2, "&&"); // encuentra operador "and"
			if(str2!=NULL){ 
				oper=1;
				str1= strtok(nstr, "&&");

			}else{
		
				str2= strstr(nstr2, "||"); // encuentra operador "or"
				if(str2!=NULL){
					oper=2;
					str1= strtok(nstr, "||");
				}else{
					str2= strstr(nstr2, "|"); // encuentra operador de tuberia
					if(str2!=NULL){
						oper=3;
						str1= strtok(nstr, "|");
					}
				}
			}
			if(oper==0){ strcpy(str1, nstr); } // si no ecuentra una operador se copia el comando completo en un solo string 
			//
			
			//
			// Para ejecutar execvp() se necesita un arreglo de strings que contenga el nombre del programa y los argumentos del comando
			// Asi que se separa la linea de comando en varias palabras y cada palabra se guarda en un arreglo de strings
			int i=0;
			char *token = strtok(str1, " ");
			while( token != NULL ) {
				arg[i]= (char *) malloc(20);
				strcpy(arg[i], token);
				token = strtok(NULL, " ");
				i++;
			}
			
			
			
			//
			if(oper==0){ // si no hay un operado ejecutar el comando
				if(execvp(arg[0], arg)==-1){printf( "error\n");}

			}
			else{
				//
				pid_t pid2;
				int j, cont; 
				
				if(oper==1){ // Para el operador "and"
					
					
					pid2 = fork(); // Crea otro proceso
					if(pid2){ 
						// primer proceso hijo ejecuta el primer comando
						if(execvp(arg[0], arg)==-1){printf( "error\n"); kill(pid2, SIGKILL);}	
					}
					else{
						//
						// guarda cada palabra del segundo comando en el areglo de string
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
						
						// si el segundo comando tiene menos argumentos que el primero 
						// establece el resto de los elementos del arreglo como vacios
						while( j<i ) { 
							arg[j]= (char *) calloc(20, sizeof(char));
							arg[j]= 0x00;
							j++;	
						}
						//
						
						// segundo proceso hijo ejecuta el segundo comando
						if(execvp(arg[0], arg)==-1){printf( "error\n");}
					}

				}
				else if(oper==2){ // Para el operador "or"
					if(execvp(arg[0], arg)==-1){printf( "error\n"); }
					
					//
					// si el prime comando fallo elecutar el segundo
					// guarda cada palabra del segundo comando en el areglo de string
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
					
					// Limpiar el resto del arreglo de strings
					while( j<i ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						arg[j]= 0x00;
						j++;	
					}
					//
					
					if(execvp(arg[0], arg)==-1){printf( "error\n"); }

				}else{ // Para el operador de tuberia
					int sfd;
					
					// Crear archivo "pipe"
					int rfd= open ("pipe",O_CREAT| O_TRUNC |O_RDWR, S_IRWXU);
					
					sfd= dup(STDOUT_FILENO);
					
					if(dup2(rfd, STDOUT_FILENO)== -1) {printf("error\n");}
					// escribir el resultado del comado en el archivo "pipe"
					close(rfd);

					pid2 = fork(); // Crea otro proceso
					if(!pid2){
						// primer proceso hijo ejecuta el primer comando
						if(execvp(arg[0], arg)==-1){printf( "error\n"); exit(EXIT_FAILURE);}	
					}
					wait(NULL);

					//
					j=0;
					token = strtok(str2, " ");
					token = strtok(NULL, " ");
					
					// guarda cada palabra del segundo comando en el areglo de string
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
					
					// Limpiar el resto del arreglo de strings
					while( j<i ) {
						arg[j]= (char *) calloc(20, sizeof(char));
						arg[j]= 0x00;
						j++;	
					}
					//
					
					dup2(sfd, STDOUT_FILENO);
					// escribir el resultado del segundo comado en la terminal
	
					// segundo proceso hijo ejecuta el segundo comando
					if(execvp(arg[0], arg)==-1){printf( "error\n"); exit(EXIT_FAILURE);}
					
				}
				

			}
			//
			
			exit(EXIT_SUCCESS);
		
		}

	}
	exit(EXIT_SUCCESS);
}
