#include "mensaje.h"
#include <sys/types.h>
#include <time.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	int msg_cs, msg_sc;
    key_t llave1,llave2;
    msg_t msg;
    int fin = 0;
    pid_t pid;
    int k;
    char op;
    struct tm *ptr;
    char fecha[80];
    time_t t;
    
	if(argc<3) exit(-1);
	int tiempo = atoi(argv[1]);
	int repeticiones = atoi(argv[2]);
	setbuf(stdout,NULL);        
    printf("Servidor Activo\n");      
/* Obtner los identificadores de las colas */

	 llave1=ftok(KEYFILE,TOK_CS);
	   msg_cs=msgget(llave1,IPC_CREAT | 0600);

	   llave2=ftok(KEYFILE,TOK_SC);
	   msg_sc=msgget(llave2,IPC_CREAT | 0600);
    
    printf("Servidor Activo\n");
    
/* El tipo del mensaje ha de ser igual al pid del proceso para distinguirlos */

	msg.tipo=getpid();//Al campo tipo del mensaje asignarle el pid
    int i =0;
   	while (i<repeticiones)
    {
      	/* Obtencion del tiempo actual (t) */
printf("HOLA\n");

	time(&t);
	   
	/* Obtencion de la estructura  (ptr) */
	ptr = localtime(&t);
	    
	/* Obtener la fecha en una cadena de la forma
	   "Hoy es Martes 17:40" */
	       
	    strftime(fecha,80,"hoy es %A, %H:%M" ,ptr);
	    
	    
	  strcpy(msg.cad,fecha);                
		msg.op=TIPO1;
		printf("[SERVIDOR] Creando cadena con fecha y enviando al cliente\n");
		// enviar el mensaje 
		msgsnd(msg_sc,&msg,LONG,0);
		sleep(3);

		//recive un mensage de confirmacion
		msgrcv(msg_cs,&msg,LONG,0,0);

		//repeticiones=repeticiones+1;  
		i=i+1;   
}//fin del while
        msg.op=TIPO2;
	msgsnd(msg_sc,&msg,LONG,0); 
/*ELIMINACION RECURSOS IPC*/
msgctl(msg_cs,IPC_RMID,0);
msgctl(msg_sc,IPC_RMID,0);
                        
    return 1;
}



