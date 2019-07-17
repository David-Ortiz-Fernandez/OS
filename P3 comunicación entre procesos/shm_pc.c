#include "shared.h"

int crear_region(size_t size, mode_t modo);
void *coge_region(int shmid);
void suelta_region(void * addr);
void borra_region(int shmid);

#define BUFFER 5

static int sem_entrada, sem_datos, sem_buffer, sem_huecos;

void productor(int *buffer)
{
	int numero;
	int puntero;

/* Produce numero */        
    numero = getpid();
    sleep(1);
    	printf("Productor %i, bloqueado a la espera de huecos...",getpid());       
/* Hay huecos ?*/ //Wait sobre huecos
	wait_mutex(sem_huecos);
	
	printf("Productor %i, desbloqueado a la espera de huecos...",getpid());	

/* Proteger el buffer */ //Wait sobre buffer

	wait_mutex(sem_buffer);
    
    	printf("Productor %i, escribiendo...",getpid());
    
/* Actualizar el puntero de acceso sem_entrada*/ //Wait sobre entrada

	wait_mutex(sem_entrada);
    
/* Obtener el valor de la posicion en la que escribimos */
    
    	puntero = semctl(sem_entrada,0,GETVAL,0); //Con semctl() usar GETVAL
    
    	buffer[puntero] = numero;

    	printf(" termino\n");
        
        printf("Dato puesto en la posición [%i]=%i\n",puntero,numero);
        
/* Liberar el buffer */ //Signal sobre buffer

	signal_mutex(sem_buffer);
    
/* Actulizar los datos */ //Signal sobre datos
	signal_mutex(sem_datos);

}            

void consumidor(int *buffer)
{
	int numero;
	int puntero;
        
    numero = getpid();
    
/* Puedo leer ? */ //Wait sobre datos
	wait_mutex(sem_datos);
    
/* Proteger el buffer */ //Wait sobre buffer

	wait_mutex(sem_buffer);
    		
    		printf("Consumidor %i, leyendo...",getpid());    
            

/* Obtener el valor de la posicion en la que escribimos */            
    		puntero = semctl(sem_entrada,0,GETVAL,0);//Usar semctl() con GETVAL
    
    		numero = buffer[puntero];
                	            
    		printf(" termino\n");
        
			printf("Dato recogido de la posición [%i]=%i\n",puntero,numero);    
        
/* Actualizar el puntero de acceso sem_entrada*/  //Wait sobre sem_entrada     

	signal_mutex(sem_entrada);

/* Liberar el buffer */ //Signal sobre buffer

	signal_mutex(sem_buffer);
        
/* Actulizar los huecos */ //Signal sobre huecos

	signal_mutex(sem_huecos);       

	sleep(2);
}            
    
int main (int argc, char **argv)
{

    
	int prod,cons;
    int i;
    int *buffer;
    int shmid;
    
    prod = atoi(argv[1]);
    cons = atoi(argv[2]);
/* Creacion de los semaforos */    
	sem_buffer  = crear_mutex(0600) ;
	sem_entrada = crear_mutex(0600) ;
    sem_datos   = crear_mutex(0600) ;
    sem_huecos   = crear_mutex(0600) ;

/* Crear la region de memoria compartida */    
    shmid = crear_region(BUFFER*sizeof(int),0600);

/* Obtener la direccion */
    
    buffer = (int *)coge_region(shmid);
    
/* Inicilizar los semaforos */
    
    semctl(sem_entrada,0,SETVAL,BUFFER);//Apunta al ultimo dato producido
    semctl(sem_huecos,0,SETVAL,BUFFER);
    semctl(sem_datos,0,SETVAL,0);
    semctl(sem_buffer,0,SETVAL,1);//Para que pueda entrar siempre uno

/* Lanza los productores */
                    
	for (i=0;i <prod;i++)
    {
    	
    	if (fork() == 0)
        {
			productor(buffer);
			exit(0);                            
        }                
	}

/* Lanza los consumidores */
    
	for (i=0;i <cons;i++)
    {
    	
    	if (fork() == 0)
        {
			consumidor(buffer);
			exit(0);                            
        }                
	}

/* Hacer un wait de todos los hijos */

	for(i=0; i < prod+cons;i++)
		wait(NULL);

/* Borramos los recursos IPC */

        
	borrar_mutex(sem_buffer );
	borrar_mutex(sem_entrada);
    borrar_mutex(sem_datos  );
	borrar_mutex(sem_huecos  );    
     
    borra_region(shmid);    
               
    return 0;
}       	
