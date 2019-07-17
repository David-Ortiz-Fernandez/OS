#include "shared.h"

/* Función para la creación de un semáforo */

int crear_mutex(mode_t modo) 
{
  key_t  llave;
  int semid;

  llave = ftok(PATH, ID);
  
  if (llave == (key_t) -1) 
  {
    perror("crear_region: ftok");
    exit(1);
  }
  
  semid = semget(IPC_PRIVATE, 1, modo | IPC_CREAT);//Con IPC_CREAT->Crealo pero si ya existe devuelveme el identificador

  if (semid == -1) 
  {
    perror("crear_mutex: semget");
    exit(1);
  }
    
  return semid;
}

void borrar_mutex( int semid)
{

  if ( semctl(semid,0,IPC_RMID,0) == -1) 
  {
    perror("borrar_mutex: semctl");
    exit(1);
  }
  return;
}

void wait_mutex(int semid)
{
  	static struct sembuf sops;
 
 	sops.sem_num = 0;
 	sops.sem_op  = -1;
 	sops.sem_flg = 0;
    
  	if (semop(semid, &sops, 1) == -1) 
  	{
    	perror("wait_mutex: semop");
    	exit(1);
  	}
}

void signal_mutex(int semid)
{
  	static struct sembuf sops;
 
 	sops.sem_num = 0;
 	sops.sem_op  = 1;
 	sops.sem_flg = 0;
    
  	if (semop(semid, &sops, 1) == -1) 
  	{
    	perror("signal_mutex: semop");
    	exit(1);
  	}
}
