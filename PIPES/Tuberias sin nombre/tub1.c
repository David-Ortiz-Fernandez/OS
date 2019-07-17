#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(){

	int pid;
    int tuberias[2];
    char buffer[256];
	int fin;
    
    fin = 0;

/*  Apertura de la tuberia */pipe(tuberias);

/* Creacion del hijo */pid = fork();
    
    switch ( pid ){
    
    case -1:
    	perror("fork");
        exit(-1);
        break;
    case 0:
    	printf("Lector de la tuberia\n");
/* Cerrar el extremo de la tuberia que no vamos a usar */close(tuberias[1]);
        while (!fin)
        {
			/* Leer de la tuberia, almacenando el resultado en 
               la variable buffer */read(tuberias[0], buffer, 256);
                           
            if (buffer[0] == 'q')//salimos del programa con q
            	fin = 1;
            else
            	printf("Mensaje recibido(%i): %s\n",getpid(),buffer);
         }
/* Cerrar el extremo de la tuberia que hemos usado */close(tuberias[0]);
         exit(0);
         break;
	default: //es decir el padre
    	printf("Escritor de la tuberia\n");
/* Cerrar el extremo de la tuberia que no vamos a usar */close(tuberias[0]);
        while(!fin)
        {
            scanf("%s",buffer);
			/* Escribir en la tuberia */write(tuberias[1], buffer, 256);
            if (buffer[0] == 'q')
            	fin = 1;           
         }
/* Cerrar el extremo de la tuberia que hemos usado */close(tuberias[1]);
         exit(0);
         break;
	}
    
}    
    
    
    
    
    
    
    
    
    
    
    
