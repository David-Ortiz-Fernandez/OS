#ifndef _SHARED_H
#define _SHARED_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

#define ID 's'
#define PATH "./keyfile"

extern int errno;

/* Función para la creación de un semáforo */

int crear_mutex(mode_t modo) ;
void borrar_mutex( int semid);
void wait_mutex(int semid);
void signal_mutex(int semid);

int crear_region(size_t size, mode_t modo);
void *coge_region(int shmid);
void suelta_region(void * addr);
void borra_region(int shmid);

#endif _SHARED_H