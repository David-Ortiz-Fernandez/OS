#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>

#define MAX_LOOP 10
#define ARCHIVO  "./temporal"

extern int errno;
//-------------------------------------------------------------------------------------
void ini_fichero(void)
{
	int fd;
    int numero;
    
    fd=open(ARCHIVO,O_CREAT|O_RDWR,0666);
    
    numero = 1;
    
    write(fd,&numero,sizeof(numero));    
        
    close(fd);

}
//------------------------------------------------------------------------------------
void contador(void)
{
	int fd;
    int numero;
    
/* Apertura del fichero */ fd=open(ARCHIVO,O_CREAT|O_RDWR,0666);
	
    
    printf("Proceso %i modificando contador... ",getpid());
    
/* leer el numero */ read(fd,&numero,sizeof(numero));
    
    printf("actual = %i ",numero);
    
    numero = numero + 1;
     
/* Ponernos al principio del archivo */lseek(fd,0,SEEK_SET);
    
/* Escribir el numero */write(fd,&numero,sizeof(numero));  
    
    printf("nuevo = %i\n",numero);
    
    close(fd);
	sleep(1);
}
//------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int semid;
    int sincro;
    int i;
    struct sembuf operacion;
    key_t llave;
    
    setbuf(stdout,NULL);
        
    if ( (argc == 2) && (strcmp(argv[1],"sync")==0) )
    {
    	printf("Ejecutando en modo sincrono\n");
        sincro = 1;
	}
    else
    {
    	sincro = 0;
	}
    
/* Crear la llave */llave=ftok ("./sem1.c",'a');
    
/* Crear el grupo de semaforos con un semaforo, de forma
que se pueda comprobar su existencia */ semid=semget(llave,1,IPC_CREAT|IPC_EXCL|0666);// el 1 indica el numero de semaforos del conjunto

    
    if ( ( semid == -1 ) && ( errno == EEXIST ) )
    {
    	printf("El semaforo ya existe\n");
    	/* Obtener el semid del semaforo */semid=semget(llave,1,0666);
    }
    else
    {
    	printf("Semaforo creado, inicializando\n");
        ini_fichero();
		/* inicializar el valor de semaforo a 1 */semctl(semid,1,SETVAL,1);
	} 
        	/* Desbloquear el semaforo */
		/*operacion.sem_num = 0;
 		operacion.sem_op  = 1;
 		operacion.sem_flg = 0;
		semop(semid,&operacion,1);//el uno indica realizar una vez la operacion   */    
	
    for (i=0;i< MAX_LOOP; i++)
    {
    	if (sincro)
    	{    
			
            printf("Solicitando semaforo...");    
			/* Bloquear el semaforo */
			operacion.sem_num = 0;
 			operacion.sem_op  = -1;/* Completar */
 			operacion.sem_flg = 0;
			semop(semid,&operacion,1);//el uno indica realizar una vez la operacion
			

            printf("abierto\n");
		}        
        
    	contador();
    
    	if (sincro==0)
    	{    
        	/* Desbloquear el semaforo */
		operacion.sem_num = 0;
 		operacion.sem_op  = 1/* Completar */;
 		operacion.sem_flg = 0;
		semop(semid,&operacion,1);//el uno indica realizar una vez la operacion
	sleep(5);
	            

		}        
        

    }
    
/* Borrar el semaforo */semctl(semid,1,IPC_RMID);
    
    return 0;
}    
    
    
    
    
    
    
                    
                    
