#include "mensaje.h"
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

/* Datos */

int i = 0;

int main(int argc, char **argv)
{
	int msg_cs, msg_sc;
    key_t llave1,llave2;
    msg_t msg;
    int fin = 0;
    pid_t pid;
    int k;
      struct tm *ptr;
	char fecha[80];
	time_t t;
/* Obtener las llaves para las colas msg_cs y msg_sc */

	   llave1=ftok(KEYFILE,TOK_CS);
	   msg_cs=msgget(llave1,IPC_CREAT | 0600);

	   llave2=ftok(KEYFILE,TOK_SC);
	   msg_sc=msgget(llave2,IPC_CREAT | 0600);
    
    printf("Cliente Activo\n");

/* Ejecutar el comando ipcs -q, para comprobar el estado de las colas */
    
 	msg.tipo=getpid();
	printf("[CLIENTE]SOY CLIENTE ME HE CREADO\n");	
	msgsnd(msg_cs,&msg,LONG,0);
	fin=0;
	while(fin==0)
		{
//		printf("SOY CLIENTE ESPERO CONFIRMACION\n");
		msgrcv(msg_sc,&msg,LONG,0,0);
		if(msg.op==TIPO1)
			{
			printf("[CLIENTE]Recibido mensaje de TIPO1\n");
			
			}
		else if(msg.op==TIPO2){fin=1;}
		msgsnd(msg_cs,&msg,LONG,0);
		//sleep(3);
		}

printf("[CLIENTE] Recibido mensaje de finalizacion\n");
printf("[CLIENTE] Finalizando...\n");
                        
    return 1;
}        
