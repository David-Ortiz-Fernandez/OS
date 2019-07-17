#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]){

	int pid;
    	int tuberia;
	char *path = "fifo";
    	char buffer[256];
	int fin;
	int fd;
	
    
    fin = 0;

/* Crear la tuberia */ 
	mkfifo(path, 0666);           
    
/* Crear el proceso hijo */
	pid=fork();
    
    switch ( pid ){
    
    case -1:
    	perror("fork");
        exit(-1);
        break;
    case 0:
    	printf("Lector de la tuberia\n");
        /* Abrir la tuberia como solo lectura */
	fd = open(path, O_RDONLY, S_IREAD);
        while (!fin)
        {
        	/* Leer de la tuberia */
		read(fd, buffer, 256);
            
            if (buffer[0] == 'q')
            	fin = 1;
            else
            	printf("Mensaje recibido(%i): %s\n",getpid(),buffer);
         }
         /* Cerrar la tuberia */  
	close(fd);       
         exit(0);
         break;
	default:
    	printf("Escritor de la tuberia\n");
        /* Abrir la tuberia como solo de escritura */
	fd = open(path, O_WRONLY, S_IWRITE);
        while(!fin)
        {
            scanf("%s",buffer);
			/* Escribir el mensaje en la tuberia */	
			write(fd, buffer, 256);
            if (buffer[0] == 'q')
            	fin = 1;           
         }
         /* Cerrar la tuberia */
	close(fd);
         exit(0);
         break;
	}
    
}
