#include "shared.h"



int main (int argc, char **argv)
{
	int shmid;
    int servidor;
    char *dato;
    
    if (argc == 2)
    {
    	/* Crear la region */

	shmid=crear_region(sizeof(char), 0666);
        
    	/* Obtener la direccion */
    
	dato=(char*)coge_region(shmid);
	
    	*dato = 'O';
    
    	sleep(30);
    
		/* Despegarse de la region */

		suelta_region(dato);
    
		/* Borrar la region */
		borra_region(shmid);

	}        
	else
    {
    	/* Crear la region */

	shmid=crear_region(sizeof(char), 0666);
        
    	/* Obtener la direccion */

	dato=(char*)coge_region(shmid);
    
    	printf("En la memoria hay %c\n", *dato);
        
		/* Despegarse de la region */ 

		   suelta_region(dato);
	}
    
    return 0;
}       	
