#include "shared.h"



int main (int argc, char **argv)
{
	int shmid;
    int servidor;
    char *dato;
    
    if (argc == 2)
    {
    	/* Crear la region */
        
    	/* Obtener la direccion */
    
    	*dato = 'O';
    
    	sleep(10);
    
		/* Despegarse de la region */
    
		/* Borrar la region */
	}        
	else
    {
    	/* Crear la region */
        
    	/* Obtener la direccion */
    
    	printf("En la memoria hay %c\n", *dato);
        
		/* Despegarse de la region */    
	}
    
    return 0;
}       	
