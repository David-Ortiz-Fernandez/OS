#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int 
main(int argc, char *argv[]) {
/************************************VARIABLES****************************************************/
struct stat buf, buf2, buf3, buf4;
DIR   *dirp;
DIR   *dirp2;
struct dirent *dent;
struct dirent *dent2;
int fd,fd2,n,n2;
char c,c2;
char preset[10] ="";
char permisos1[10], permisos2[10];
int existe=0;
char bufff[128], bufff2[128], bufff3[128], bufff4[128];
/*************************************************************************************************/
/*obtencion de la descripcion del directorio, si no lo es se sale */
  if (stat(argv[1], &buf) == -1) {
    perror("stat");
    exit(1);
  }
/*Comprobamos que es un directorio*/
  if (!S_ISDIR(buf.st_mode)) {
    fprintf(stderr, "%s no es un directorio.\n", argv[1]);
    exit(1);
  }
/*Comprobamos que es un directorio el destino, sino lo es lo crea, no hace falta comprobarlo puesto que al crearlo sabemos segurolo es*/
  if (stat(argv[2], &buf2) == -1) {
//si no existe crear directorio, si lo creas tu no necesitas comprobarlo
mkdir(argv[2], buf2.st_mode);
}

/* Apertura del directorio origen */
  if ((dirp = opendir(argv[1])) == NULL) {
    perror("opendir");
    exit(1);
  }
/* Apertura del directorio destino */
   if ((dirp2 = opendir(argv[2])) == NULL) {
    perror("opendir");
    exit(1);
  } 
//-------------------------------------------------------------------------------------------------
/* Leemos el directorio origen */
  while (dent = readdir(dirp)) 
	{existe=0;

/*para cada entrada del origen tenemos que comprobar cada uno de los archivos del destino*/ 
  	while ((dent2=readdir(dirp2))&&(existe==0))
		{
	 	if (strcmp(dent->d_name,dent2->d_name)==0)
			/*el fichero existe en el destino*/
			{existe=1;

			snprintf(bufff3,sizeof(bufff3),"%s%s",argv[1],"/");	
			snprintf(bufff,sizeof(bufff),"%s%s",bufff3,dent->d_name);
			snprintf(bufff4,sizeof(bufff4),"%s%s",argv[2],"/");
			snprintf(bufff2,sizeof(bufff2),"%s%s",bufff4,dent2->d_name);
			
			stat(bufff,&buf);/*obtener descripcion del fichero origen*/
			//(S_ISREG(buf.st_mode));
			stat(bufff2,&buf2);/*obtener la descripcion del fichero destino2*/


			if (((buf.st_mtime)>(buf2.st_mtime))&&(S_ISREG(buf.st_mode)) &&(S_ISREG(buf2.st_mode))) /*el fichero existe en el destino 															pero a sido modificado e el 															origen y son ficheros*/
				{
				fd= open(bufff, O_RDONLY);
								
				fd2 = open(bufff2, O_RDWR|O_TRUNC);
				/*COPIA BYTE A BYTE*/
 				 while ( (n=read(fd,&c2,sizeof(char))) > 0 )
  					{
  	  				write(fd2,&c2,sizeof(char));

  					} 
				printf("Archivo sincronizado en destino: %s\n", bufff2);
				close(fd); 
				close(fd2);
				
				}
			/*permisosy propietario*/
			strcpy(permisos2,preset);
			strcat(permisos2,((buf2.st_mode & S_IRUSR) ? "r" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IWUSR) ? "w" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IXUSR) ? "x" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IRGRP) ? "r" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IWGRP) ? "w" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IXGRP) ? "x" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IRGRP) ? "r" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IWGRP) ? "w" : "-"));
			strcat(permisos2,((buf2.st_mode & S_IXUSR) ? "x" : "-"));
		
			stat(bufff,&buf);

			strcpy(permisos1,preset);
			strcat(permisos1,((buf.st_mode & S_IRUSR) ? "r" : "-"));
			strcat(permisos1,((buf.st_mode & S_IWUSR) ? "w" : "-"));
			strcat(permisos1,((buf.st_mode & S_IXUSR) ? "x" : "-"));
			strcat(permisos1,((buf.st_mode & S_IRGRP) ? "r" : "-"));
			strcat(permisos1,((buf.st_mode & S_IWGRP) ? "w" : "-"));
			strcat(permisos1,((buf.st_mode & S_IXGRP) ? "x" : "-"));
			strcat(permisos1,((buf.st_mode & S_IRGRP) ? "r" : "-"));
			strcat(permisos1,((buf.st_mode & S_IWGRP) ? "w" : "-"));
			strcat(permisos1,((buf.st_mode & S_IXUSR) ? "x" : "-"));

				if((strcmp(permisos1,permisos2))!=0) 
					{chmod(bufff2, buf.st_mode);
					printf("PERMISOS ACTUALIZADOS \n");
					}
				chown(bufff2,buf.st_uid,buf.st_gid);				
			}
		}/*fin segundo while*/

	 if (existe==0) /*el fichero no existe en el destino*/
		{
			snprintf(bufff3,sizeof(bufff3),"%s%s",argv[1],"/");	
			snprintf(bufff,sizeof(bufff),"%s%s",bufff3,dent->d_name);
			snprintf(bufff4,sizeof(bufff4),"%s%s",argv[2],"/");
			snprintf(bufff2,sizeof(bufff2),"%s%s",bufff4,dent->d_name);
			stat(bufff,&buf3);/*obtener descripcion del fichero origen*/
			stat(bufff2,&buf4);/*obtener la descripcion del fichero destino2*/

if (S_ISREG(buf3.st_mode)){
			fd= open(bufff, O_RDONLY);					
			fd2 = open(bufff2, O_RDWR|O_CREAT);


 			while ( (n2=read(fd,&c,sizeof(char))) > 0 )
  				{
  	  			write(fd2,&c,sizeof(char));
  				} 
		printf("---Archivo creado en destino: %s\n",bufff2);
		close(fd2); 
		close(fd);
}
		/*copiamos propietario y permisos*/
		chown(bufff2,buf3.st_uid,buf3.st_gid);
		chmod(bufff2, buf3.st_mode);
		}

	closedir(dirp2);
	dirp2 = opendir(argv[2]);
	}/*fin 1ºwhile de la primera pasada*/


  
/*------------BORRADO EN DESTINO SI NO SE ENCUENTRA EN ORIGEN LO BORRAMOS EN EL DESTINO-----------*/
closedir(dirp2);
closedir(dirp);
dirp = opendir(argv[1]);
dirp2 = opendir(argv[2]);
  while (dent2 = readdir(dirp2)) 
	{
	existe=0;
	while ((dent=readdir(dirp))&&(existe==0))
		{
		if (strcmp(dent->d_name,dent2->d_name)==0)
			{
			existe=1;/*si existe*/
			}
		}
	 if (existe==0) /*si no existe*/
		{
		snprintf(bufff4,sizeof(bufff4),"%s%s",argv[2],"/");
		snprintf(bufff2,sizeof(bufff2),"%s%s",bufff4,dent2->d_name);
		unlink(bufff2);	
		printf("---Archivo borrando en destino, no se encuentra en origen: %s\n",bufff2);
		}
	closedir(dirp);
	dirp = opendir(argv[1]);
	}
/*Cerramos los directorios*/
closedir(dirp);
closedir(dirp2);
printf("***************************SINCRONIZACION COMPLETADA*************************** \n");
return 0;
}
