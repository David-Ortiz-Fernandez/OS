#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[])
{
	char *var, *value;

	if(argc==1||argc>3){
		fprintf(stderr, "uso:variable de entorno [valor]\n");
		exit(1);
	}
	var=argv[1];
	value = getenv(var);
	if(value)
		printf("Variable: %s  Valor: %s\n",var, value);
	else
		printf("Variable: %s  no tiene valor %s\n",var);

	if(argc==3) {
		char*string;
		value = argv[2];
		string = malloc(strlen(var) + strlen(value) +2);
		if(!string) {
			fprintf(stderr, "fuera de memoria\n");
			exit(1);
		}
		strcpy(string, var);
		strcat(string, "=");
		strcat(string, value);
		printf("Llamando a putenv con: %s\n", string);
		if(putenv(string) !=0){
			fprintf(stderr, "fallo en putenv\n");
			free(string);
			exit(1);
		}
	}

		value = getenv(var);
		if(value)
			printf("El nuevo valor de %s es %s\n", var, value);
		else
			printf("El nuevo valor de %s es null\n", var);
		exit(0);

}
