#include "shared.h"
#include <unistd.h>
#include <stdlib.h>



/* Funcion para la creación de una región de memoria compartida */
/* La creación se realiza en función de los defines, PATH y ID  */

int crear_region(size_t size, mode_t modo) {

  key_t  llave;
  int    shmid;

/* Crear la llave con ftok */
llave=ftok(PATH,ID);
  
  if (llave == (key_t) -1) 
  {
    perror("crear_region: ftok");
    exit(1);
  }

/* Obtener la region de memoria compartida, con IP_CREAT y modo */

	shmid=shmget(llave,size,modo | IPC_CREAT);
  
  if (shmid == -1) 
  {
    perror("crear_region: shmget");
    exit(1);
  }
  
  return shmid;
}
//--------------------------------------------------------------------------------------
/* Funcion para "pegarse" a una region de memoria compartida */

void *coge_region(int shmid)
{
  void  *addr;  

/* Obtener la dirección de memoria compartida */

	addr=shmat(shmid,0,0);//primer  se lo deja al kernell, segundo cero lectura escritura


  if (addr == (void *) -1) 
  {
    perror(" coge_region: shmat");
    exit(1);
  }
  return addr;
}
//----------------------------------------------------------------
/* Funcion para "despegarse" de una region de memoria compartida */

void suelta_region(void * addr)
{
  if (shmdt(addr)) 
  {
    perror("suelta_region: shmdt");
    exit(1);
  }
}
//----------------------------------------------------------------
/* Funcion para eliminar una region de memoria compartida */

void borra_region(int shmid)
{
  if (shmctl(shmid,IPC_RMID,0)) {
    
    if (errno == EPERM)  /* No puedo borrarla */
    	return;
    else 
    	if (errno == EINVAL) /* Ya está borrada */
      		return;
   		else 
        {
      		perror("borrar_region: shmctl");
      		exit(1);
    	}
  }
}
