#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//-----------------------------------------------------------------------------------------------
int pid;
struct lista{
	int pid_proceso;
	char nombre;
};

struct lista lista_proc [10];
//-----------------------------------------------------------------------------------------------
/*FUNCION DE PARSEO-----------------------------------------------------------------------------*/ 
/* Funcion que realiza el parse de la linea de
   comandos, devuelve un array de cadenas con 
   los argumentos
*/       
char ** parse(char *line)
{
   static char delim[] = " \t\n"; 
   int count = 0;
   char * p;
   static char **scr ;

   if (*line == '\n')
      return NULL;
   for (p = (char *) strtok(line,delim); p ; p = (char *)strtok(NULL,delim))
   {
      scr = (char **) realloc(scr,(count + 2) * sizeof(char *));
      scr[count++] = p;
   }
   scr[count] = NULL;

   return scr;
}
//------------------------------------------------------------------------------------------------
/*MANEJADOR DEL MANAGER--------------------------------------------------------------------------*/
void handler(int signo) 
{  
if (signo == SIGUSR1)
	{    
	kill(pid,15);  
 	}        
        
if (signo == SIGUSR2)        
   	{
	}
if (signo == SIGCHLD)        
    	{	
	int pid2 = wait( NULL );
	printf("%d",pid2);           
    	}
}

//------------------------------------------------------------------------------------------------
/*Funcion MAIN----------------------------------------------------------------------------------*/
int main() {
char **argumentos;
char comando[80];
pid_t  pid, pid_ret;

struct sigaction act;
        
/* Instalasmos el manejador para SIGINT y SIGSTP */
act.sa_handler = handler;
	
/* Inicializar el puntero de la funcion en act*/
sigemptyset(&act.sa_mask);

/* Crear el conjunto de señales en act con SIGUSR1 SIGUSR2 SIGCHLD */
 sigaddset(&act.sa_mask,  SIGUSR1);
sigaddset(&act.sa_mask,  SIGUSR2);
sigaddset(&act.sa_mask,  SIGCHLD);

/* Opciones del handler a SA_RESTART */
act.sa_flags = SA_RESTART;

/* Instalar el controlador para las tres señales */
if (sigaction(SIGUSR1, &act, NULL) == -1){}
if (sigaction(SIGUSR2, &act, NULL) == -1){}
if (sigaction(SIGCHLD, &act, NULL) == -1){}
  
  
setbuf(stdout,NULL);
printf("\n");
/*inicializacion de lista*/
int i;
int count=0;
for(i=0;i<10;i++) lista_proc[count].pid_proceso=0;

/* Bucle Principal */
  
  while (1){
/* Espera de Comando */
  
  	do{
  	printf("> ");
  	fgets(comando,80,stdin);
  	argumentos = parse(comando);
    } while(argumentos == NULL);
    
//----------------------------------------------------------------------------------------
printf("HAS ELEGIDO LA OPCIÓN: %s\n",comando);
/*OPCION HELP----------------------------------------------------------------------------*/
if(strcmp(argumentos[0],"help")==0) 
{
printf("=========================\n");
printf("Comandos soportados\n");
printf("=========================\n");
printf("help: muestra un mensaje de ayuda con los comandos soportados\n");
printf("list: muestra lista de procesos lanzados y aun no finalizados\n");
printf("run aplicación: invoca al launcher para que ejecute la aplicación 'aplicación'\n");
printf("kill pid: envia al launcher el pid del proceso con PID 'pid' para que este lo finalice\n");
printf("prio pid: envia al launcher el pid de la aplicacion lanzada que debe de cambiar la prioridad\n");
printf("exit: sale del programa manager\n");
printf("=========================\n");

}
/*OPCION LIST-----------------------------------------------------------------------------*/
else if(strcmp(argumentos[0],"list")==0)
	{
	printf("=========================\n");
	printf("Lista de procesos activos\n");
	printf("=========================\n");
	printf("                  Comando\n");
	for(i=0;i<10;i++)
		{
		if (lista_proc[i].pid_proceso!=0) printf("[%s              %d\n","PROCESO",  lista_proc[i].pid_proceso);	
		}
	}
/*OPCION RUN-------------------------------------------------------------------------------*/
	else if(strcmp(argumentos[0],"run")==0)
		{
		if(argumentos[1]==NULL) printf ("Falta especificar el argumento 'aplicacion'\n");
		else{
			pid=fork();
			if (pid==0){/*el hijo*/
	   			argumentos[0]="./launcher";
	   			printf("%d\n", getpid());
	   			int num;
	   			num=execvp(argumentos[0], argumentos);
	   			printf("Error en el execvp del manager\n");
				}
			else
				{/*el padre*/
		    		lista_proc[count].pid_proceso = pid;
		    		if (count<10)count++;
	   	   		else printf("lista de procesos llena\n");
		    		}
		    }
		}
/*OPCION KILL------------------------------------------------------------------------------*/
		else if(strcmp(argumentos[0],"kill")==0)
			{
			if(argumentos[1]==NULL) printf ("Falta especificar el argumento 'pid del proceso'\n");
			else{
				printf("El PID del proceso a matar es: %d\n",atoi(argumentos[1]));
				kill(atoi(argumentos[1]), SIGUSR1); 
				for(i=0; i<10; i++)
					{
					if(atoi(argumentos[1])==lista_proc[i].pid_proceso){
						lista_proc[i].pid_proceso=0;
						count--;}	
					}
				}
    			}
/*OPCION PRIO-------------------------------------------------------------------------------*/
			else if(strcmp(argumentos[0],"prio")==0)
				{
				if(argumentos[1]==NULL) printf ("Falta especificar el argumento 'aplicacion'\n");
				else{
					printf("El PID del proceso al que le vamos a cambiar la prioridad es: %d\n",atoi(argumentos[1]));
					kill(atoi(argumentos[1]), SIGUSR2);
				    } 
				}
/*OPCION EXIT-------------------------------------------------------------------------------*/
				else if(strcmp(argumentos[0],"exit")==0)
					{
					for(i=0;i<10;i++)
					{
						if(lista_proc[i].pid_proceso!=0)
							kill(lista_proc[i].pid_proceso, SIGUSR1);						
					}						
					return 0;
					//abort();
					}
/*NO ELEGIR OPCION--------------------------------------------------------------------------*/
					else 	{
						printf("No has elegido ninguna de las opciones soportadas\n");
						}

  }
} 
