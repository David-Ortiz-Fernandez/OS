#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(){

	int pid;
    int tuberia_ph[2];
    int tuberia_hp[2];    
    char buffer[256];
	int fin;
    
    fin = 0;
        
/* Apertura de la tuberia de comunicacion padre --> hijo  (tuberia_ph) */
	pipe(tuberia_ph[2]);
    
/* Apertura de la tuberia de comunicacion hijo  --> padre (tuberia_hp) */    
	pipe(tuberia_hp[2]);
/* Crear el proceso hijo */
	pid = fork();
    
    switch ( pid ){
    
    case -1:
    	perror("fork");
        exit(-1);
        break;
    case 0:
    	printf("Lector de la tuberia\n");
/* Cerrar los extremos de las tuberia que no vamos a usar */
	close(tuberia_ph[1]);
	close(tuberia_hp[0]);
/* Escribir un mensaje en la tuberia hijo --> padre para parar al padre */
		 write( tuberia_hp[1], buffer, strlen( buffer ) );
        while (!fin)
        {
        	/* Leer mensaje de la tuberia padre --> hijo */

		read( tuberia_ph[0], buffer, 256);
            
            if (buffer[0] == 'q')
            	fin = 1;
            else
            	printf("Mensaje recibido(%i): %s\n",getpid(),buffer);
                
			/* Suspender el proceso 2 segundos */
            
/* Escribir un mensaje en la tuberia hijo --> padre para parar al padre */
		 write( tuberia_hp[1], buffer, strlen( buffer ) );

        }
/* Cerrar los extremos de las tuberias que hemos usado */
	close(tuberia_ph[0]);
	close(tuberia_hp[1]);

        exit(0);
        break;
	default:
    	printf("Escritor de la tuberia\n");
/* Cerrar los extremos de las tuberia que no vamos a usar */
	close(tuberia_ph[0]);
	close(tuberia_hp[1]);

        while(!fin)
        {
        	/* Leer un mensaje de la tuberia hijo --> padre para sincronizarnos */
	    read( tuberia_hp[0], buffer, 256);
            printf("? ");
            scanf("%s",buffer);
            
            /* Escribir el nuevo mensaje en la tuberia padre --> hijo */
	     write( tuberia_ph[1], buffer, strlen( buffer ) );
            if (buffer[0] == 'q')
            	fin = 1;           
         }
/* Cerrar los extremos de las tuberias que hemos usado */
	close(tuberia_ph[1]);
	close(tuberia_hp[0]);
        exit(0);
        break;
	}
    
}    
    
    
    
    
    
    
    
    
    
    
    
