#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int bloquea(int fd, int bloqueo)
{

	struct flock c;

  	c.l_type = bloqueo;
    c.l_whence = SEEK_SET;
    c.l_start = 0;
    c.l_len = 0;
    
    if (fcntl(fd,F_SETLKW,&c) == -1 )
    {
    	perror("fcntl");        	
        exit(-1);
	}        
}

int main()
{

    int fd;
    int num,i;
    
    /* Apertura del fichero */
    
    fd = open("./temporal", O_CREAT | O_RDWR | O_SYNC, 0666);
    
    /* Comprobacion de errores */
  
    for (i=0; i<10000 ; i++)
    {    	

	sleep(1);
	//1.- Poner el puntero al principio
	lseek(fd, 0, SEEK_SET);
	//2.- Bloquear el archivo para escritura
	bloquea(fd, F_WRLCK);
	//3.- Leer un entero
	read(fd, &num, sizeof(int));
	//4.- Incrementar el entero leido
	num++;
	//5.- Poner el puntero al principio
	lseek(fd, 0, SEEK_SET);
	//6.- Escribir el puntero
	write(fd, &num, sizeof(int));

        printf("(%d) numero = %i\n",getpid(),num);
	//7.- Desbloquear el fichero   
        bloquea(fd, F_UNLCK);   
    }

    close(fd);
}    
