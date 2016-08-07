#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
//Structs
char buffer[1];

struct Particion{
  char part_status,part_type,part_fit;
  int part_start,part_size;
  char part_name [16];

};
struct Particion lista [3];
struct DiscoVirtual{
	int mbr_tamano;
	int  mbr_disk_signature;
	struct tm *mbr_fecha_creacion;
	struct Particion mbr_partition_1;
	struct Particion mbr_partition_2;
	struct Particion mbr_partition_3;
	struct Particion mbr_partition_4;
};
void CrearDisco(char *nombre,char *path,int tam,int multiplo)
{

    int i;
    struct DiscoVirtual disco_temp;
    disco_temp.mbr_tamano=1024*multiplo*tam;
	disco_temp.mbr_disk_signature=numero_random();
	disco_temp.mbr_partition_1.part_size=0;
	disco_temp.mbr_partition_2.part_size=0;
	disco_temp.mbr_partition_3.part_size=0;
	disco_temp.mbr_partition_4.part_size=0;
	disco_temp.mbr_partition_1.part_start=0;
	disco_temp.mbr_partition_2.part_start=0;
	disco_temp.mbr_partition_3.part_start=0;
	disco_temp.mbr_partition_4.part_start=0;
	timer_t tiempo;
	time(&tiempo);
    disco_temp.mbr_fecha_creacion=localtime(&tiempo);
	printf("Creando disco...\n");
    strcat ( path, nombre);
    FILE *f = fopen (path, "w+b");
	for( i=0;i<tam*multiplo;i++)
		fwrite (buffer, sizeof(buffer), 1024, f);
	rewind(f);
	fwrite(&disco_temp,sizeof(disco_temp),1,f);
	fclose(f);
	printf("Disco creado exitosamente\n");
	inicio();

}
void eliminarDisco(char *path)
{
    char *comando;
    int opcion=0;
    comando =strndup("rm ",3);

    printf("Desea eliminar el disco?(1/0)\n");
    scanf("%d",&opcion);
    if(opcion==1)
    {
        opcion=existsFile(path);
        if(opcion==1)
        {
        strcat(comando,path);
        system(comando);
        printf("Disco Eliminado... \n");
        }
        else
        {
        printf("No Existe el Disco... \n");
        }
        inicio();

    }
    else {printf("Se cancelo la operacion \n");
        inicio();
    }


}
void Lexico(char *temp, char *comando)
{
      int ret;

  if( strncasecmp(temp, "Mkdisk", 5)==0)
  {
      Mkdisk(comando);
  }
  else if( strncasecmp(temp, "Rmdisk", 5)==0)
  {
      Rmdisk(comando);
  }
    else if( strncasecmp(temp, "fdisk", 4)==0)
  {
      fdisk(comando);
  }
    else if( strncmp(temp, "mount", 4)==0)
  {
      leerParticion();
  }

}
void umount(char *comando)
{
char *token;
char *temp;
  int ret;
  int len;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncmp(token, "-id", 2)==0)
         {
             len=strlen(token);
              temp=strndup(token+2, len-2);
              printf("Path: %s\n", temp);
         }
         else {
            Lexico(token,comando);
         }
  }
}
void mount(char *comando)
{char *token;
char *temp;
  int ret;
  int len;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncmp(token, "-Path::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              printf("Path: %s\n", temp);
         }
          else if( strncmp(token, "+Name::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              printf("Name: %s\n", temp);
         }
          else {
            Lexico(token,comando);
         }
  }
}
void fdisk(char *comando)
{
    char *token;
char *temp;
  int ret;
  int len,unit;
  int tam,Add=0,eliminar;
  char type;
   char *nombre, *teliminar;
   char fit;
  char *path;
  fit='W';
  type='P';
  unit=1024;
    eliminar=0;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncasecmp(token, "-Size::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
               tam=strtol(temp,(char**)NULL, 10);
              printf("Tamaño: %s\n", temp);
         }
          else if( strncasecmp(token, "+Unit::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
               if(strcasecmp(temp,"M")==0)
               {
                   unit=1024*1024;
               }
               else if(strcasecmp(temp,"B")==0)
               {
                unit=1;
               }
              printf("Unidad: %s\n", temp);
         }
          else if( strncasecmp(token, "+Type::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              strcpy(type,temp);
              printf("Tipo: %s\n", temp);
         }
           else if( strncasecmp(token, "+Fit::", 5)==0)
         {
             len=strlen(token);
              temp=strndup(token+6, len-5);
              strcpy(fit,temp);
              printf("Fit: %s\n", temp);
         }
            else if( strncasecmp(token, "+Add::", 5)==0)
         {
             len=strlen(token);
              temp=strndup(token+6, len-5);
               Add=strtol(temp,(char**)NULL, 10);
              printf("Add: %s\n", temp);
         }
           else if( strncasecmp(token, "+Delete::", 8)==0)
         {
             len=strlen(token);
              temp=strndup(token+9, len-8);
              teliminar=temp;
              eliminar=1;
              printf("Delete: %s\n", temp);
         }
           else if( strncasecmp(token, "-Path::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);

                 temp=strndup(token+8,(len-7)-2);

             path=temp;
              printf("Path: %s\n", path);
         }
           else if( strncasecmp(token, "-Name::", 6)==0)
         {
              len=strlen(token);
                if((path==NULL)||(tam==-1))
             {
                 temp=strndup(token+8,(len-7)-2);
             }
             else{
                temp=strndup(token+8,(len-7)-3);
             }
             nombre=temp;
              printf("Nombre: %s\n", temp);
         }
         else{
            Lexico(token,comando);
         }
  }
       if(eliminar==0)
         {
             crear_particion(path,type,unit,fit,tam,nombre);
         }

}
void Rmdisk(char *comando)
{
char *token;
char *temp;
  int ret;
  int len;

    while ((token = strsep(&comando, " ")) != NULL)
  {
        if( strncasecmp(token, "-Path::", 6)==0)
         {
             len=strlen(token);
             temp=strndup(token+8,(len-7)-3);
              printf("Path: %s\n", temp);
              eliminarDisco(temp);
         }
        else {
            Lexico(token,comando);
         }
  }
}
void Mkdisk(char *comando)
{
char *token;
char *temp;
  int tamano=-1;
  char *nombre;
  char *path;
  int multiplo=1;
  int len;

    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncasecmp(token, "-Size::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              tamano=strtol(temp,(char**)NULL, 10);
              printf("Tamaño: %s\n", temp);
         }
          else if( strncasecmp(token, "+Unit::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              printf("Unidad: %s\n", temp);
              if(strncasecmp(temp,"M",0)==0)
              {
                  multiplo=1024;
              }
         }
           else if( strncasecmp(token, "-Path::", 6)==0)
         {
             len=strlen(token);
             if((nombre==NULL)||(tamano==-1))
             {
                 temp=strndup(token+8,(len-7)-2);
             }
             else{
                temp=strndup(token+8,(len-7)-3);
             }

              path=temp;

         }
           else if( strncasecmp(token, "-Name::", 6)==0)
         {
              len=strlen(token);
                if((path==NULL)||(tamano==-1))
             {
                 temp=strndup(token+8,(len-7)-2);
             }
             else{
                temp=strndup(token+8,(len-7)-3);
             }
              nombre=temp;

         }
          else {
            Lexico(token,comando);
         }
  }
  printf("Nombre: %s\n", nombre);
  printf("Path: %s\n", path);
  printf("Tamaño: %d\n", tamano);


      CrearDisco(nombre,path,tamano,multiplo);

}
int main()
{
    inicio();
    return 0;
}
int existsFile(char* filename) {
	FILE* f = NULL;
	f = fopen(filename,"r");
	if (f == NULL && errno == ENOENT)
		return 0;
	else {
		fclose(f);
		return 1;
	}

}
void inicio()
{
char comando [256];
    char *token;
    char *string;
    char *espacio=" ";
    printf("Ingresar Comando: ");
    fgets(comando, 1024, stdin);
  string = strdup(comando);

if (string != NULL) {

  espacio = string;

  token = strsep(&string, " ");
    Lexico(token,string);


  free(espacio);
}
}
int numero_random(){
int n;
srand(time(NULL));
n=rand()%200;
return n;
}
void crear_particion(char *path, char type,int unit,char fit,int part_tam, char *nombre)
{
 struct   DiscoVirtual temp;
 struct   Particion part;
 part.part_fit=fit;
 part.part_size=part_tam*unit;
 part.part_status='1';
 part.part_type=type;
    FILE *fp;
    if((fp=fopen(path, "rb")) == NULL) {
      printf("El disco no existe.\n");
      exit(1);
    }

    fread(&temp, sizeof(struct DiscoVirtual), 1, fp);
    fclose(fp);
    if(temp.mbr_partition_1.part_size==0)
    {

      part.part_start=Buscar_inicio(temp,part_tam);
       temp.mbr_partition_1=part;


    }
    else if(temp.mbr_partition_2.part_size==0)
    {
          part.part_start=Buscar_inicio(temp,part_tam);
          temp.mbr_partition_2=part;
    }
     else if(temp.mbr_partition_3.part_size==0)
    {
         part.part_start=Buscar_inicio(temp,part_tam);
         temp.mbr_partition_3=part;
    }
     else if(temp.mbr_partition_4.part_size==0)
    {
       part.part_start=Buscar_inicio(temp,part_tam);
       temp.mbr_partition_4=part;
    }
    else{
        printf("No se pueden crear mas de 4 particiones... \n");
    }
     printf("Inicia en: %d\n", part.part_start);
     int i;
   FILE *f = fopen (path, "w+b");
      for( i=0;i<temp.mbr_tamano/1024;i++)
		fwrite (buffer, sizeof(buffer), 1024, f);
	rewind(f);
	fwrite(&temp,sizeof(temp),1,f);
	fclose(f);
}
int Buscar_inicio(struct DiscoVirtual temp,int size)
{
    lista [0]=temp.mbr_partition_1;
    lista [1]=temp.mbr_partition_2;
    lista [2]=temp.mbr_partition_3;
    lista [3]=temp.mbr_partition_4;
    ordenarLista();
    int tam= llenos();
     printf("Llenos en: %d\n", tam);
   if(tam==0)
   {
     return(sizeof(struct DiscoVirtual)+1);
   }
   else if(tam==1)
   {
     if((lista[3].part_start-sizeof(struct DiscoVirtual))>size)
     {
       return(sizeof(struct DiscoVirtual)+1);
     }
     else if(temp.mbr_tamano-(lista[3].part_start+lista[3].part_size)>size)
     {
         return(lista[3].part_start+lista[3].part_size+1);
     }
   }
   else if(tam==2)
   {
     if((lista[2].part_start-sizeof(struct DiscoVirtual))>size)
     {
       return(sizeof(struct DiscoVirtual)+1);
     }
     else if((lista[3].part_start+lista[3].part_size)-(lista[2].part_start+lista[2].part_size)>size)
     {
         return(lista[2].part_start+lista[2].part_size+1);
     }
     else if(temp.mbr_tamano-(lista[3].part_start+lista[3].part_size)>size)
     {
         return(lista[3].part_start+lista[3].part_size+1);
     }

   }
   else if ((tam)==3)
   {
        if((lista[1].part_start-sizeof(struct DiscoVirtual))>size)
     {
       return(sizeof(struct DiscoVirtual)+1);
     }
      else if((lista[2].part_start+lista[2].part_size)-(lista[1].part_start+lista[1].part_size)>size)
     {
         return(lista[1].part_start+lista[1].part_size+1);
     }
      else if((lista[3].part_start+lista[3].part_size)-(lista[2].part_start+lista[2].part_size)>size)
     {
         return(lista[2].part_start+lista[2].part_size+1);
     }
     else if(temp.mbr_tamano-(lista[3].part_start+lista[3].part_size)>size)
     {
         return(lista[3].part_start+lista[3].part_size+1);
     }


   }
   return 0;
}
int llenos ()
{
   int cont=0,i=0;
   for(i;4>i;i++)
   {
       if(lista[i].part_size!=0)
       {
           cont=cont+1;
       }
   }
   return cont;

}
void ordenarLista()
{
    int i=0,j=0,aux;
    struct Particion temp;
    for(i=1; i<4; i++) {
        j = i;
        aux = lista[i].part_start;
        temp=lista[i];
    while(j>0 && aux<lista[j-1].part_start) {
        lista[j] = lista[j-1];
        j=j-1;
        }
        lista[j] = temp;
        }
}
void leerParticion()
{
    struct DiscoVirtual temp;
     FILE *fp;
    if((fp=fopen("/home/alfredo/Documents/discon.dsk", "rb")) == NULL) {
      printf("El disco no existe.\n");
      exit(1);
    }

    fread(&temp, sizeof(struct DiscoVirtual), 1, fp);
    fclose(fp);
    printf("Particion 1 Inicia en: %d\n", temp.mbr_partition_1.part_start);
     printf("Particion 1 Tamaño de: %d\n", temp.mbr_partition_1.part_size);
      printf("Particion 2 Inicia en: %d\n", temp.mbr_partition_2.part_start);
     printf("Particion 2 Tamaño de: %d\n", temp.mbr_partition_2.part_size);

}