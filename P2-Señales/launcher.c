#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int pid;
/*MANEJADOR DEL LAUNCHER--------------------------------------------------------------------*/
void handler(int signo) 
{
	char m1[80]="Recibida señal SIGUSR1\n";
	char m2[80]="Recibida señal SIGUSR2\n";   

	if (signo == SIGUSR1)  	 	{
//	  printf("HOLA QUE HACE\n");
	  	kill(pid,15);  
 	  	}        
        
	if (signo == SIGUSR2)        
    		{
		int prio=getpriority(PRIO_PROCESS, pid);
		printf("%d\n",prio);
		setpriority(PRIO_PROCESS, pid, prio+1);
		prio=getpriority(PRIO_PROCESS, pid);
		printf("%d\n",prio);	          
    		}
//    printf("Estoy en el handler\n");
}

//---------------------------------------------------------------------------------
/*FUNCION MAIN--------------------------------------------------------------------*/
int main(int argc,char* argv[]) {

  struct sigaction act;
        
  /* Instalasmos el manejador para SIGINT y SIGSTP */
	act.sa_handler = handler;
	
  /* Inicializar el puntero de la funcion en act*/
	sigemptyset(&act.sa_mask);

  /* Crear el conjunto de señales en act con SIGINT y SIGSTP*/
  	sigaddset(&act.sa_mask,  SIGUSR1);
	sigaddset(&act.sa_mask,  SIGUSR2);

  /* Opciones del handler a SA_RESTART */
  	act.sa_flags = SA_RESTART;

  /* Instalar el controlador para las dos señales */
	if (sigaction(SIGUSR1, &act, NULL) == -1){}
	if (sigaction(SIGUSR2, &act, NULL) == -1){}

	
 pid=fork();

if (pid==0){
	int num;
	printf("ENTRANDO a pid = %d\n", getpid());
	num =execvp(argv[1], argv+1);
	printf("Error %d \n", num);
	
}
//else{}
	wait();

}

