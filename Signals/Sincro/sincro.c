#include <stdio.h>
#include <signal.h>

#define INTERCAMBIO "cola"

static volatile int hay_num = 0;
static volatile int necesito_num = 1;

void productor (int signo)
{
	necesito_num = 1;
};

void consumidor (int signo)
{
	hay_num = 1;
};

void * ini_manejador(int signal, void *manejador)
{
	struct sigaction act;
    struct sigaction old_act;

    int i;

    act.sa_handler = manejador;

    sigemptyset(&act.sa_mask);

    act.sa_flags = SA_RESTART;

    sigaction(signal, &act, &old_act);
}

void escribe_float(float num)
{
	FILE *tmp;
    
    tmp = fopen(INTERCAMBIO,"w");
    fprintf(tmp,"%f\n",num);
    fclose(tmp);
}
    
void lee_float(float *num)
{
	FILE *tmp;
    
    tmp = fopen(INTERCAMBIO,"r");
    fscanf(tmp,"%f\n",num);
    fclose(tmp);
}

int main()
{
	int pid;
    FILE *tmp;
    float num;        

	tmp = fopen(INTERCAMBIO,"w");
    fclose(tmp);
    
    /* =====> Bifurcar el proceso */   
    pid=fork();

    switch(pid)
    {
    	case -1:
        	perror("Error en el fork()");
            exit(-1);
            break;
        case 0:
        /* =====> inicializar el manejar de SIGUSR1 para el hijo (consumidor) */
	ini_manejador(SIGUSR1, consumidor);

            while(1)
            {
            	/* Esperar hasta que haya un numero */
            	while(hay_num == 0){};
                
                /* Leer el numero, multiplicarlo por dos y escribirlo */
				lee_float(&num);            
				
                num = 2.0 * num;
                
				escribe_float(num);
                
            	hay_num = 0;

				/* =====> Enviar la señal SIGUSR1 al padre */       
				kill(getppid(),SIGUSR1);
         
            };   	
         	break;
        default:
       /* =====> inicializar el manejar de SIGUSR1 para el padre (productor) */ 
	ini_manejador(SIGUSR1, productor);
            while(1)
            {
            	/* Esperar hasta que necesite un numero */
            	while(necesito_num == 0){};
                
                /* Leer el numero, y escribirlo en el fichero*/            
				lee_float(&num);
                			
            	printf("Resultado = %f\n",num);
            
            	scanf("%f",&num);
                printf("NUMERO = %f\n",num);
                
				escribe_float(num);
                            
            	necesito_num = 0;
                
                /* =====> enviar la señal SIGUSR1 al hijo */
		kill(pid,SIGUSR1);
            }                
         	break;                
    }
}
