

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h> 
#define NUM_HIJOS 3

void creaHijos(int nHij){
	int valor,j;
	for (j=0; j<nHij; j++){
 	valor = fork(); 
 	if(valor==0){
 		printf("Soy %d, PPID = %d, PID = %d\n", j,getppid(), getpid());
		sleep(10); 
 	} 
	else if(valor > 0){
		valor = wait(NULL);
		while (valor > 0) {	
			valor = wait(NULL);
		}
		exit(EXIT_SUCCESS);
	}
 	else if (valor == -1){ 
		perror("error en fork"); 
		exit(EXIT_FAILURE); 
 	} 
 } 
	
}

int main(int argc, char *argvc[]){
 int valor, i;
 int nHijos = atoi(argvc[1]);
 for (i=0; i<nHijos; i++){
 	valor = fork(); 
 	if (valor == 0){
		printf("Soy %d, PPID = %d, PID = %d\n", i,getppid(), getpid()); 
 		creaHijos(i);
				
		exit(EXIT_SUCCESS);
 	} 
 	else if (valor == -1){ 
		perror("error en fork"); 
		exit(EXIT_FAILURE); 
 	} 
 } 
  
 valor = wait(NULL);
 while (valor > 0) {
 	valor = wait(NULL); 
 } 
 if (valor == -1 && errno != ECHILD) { 
	perror("error en wait"); exit(EXIT_FAILURE); 
 }
}


