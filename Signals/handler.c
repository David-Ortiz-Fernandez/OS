#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static int contador_STP=0;
static int contador_INT=0;

void handler(int signo) 
{
	char m1[80]="Recibida señal SIGTSTP\n";
	char m2[80]="Recibida señal SIGINT\n";   

	if (signo == SIGTSTP)
    {
  		write(1, m1, 80 );
		contador_STP++;        
 	}        
        
	if (signo == SIGINT)        
    {
		write(1, m2, 80 );
		contador_INT++;           
    }
}



int main() {

  struct sigaction act;
        
  /* Instalasmos el manejador para SIGINT y SIGSTP */
	act.sa_handler = handler;
	
  /* Inicializar el puntero de la funcion en act*/
	sigemptyset(&act.sa_mask);

  /* Crear el conjunto de señales en act con SIGINT y SIGSTP*/
  	sigaddset(&act.sa_mask,  SIGTSTP);
	sigaddset(&act.sa_mask,  SIGINT);

  /* Opciones del handler a SA_RESTART */
  	act.sa_flags = SA_RESTART;

  /* Instalar el controlador para las dos señales */
	if (sigaction(SIGTSTP, &act, NULL) == -1){}
	if (sigaction(SIGINT, &act, NULL) == -1){}

	
fork();
if (getpid()!=0){
	printf("Soy el hijo\n");
	
}
else{
	wait();
	printf("Soy el padre\n");
	
  while (contador_STP+contador_INT<10){};	
  printf("Se recibierno %d señales: %d SIGINT y %d SIGTSTP\n",    
  contador_STP+contador_INT,  contador_INT,contador_STP);
}

}
