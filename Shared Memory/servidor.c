#include "shared.h"
#include <time.h>
#include <stdio.h>


int main (int argc, char **argv)
{
sleep(5);
int servidor;
char *dato;
char fecha[128];
time_t t;
struct tm *tm;

/* Crear la region */
id=crear_region(sizeof(char), 0666);
/* Obtener la direccion */
dato=(char*)coge_region(id);
	time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        strftime(fecha,128,"%d/%m/%y %H:%M:%S",tlocal);
	strcpy(dato, fecha);
printf("Esperando que el caracter cambie\n");

//-----------------------------
while((*dato)!='*'){}
//-----------------------------
printf("EL CARACTER HA CAMBIADO A: *\n");
printf("PROCEDO A CERRARME\n");
/* Despegarse de la region */
suelta_region(dato);
/* Borrar la region */
borra_region(id);      
    
    return 0;
}       	
