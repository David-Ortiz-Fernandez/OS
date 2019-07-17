#include <sys/types.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) 
{

  int fd;
  void * addr;
  struct stat statbuf;

  if( argc != 2 ) {
    fprintf(stderr,"Uso: mmap archivo\n");
    exit(1);
  }

/* Obtener las estadisticas del inodo del archivo con stat */
	stat(argv[1], &statbuf);
/* Abrir el fichero de s�lo lectura */
	fd = open(argv[1], O_RDONLY);
/* Mapear el fichero en memoria, como p�ginas de lectura compartida */
	addr = mmap(NULL,statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
/* Cerrar el descriptor */
	close(fd);
/* Escribir toda la region de memoria en la salida est�ndar */
	write(1, addr, statbuf.st_size);  
return(0);
}
