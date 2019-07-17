#include "shared.h"
int main (int argc, char **argv)
{
sleep(5);

char *dato;
char fecha[128];
int fin=0;
/* Crear la region */
id=crear_region(sizeof(char), 0666);
/* Obtener la direccion */
dato=(char*)coge_region(id);
strcpy(fecha, dato);
//-----------------------------
while(fin!=1)
	{
	if ((fecha)!='*')
		{
		printf("%s\n",fecha);
		strcpy(dato,"*");
		printf("Cambio el caracter a *\n");
		fin=1;
		}
	}
//-----------------------------
printf("PROCEDO A CERRARME\n");
/* Despegarse de la region */
suelta_region(dato);
   
    
    return 0;
}       	
