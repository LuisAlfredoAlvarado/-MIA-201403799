#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
//Structs
char buffer[1];
char *ruta;
int iguales;
char letra='a';
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
struct DiscoVirtual contenedor;
struct EBR
{
   char part_status,part_fit;
   int part_start,part_size,part_next;
   char part_name [16];
};
struct Part{
    char part_status,part_type,part_fit,tipo;
  int part_start,part_size,part_next,id;
  char *part_name;
};
struct Montadas
{
    char *path;
    char letra;
    struct Part parts[24];

};
struct Montadas Mounts [24];
char *String_SinComillas;
struct EBR EBR_Contenedor,EBR_Auxiliar,anterior;
void CrearDisco(char *nombre,char *path,int tam,int multiplo)
{

    int i;
    char *comando;
    comando=strndup("mkdir \"",7);
    strcat(comando,path);
    strcat(comando,"\"");
    system(comando);
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

}
void eliminarDisco(char* path)
{

    int opcion=0;
    char *comando;
    comando =strndup("rm ",3);
    printf("Path %s\n",path);
    printf("Desea eliminar el disco?(1/0)\n");
    scanf("%d",&opcion);
    if(opcion==1)
    {
        //opcion=existsFile(String_SinComillas);
        if(opcion==1)
        {
        strcat(comando,path);
        printf("comando... %s\n",comando);
        system(comando);
        printf("Disco Eliminado... \n");
        }
        else
        {
        printf("No Existe el Disco... \n");
        }
    }
    else {printf("Se cancelo la operacion \n");
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
   else if( strncasecmp(temp, "rep", 2)==0)
  {
      Reportes(comando);
    // ruta="/home/alfredo/discon.dsk";
    // ReporteDisco("/home/alfredo/disco.jpg");
  }
    else if( strncasecmp(temp, "fdisk", 4)==0)
  {
      fdisk(comando);
  }
    else if( strncmp(temp, "mount", 4)==0)
  {
      mount(comando);
  }
    else if( strncmp(temp, "unmount", 6)==0)
  {
      unmount(comando);
  }
   else if( strncmp(temp, "exec", 4)==0)
  {
      exec(comando);
  }
    else if( strncmp(temp, "leer", 4)==0)
  {
      leerParticion();
  }

}
void unmount(char *comando)
{
char *token;
char *temp;
char *letras;
char *id;
char unit;
  int id_int;
  int len;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncmp(token, "-id", 2)==0)
         {
             len=strlen(token);
              temp=strndup(token+8, len-6);
              letras=strndup(temp, 1);
              id=strndup(temp+1, strlen(temp)-1);
              id_int=strtol(id,(char**)NULL, 10);
              unit=letras[0];
            DesmontarParticion(unit,id_int);
         }
         else {
            Lexico(token,comando);
         }
  }
}
void mount(char *comando)
{
char *token;
char *temp;
  char *nombre;
  int ret,len,p=0,n=0;
    while ((token = strsep(&comando, " ")) != NULL)
  {
          if( strncasecmp(token, "-Path::", 6)==0)
         {
             if((p+n)==0)
             {len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              p=1;
             }
             else{
                len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              p=1;
             }
              ruta=temp;
              if(ruta[strlen(ruta)-1]!='"')
              {
                  p=2;
              }
              else{
                BorrarComillas(ruta);
                ruta=String_SinComillas;
              }

         }
          else if( strncasecmp(token, "-Name::", 6)==0)
         {
            if((p+n)==0)
             {len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              n=1;
             }
             else{
                len=strlen(token)-1;
              temp=strndup(token+7, len-7);
              n=1;
             }
              nombre=temp;
              if(nombre[strlen(nombre)-1]!='"')
              {
                  n=2;
              }
              else{
                  BorrarComillas(nombre);
                       nombre=String_SinComillas;
              }

         }
          else {
                if(p==2)
                {
                 strcat(ruta," ");
                 int i=0;
                 while(token[i]!='\0')
                 {
                     if(token[i]=='"')
                     {
                         token[i+1]='\0';

                     }
                      i++;
                 }
                 strcat(ruta,token);
                if(ruta[strlen(ruta)-1]!='"')
                   {
                  p=2;
                   }
                   else
                   {
                       p=1;
                       BorrarComillas(ruta);
                       ruta=String_SinComillas;
                   }

                }
                else if(n==2)
                {
                 strcat(nombre," ");
                 int i=0;
                while(token[i]!='\0')
                 {
                     if(token[i]=='"')
                     {
                         token[i+1]='\0';
                     }
                     i++;
                 }
                 strcat(nombre,token);
                if(nombre[strlen(nombre)-1]!='"')
                   {
                  n=2;
                   }
                   else
                   {
                       n=1;
                       BorrarComillas(nombre);
                       nombre=String_SinComillas;
                   }

                }
                else{
                      if(p+n==2)
                        {
                            MontarParticion(ruta,nombre);
                        }
                        else{  printf("Para montar una particion se necesita el nombre y la ruta\n");}
                    return;
                }
                 //return;
         }
  }
  if(p+n==2)
  {
      MontarParticion(ruta,nombre);
  }
  else{  printf("Para montar una particion se necesita el nombre y la ruta\n");}
  inicio();
}
void fdisk(char *comando)
{
  char *token;
char *temp;
  int ret;
  int len,unit;
  int tam=-1,Add=0,eliminar=0,nom=0,pat=0,crear=0,Agregar=0;
  char type;
   char *nombre, *teliminar;
   char fit;
  char *path;
  fit='W';
  type='P';
  unit=1024;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncasecmp(token, "-Size::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
               tam=strtol(temp,(char**)NULL, 10);
              printf("Tamaño: %s\n", temp);
              crear=1;
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
               if(strcasecmp(temp,"E")==0)
               {
                   type='E';
               }
               else if(strcasecmp(temp,"L")==0)
               {
                type='L';
               }
               else if(strcasecmp(temp,"P")==0)
               {
               }
               else{
                 printf("Error tipo de particion desconocida: %s\n", temp);
                 exit(0);
               }
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
             Agregar=1;
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
                 if((nom==0)||(tam==-1))
             {
                 temp=strndup(token+8,(len-7)-2);
             }
             else{
                temp=strndup(token+8,(len-7)-3);
             }
             pat=1;
             path=temp;
              printf("Path: %s\n", path);
         }
           else if( strncasecmp(token, "-Name::", 6)==0)
         {
              len=strlen(token);
                if((pat==0)||(tam==-1))
             {
                 temp=strndup(token+8,(len-7)-2);
             }
             else{
                temp=strndup(token+8,(len-7)-3);
             }
             nom=1;
             nombre=temp;
              printf("Nombre: %s\n", temp);
         }
         else{

            Lexico(token,comando);
         }
  }
       if(Agregar+eliminar+crear>2)
         {
              printf("Las operaciones de Creación, Agregar Espacio y Eliminar son excluyentes. \n");
              return;
         }
         else if(eliminar==1)
         {
             EliminarParticion(nombre,path,teliminar);
         }
          else if(Agregar==1)
         {
             //
         }
          else if(crear==1)
         {
             crear_particion(path,type,unit,fit,tam,nombre);
         }
}
void Rmdisk(char *comando)
{
char *token;
char *temp;
char *path ;
  int ret;
  int len,p=0;

    while ((token = strsep(&comando, " ")) != NULL)
  {
        if( strncasecmp(token, "-Path::", 6)==0)
         {


              len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              ret=0;
              while(temp[ret]!='\0')
                 {
                     if((temp[ret]=='"')&&(ret!=0))
                     {
                         temp[ret+1]='\0';
                     }
                     ret++;
                 }
              path=temp;
              if(path[strlen(path)-1]!='"')
              {
                  p=2;

              }
              else{
                eliminarDisco(path);
              }

         }
        else {
                if(p==2)
                {
                 strcat(path," ");
                 int i=0;
                 while(token[i]!='\0')
                 {
                     if(token[i]=='"')
                     {
                         token[i+1]='\0';

                     }
                      i++;
                 }
                 strcat(path,token);
                if(path[strlen(path)-1]!='"')
                   {
                  p=2;
                   }
                   else
                   {
                    p=1;
                       eliminarDisco(path);
                   }

                }
                else{
                 // Lexico(token,comando);
                }
         }
  }
}
int Mkdisk(char *comando)
{
char *token;
char *temp;
  int tamano=-1;
  char *nombre;
  char *path;
  int multiplo=1;
  int len,p=0,n=0,s=0;

    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncasecmp(token, "-Size::", 6)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              tamano=strtol(temp,(char**)NULL, 10);
              printf("Tamaño: %s\n", temp);
              s=1;
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
             if((p+n)==0)
             {len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              p=1;
             }
             else{
                len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              p=1;
             }
              path=temp;
              if(path[strlen(path)-1]!='"')
              {
                  p=2;
              }
              else{
                BorrarComillas(path);
                path=String_SinComillas;
              }

         }
          else if( strncasecmp(token, "-Name::", 6)==0)
         {
            if((p+n)==0)
             {len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              n=1;
             }
             else{
                len=strlen(token)-1;
              temp=strndup(token+7, len-6);
              n=1;
             }
              nombre=temp;
              if(nombre[strlen(nombre)-1]!='"')
              {
                  n=2;
              }
              else{
                  BorrarComillas(nombre);
                       nombre=String_SinComillas;
              }

         }
          else {
                if(p==2)
                {
                 strcat(path," ");
                 int i=0;
                 while(token[i]!='\0')
                 {
                     if(token[i]=='"')
                     {
                         token[i+1]='\0';

                     }
                      i++;
                 }
                 strcat(path,token);
                if(path[strlen(path)-1]!='"')
                   {
                  p=2;
                   }
                   else
                   {
                       p=1;
                       BorrarComillas(path);
                       path=String_SinComillas;
                   }

                }
                else if(n==2)
                {
                 strcat(nombre," ");
                 int i=0;
                while(token[i]!='\0')
                 {
                     if(token[i]=='"')
                     {
                         token[i+1]='\0';
                     }
                     i++;
                 }
                 strcat(nombre,token);
                if(nombre[strlen(nombre)-1]!='"')
                   {
                  n=2;
                   }
                   else
                   {
                       n=1;
                       BorrarComillas(nombre);
                       nombre=String_SinComillas;
                   }

                }
                else{
                    CrearDisco(nombre,path,tamano,multiplo);
                    return;
                }
                 //return;
         }
  }
  printf("Name: %s\n", nombre);
  printf("Path: %s\n", path);
  CrearDisco(nombre,path,tamano,multiplo);
}
int main()
{
   Inicializar();
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
void BorrarComillas(char *cadena)
{
    char *temp=cadena;
    int i=0;
    for(i;i<strlen(temp)-2;i++)
    {
        cadena[i]=temp[i+1];
    }
    cadena[i]='\0';
    String_SinComillas=cadena;
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
 ruta=path;
 iguales=1;
 struct   DiscoVirtual temp;
 struct   Particion part;
 struct EBR temporal;
 part.part_fit=fit;
 part.part_size=part_tam*unit;
 part.part_status='1';
 part.part_type=type;
 strcpy (part.part_name,nombre);
 CargarDisco(path);
 int Extendida=HayExtendida();
 temp=contenedor;
 Buscar_nombre(nombre,type);
  if((type=='E')&&(Extendida==1))
  {
      printf("No se pueden crear mas de 2 particiones Extendidas... \n");
      return;
  }
  if((type=='L'&&Extendida!=1))
  {
       printf("No se pueden crear una particion lógica sin Extendidas... \n");
      return;
  }

    if(iguales==1)
  {
       printf("Ya existe una particion con ese nombre... \n");
      return;
  }

   if(type=='L')
  {
      temporal.part_fit=fit;
      temporal.part_size=part_tam*unit;
      temporal.part_status='1';
      strcpy (temporal.part_name,nombre);
      EBR_Auxiliar=temporal;
      BuscarEBR(path);
      Buscar_nombre(nombre,type);
      if(iguales==0)
      {
          insertarLogica(lista[0].part_size);
      }
      else{printf("Ya existe una particion con ese nombre... \n");}

      return;
  }
  else
  {
      if(temp.mbr_partition_1.part_size==0)
    {

      part.part_start=Buscar_inicio(temp,part_tam);
       temp.mbr_partition_1=part;
       if(type=='E')
        {
            EBR_Contenedor.part_size=0;
            EBR_Contenedor.part_start=0;
            EBR_Contenedor.part_next=0;
            strcpy (EBR_Contenedor.part_name,"Prueba");
            EscribirEBRU(path,part.part_start);
        }
    }
    else if(temp.mbr_partition_2.part_size==0)
    {
          part.part_start=Buscar_inicio(temp,part_tam);
          temp.mbr_partition_2=part;
          if(type=='E')
        {
            EBR_Contenedor.part_size=0;
            EBR_Contenedor.part_start=0;
            EBR_Contenedor.part_next=0;
            strcpy (EBR_Contenedor.part_name,"Prueba");
            EscribirEBRU(path,part.part_start);
        }
    }
     else if(temp.mbr_partition_3.part_size==0)
    {
         part.part_start=Buscar_inicio(temp,part_tam);
         temp.mbr_partition_3=part;
          if(type=='E')
        {
            EBR_Contenedor.part_size=0;
            EBR_Contenedor.part_start=0;
            EBR_Contenedor.part_next=0;
            strcpy (EBR_Contenedor.part_name,"Prueba");
            EscribirEBRU(path,part.part_start);
        }
    }
     else if(temp.mbr_partition_4.part_size==0)
    {
       part.part_start=Buscar_inicio(temp,part_tam);
       temp.mbr_partition_4=part;
        if(type=='E')
        {
            EBR_Contenedor.part_size=0;
            EBR_Contenedor.part_start=0;
            EBR_Contenedor.part_next=0;
            strcpy (EBR_Contenedor.part_name,"Prueba");
            EscribirEBRU(path,part.part_start);
        }
    }
    else{
        printf("No se pueden crear mas de 4 particiones... \n");
    }
    contenedor=temp;
     printf("Inicia en: %d\n", part.part_start);
     EscribirMBR(path);

  }

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
     else if((lista[3].part_start)-(lista[2].part_start+lista[2].part_size)>size)
     {
         printf("Aqui clavo de 2: %d\n", lista[2].part_start);
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
      else if((lista[2].part_start)-(lista[1].part_start+lista[1].part_size)>size)
     {
         return(lista[1].part_start+lista[1].part_size+1);
     }
      else if((lista[3].part_start)-(lista[2].part_start+lista[2].part_size)>size)
     {
         return(lista[2].part_start+lista[2].part_size+1);
     }
     else if(temp.mbr_tamano-(lista[3].part_start+lista[3].part_size)>size)
     {
         return(lista[3].part_start+lista[3].part_size+1);
     }


   }
   else {
     printf("No se encontro espacio ... \n");
     exit(0);
   }
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
    if((fp=fopen("/home/alfredo/discon.dsk", "rb")) == NULL) {
      printf("El disco no existe.\n");
      exit(1);
    }

    fread(&temp, sizeof(struct DiscoVirtual), 1, fp);
    fclose(fp);
     printf("Nombre Particion 1: %s\n", temp.mbr_partition_1.part_name);
    printf("Particion 1 Inicia en: %d\n", temp.mbr_partition_1.part_start);
    printf("Particion 1 Tamaño de: %d\n", temp.mbr_partition_1.part_size);
    if(temp.mbr_partition_1.part_type=='E')
    {
          CargarEBR("/home/alfredo/discon.dsk",temp.mbr_partition_1.part_start);
          leerLogicas("/home/alfredo/discon.dsk");
    }
     printf("Nombre Particion 2: %s\n", temp.mbr_partition_2.part_name);
    printf("Particion 2 Inicia en: %d\n", temp.mbr_partition_2.part_start);
    printf("Particion 2 Tamaño de: %d\n", temp.mbr_partition_2.part_size);
    if(temp.mbr_partition_2.part_type=='E')
    {
        CargarEBR("/home/alfredo/discon.dsk",temp.mbr_partition_2.part_start);
        leerLogicas("/home/alfredo/discon.dsk");
    }
     printf("Nombre Particion 3: %s\n", temp.mbr_partition_3.part_name);
    printf("Particion 3 Inicia en: %d\n", temp.mbr_partition_3.part_start);
    printf("Particion 3 Tamaño de: %d\n", temp.mbr_partition_3.part_size);
    if(temp.mbr_partition_3.part_type=='E')
    {
        CargarEBR("/home/alfredo/discon.dsk",temp.mbr_partition_3.part_start);
           leerLogicas("/home/alfredo/discon.dsk");
    }
     printf("Nombre Particion 4: %s\n", temp.mbr_partition_4.part_name);
    printf("Particion 4 Inicia en: %d\n", temp.mbr_partition_4.part_start);
    printf("Particion 4 Tamaño de: %d\n", temp.mbr_partition_4.part_size);
    if(temp.mbr_partition_4.part_type=='E')
    {
        CargarEBR("/home/alfredo/discon.dsk",temp.mbr_partition_4.part_start);
          leerLogicas("/home/alfredo/discon.dsk");
    }

}
void exec(char *comando)
{
    char *temp;
    char *token;
    char *path;
    int len;
    char caracter;
	char *comando2="";
	int contador=0;
	char  comandos [256];
    while ((token = strsep(&comando, " ")) != NULL)
  {
               len=strlen(token);
               temp=strndup(token,len-1);
               path=temp;

   }
   FILE *archivo;
	archivo = fopen(path,"r");

	if (archivo == NULL){

		printf("\nError No se econtro el script. \n\n");
        }else{
	    while (feof(archivo) == 0)
            {
            caracter = fgetc(archivo);
            comandos [contador]=caracter;
            contador++;
            }
        }
         fclose(archivo);
         comando2=strndup(comandos,contador);
         printf("Leido: %s\n", comando2);
         token = strsep(&comando2, " ");
         Lexico(token,comando2);


        // free(comandos);
       //inicio();*/
}
void EliminarParticion(char *name, char *path, char *type)
{
    ruta=path;
   CargarDisco(path);
   int len = strlen(name)-1;
   iguales=0;
        if( strncasecmp(contenedor.mbr_partition_1.part_name, name, len)==0)
         {
             if(strncasecmp(type, "Fast", 3)==0)
             {
                contenedor.mbr_partition_1.part_status='0';
             }
             else if(strncasecmp(type, "Full", 3)==0)
             {
                 VaciarEspacio(contenedor.mbr_partition_1.part_start,contenedor.mbr_partition_1.part_start+contenedor.mbr_partition_1.part_size);
                 contenedor.mbr_partition_1.part_size=0;
                 contenedor.mbr_partition_1.part_start=0;
                 contenedor.mbr_partition_1.part_type='N';
             }
             else{
                printf("Tipo de Eliminación Desconocida\n");
             }
         }
         else if( strncasecmp(contenedor.mbr_partition_2.part_name, name, len)==0)
         {
              if(strncasecmp(type, "Fast", 3)==0)
             {
                contenedor.mbr_partition_2.part_status='0';
             }
             else if(strncasecmp(type, "Full", 3)==0)
             {
                 VaciarEspacio(contenedor.mbr_partition_2.part_start,contenedor.mbr_partition_2.part_start+contenedor.mbr_partition_2.part_size);
                 contenedor.mbr_partition_2.part_size=0;
                 contenedor.mbr_partition_2.part_start=0;
                 contenedor.mbr_partition_2.part_type='N';
             }
             else{
                printf("Tipo de Eliminación Desconocida\n");
             }
         }
        else  if( strncasecmp(contenedor.mbr_partition_3.part_name, name, len)==0)
         {
              if(strncasecmp(type, "Fast", 3)==0)
             {
                contenedor.mbr_partition_3.part_status='0';
             }
             else if(strncasecmp(type, "Full", 3)==0)
             {
                 VaciarEspacio(contenedor.mbr_partition_3.part_start,contenedor.mbr_partition_3.part_start+contenedor.mbr_partition_3.part_size);
                 contenedor.mbr_partition_3.part_size=0;
                 contenedor.mbr_partition_3.part_start=0;
                 contenedor.mbr_partition_3.part_type='N';
             }
             else{
                printf("Tipo de Eliminación Desconocida\n");
             }
         }
        else  if( strncasecmp(contenedor.mbr_partition_4.part_name, name, len)==0)
         {
              if(strncasecmp(type, "Fast", 3)==0)
             {
                contenedor.mbr_partition_4.part_status='0';
             }
             else if(strncasecmp(type, "Full", 3)==0)
             {
                 VaciarEspacio(contenedor.mbr_partition_4.part_start,contenedor.mbr_partition_4.part_start+contenedor.mbr_partition_4.part_size);
                 contenedor.mbr_partition_4.part_size=0;
                 contenedor.mbr_partition_4.part_start=0;
                 contenedor.mbr_partition_4.part_type='N';
             }
             else{
                printf("Tipo de Eliminación Desconocida\n");
             }
         }
         else{
                 printf("Eliminando: %s  en la ruta: %s\n", name,ruta);
                BuscarEBR(ruta);
                Buscar_nombre(name,'L');
                if(iguales==1)
                {
                    EliminarParticionLogica(name);
                }
                else{
             printf("No se encontro la particion\n");
                }
         }
         EscribirMBR(path);

}
void EscribirMBR(char *path)
{
    FILE *f = fopen (path, "rb+");
	fseek(f, 0, SEEK_SET);
	fwrite(&contenedor,sizeof(contenedor),1,f);
	fclose(f);
}
void CargarDisco(char *path)
{
        FILE *fp;
    if((fp=fopen(path, "rb")) == NULL) {
      printf("El disco no existe.\n");
      exit(1);
    }

    fread(&contenedor, sizeof(struct DiscoVirtual), 1, fp);
    fclose(fp);
    lista[0]=contenedor.mbr_partition_1;
    lista[1]=contenedor.mbr_partition_2;
    lista[2]=contenedor.mbr_partition_3;
    lista[3]=contenedor.mbr_partition_4;
}
int HayExtendida()
{
    if((contenedor.mbr_partition_1.part_type=='E')||(contenedor.mbr_partition_2.part_type=='E')||(contenedor.mbr_partition_3.part_type=='E')||(contenedor.mbr_partition_4.part_type=='E'))
    {
        return 1;
    }
    else{ return 0;}
}
void EscribirEBRU(char *path, int inicio)
{
    printf("Escribiendo EBR en pos %d... \n",inicio);
    printf("Escribiendo... EBR Nombre EBR: %s y tamaño %d \n", EBR_Contenedor.part_name,EBR_Contenedor.part_size);
    FILE *f = fopen (path, "rb+");
	fseek(f, inicio, SEEK_SET);
	fwrite(&EBR_Contenedor,sizeof(EBR_Contenedor),1,f);
	fclose(f);
}
void CargarEBR(char *path,int inicio)
{
      //printf("Leyendo EBR en pos %d... \n",inicio);
        FILE *fp;
    if((fp=fopen(path, "rb")) == NULL) {
      printf("El disco no existe.\n");
      exit(1);
    }
    fseek(fp, inicio, SEEK_SET);
    fread(&EBR_Contenedor, sizeof(EBR_Contenedor), 1, fp);
    fclose(fp);
   //  printf("Nombre  EBR leido %s... \n",EBR_Contenedor.part_name);
}
void insertarLogica(int tam)
{
  /*printf("Estoy en : %s ... \n",EBR_Contenedor.part_name);
  printf("Valor de anterior: %s\n",anterior.part_name);
   printf("Restas: %d\n",EBR_Contenedor.part_start-(anterior.part_start+anterior.part_size));*/
     if(EBR_Contenedor.part_size==0)
     {
         if(lista[0].part_size+lista[0].part_start>EBR_Contenedor.part_start+EBR_Auxiliar.part_size)
         {
         EBR_Auxiliar.part_start=EBR_Contenedor.part_start;
         EBR_Auxiliar.part_next=EBR_Auxiliar.part_size+EBR_Auxiliar.part_start+2;
         EBR_Contenedor=EBR_Auxiliar;
         EscribirEBRU(ruta,EBR_Auxiliar.part_start);
         EBR_Contenedor.part_size=0;
         EBR_Contenedor.part_start=EBR_Contenedor.part_next;
         EBR_Contenedor.part_next=0;
         EscribirEBRU(ruta,EBR_Auxiliar.part_next);
         printf("Particion logica Creada: %s\n",EBR_Auxiliar.part_name);
         }
         else
         {
             printf("No hay espacio Necesario para crear la partición logica... \n");
             return;
         }
     }
     else if(EBR_Contenedor.part_start-anterior.part_start-anterior.part_size>EBR_Auxiliar.part_size)
     {
          printf("Entre estot logica Creada: %s\n",EBR_Auxiliar.part_name);
         EBR_Auxiliar.part_start=anterior.part_start+anterior.part_size;
         EBR_Auxiliar.part_next=EBR_Contenedor.part_start;
         anterior.part_next=anterior.part_start+anterior.part_size;
         EBR_Contenedor=EBR_Auxiliar;
         EscribirEBRU(ruta,EBR_Contenedor.part_start);
         EBR_Contenedor=anterior;
         EscribirEBRU(ruta,EBR_Contenedor.part_start);

     }
     else{
        anterior=EBR_Contenedor;
        CargarEBR(ruta,anterior.part_next);
        insertarLogica(tam);
     }
}
void BuscarEBR(char *path)
{
    if(contenedor.mbr_partition_1.part_type=='E')
    {
      CargarEBR(path,contenedor.mbr_partition_1.part_start);
      EBR_Contenedor.part_start=contenedor.mbr_partition_1.part_start;
      lista [0]=contenedor.mbr_partition_1;
    }
    else if(contenedor.mbr_partition_2.part_type=='E')
    {
      CargarEBR(path,contenedor.mbr_partition_2.part_start);
      EBR_Contenedor.part_start=contenedor.mbr_partition_2.part_start;
       lista [0]=contenedor.mbr_partition_2;
    }
    else if(contenedor.mbr_partition_3.part_type=='E')
    {
      CargarEBR(path,contenedor.mbr_partition_3.part_start);
      EBR_Contenedor.part_start=contenedor.mbr_partition_3.part_start;
       lista [0]=contenedor.mbr_partition_3;
    }
    else if(contenedor.mbr_partition_4.part_type=='E')
    {
      CargarEBR(path,contenedor.mbr_partition_4.part_start);
      EBR_Contenedor.part_start=contenedor.mbr_partition_4.part_start;
       lista [0]=contenedor.mbr_partition_4;
    }
    else {lista [0].part_size=0;}
}
void leerLogicas(char *path)
{
    while(EBR_Contenedor.part_size!=0)
    {
         printf("Nombre Particion Logica: %s\n", EBR_Contenedor.part_name);
         CargarEBR(path,EBR_Contenedor.part_next);
    }
}
void Buscar_nombre(char *name)
{
    int length=strlen(name)-1;
     if( strncasecmp(name, contenedor.mbr_partition_1.part_name, length)==0)
     {
        iguales=1;
       return 1;
     }
     else if( strncasecmp(name, contenedor.mbr_partition_2.part_name, length)==0)
     {
         iguales=1;
         return 1;
     }
      else if( strncasecmp(name, contenedor.mbr_partition_3.part_name, length)==0)
     {
         iguales=1;
         return 1;
     }
      else if( strncasecmp(name, contenedor.mbr_partition_4.part_name, length)==0)
     {
         iguales=1;
         return 1;
     }
     else

        {

           if( contenedor.mbr_partition_1.part_type=='E')
            {
              NombreIgualEBR(name,contenedor.mbr_partition_1.part_start,length);
              CargarEBR(ruta,contenedor.mbr_partition_1.part_start);
            }
           else if( contenedor.mbr_partition_2.part_type=='E')
            {
                NombreIgualEBR(name,contenedor.mbr_partition_2.part_start,length);
                 CargarEBR(ruta,contenedor.mbr_partition_2.part_start);
            }
            else if( contenedor.mbr_partition_3.part_type=='E')
            {
                 NombreIgualEBR(name,contenedor.mbr_partition_3.part_start,length);
                  CargarEBR(ruta,contenedor.mbr_partition_3.part_start);
            }
            else if( contenedor.mbr_partition_4.part_type=='E')
            {
                NombreIgualEBR(name,contenedor.mbr_partition_4.part_start,length);
                 CargarEBR(ruta,contenedor.mbr_partition_4.part_start);
            }
            else{
                iguales=0;
            return 0;}
            }



}
int NombreIgualEBR(char *names,int start,int len)
{
      printf("Buscando: %s de largo %d\n", names,len);
    while(EBR_Contenedor.part_size!=0)
    {
         if( strncasecmp(names, EBR_Contenedor.part_name, len)==0)
        {
            iguales=1;
         return 1;
        }
         CargarEBR(ruta,EBR_Contenedor.part_next);
    }
   iguales=0;
}
void VaciarEspacio(int in,int fin)
{
    int i;
     FILE *f = fopen (ruta, "rb9");
	for( i=in;i<fin;i++)
		fwrite (buffer, sizeof(buffer), 1, f);
	fclose(f);
}
void EliminarParticionLogica(char *name)
{

     printf("Eliminando: %s  en la ruta: %s\n", name,ruta);
    BuscarEBR(ruta);
    anterior.part_start=-1;
    int len=strlen(name)-1;
    while( EBR_Contenedor.part_size!=0)
    {
        if(strncasecmp(name, EBR_Contenedor.part_name, len)==0)
        {
            if(anterior.part_start!=1)
            {
                anterior.part_next=EBR_Contenedor.part_next;
                VaciarEspacio(EBR_Contenedor.part_start,EBR_Contenedor.part_size+EBR_Contenedor.part_start);
                EBR_Contenedor=anterior;
                EscribirEBRU(ruta,EBR_Contenedor.part_start);
            }
            else{
                EBR_Contenedor.part_size=0;
              EscribirEBRU(ruta,EBR_Contenedor.part_start);
             }
              return;
        }
       anterior=EBR_Contenedor;
       CargarEBR(ruta,EBR_Contenedor.part_next);
    }
}
void AddParticion(char *nombre,char *path,int cuanto)
{
    CargarDisco(path);
    lista [0]=contenedor.mbr_partition_1;
    lista [1]=contenedor.mbr_partition_2;
    lista [2]=contenedor.mbr_partition_3;
    lista [3]=contenedor.mbr_partition_4;
    ordenarLista();
    int length=strlen(nombre)-1;
    if(strncasecmp(nombre, lista [0].part_name, length)==0)
    {
        AddAuxiliar(cuanto,0);
    }
    else if(strncasecmp(nombre, lista [1].part_name, length)==0)
    {
        AddAuxiliar(cuanto,1);
    }
    else if(strncasecmp(nombre, lista [2].part_name, length)==0)
    {
        AddAuxiliar(cuanto,2);
    }
    else if(strncasecmp(nombre, lista [3].part_name, length)==0)
    {
        AddAuxiliar(cuanto,3);
    }
    else{
         iguales=0;
         Buscar_nombre(nombre);
         if(iguales==0)
         {
              printf("No hay una partición con ese nombre.\n");
         }
         else{
               // Add
         }

    }
}
void AddAuxiliar(int cuanto, int pos)
{
    if(cuanto>0)
        {
             if(pos<3)
             {
                if(lista [pos+1].part_start-(lista [pos].part_size+lista [pos].part_start)>=cuanto)
                {
                   printf("Espacio Agregado Correctamente.\n");
                   lista [pos].part_size+=cuanto;
                }
                else{
                printf("No hay espacio suficiente para agregarle a la partición.\n");
                }
             }
             else{
                if(contenedor.mbr_tamano-(lista [pos].part_size+lista [pos].part_start)>=cuanto)
                {
                   printf("Espacio Agregado Correctamente.\n");
                   lista [pos].part_size+=cuanto;
                }
                else{
                printf("No hay espacio suficiente para agregarle a la partición.\n");
                }
             }
        }
        else if(cuanto<0)
        {
            if(0<lista [pos].part_size+cuanto)
            {
               printf("Espacio Reducido Correctamente.\n");
               lista [pos].part_size+=cuanto;
            }
            else{
                printf("No se puede reducir el  espacio  de la partición.\n");
                }
        }
    contenedor.mbr_partition_1=lista [0];
    contenedor.mbr_partition_2=lista [1];
    contenedor.mbr_partition_3=lista [2];
    contenedor.mbr_partition_4=lista [3];
    EscribirMBR(ruta);
}
void AddEBR(int cuanto)
{
    BuscarEBR(ruta);
    if(EBR_Contenedor.part_size==0)
     {

     }
     else if(EBR_Contenedor.part_start-anterior.part_start-anterior.part_size>EBR_Auxiliar.part_size)
     {
          printf("Entre estot logica Creada: %s\n",EBR_Auxiliar.part_name);
         EBR_Auxiliar.part_start=anterior.part_start+anterior.part_size;
         EBR_Auxiliar.part_next=EBR_Contenedor.part_start;
         anterior.part_next=anterior.part_start+anterior.part_size;
         EBR_Contenedor=EBR_Auxiliar;
         EscribirEBRU(ruta,EBR_Contenedor.part_start);
         EBR_Contenedor=anterior;
         EscribirEBRU(ruta,EBR_Contenedor.part_start);

     }
     else{
        anterior=EBR_Contenedor;
        CargarEBR(ruta,anterior.part_next);
        AddEBR(cuanto);
     }
}
void Inicializar()
{
  int i,j;
    for(i=0;i<24;i++)
    {
        Mounts[i].letra='0';
        for(j=0;j<24;j++)
        {
             Mounts[i].parts[j].id==0;
        }
    }
}
void MontarAuxiliar(char *path, char *nombre,char type)
{
    int i;
    letra='a';
    struct Part temporal;
   if(type=='N')
   {
       temporal.part_fit= lista[0].part_fit;
       temporal.part_name=nombre;
       temporal.part_size= lista[0].part_size;
       temporal.part_start= lista[0].part_start;
       temporal.part_status=lista[0].part_status;
        temporal.part_type=lista[0].part_type;
        temporal.tipo='N';
   }
   else{
     temporal.part_name=nombre;
     temporal.part_fit=EBR_Contenedor.part_fit;
     temporal.part_size=EBR_Contenedor.part_size;
     temporal.part_start=EBR_Contenedor.part_start;
     temporal.part_status=EBR_Contenedor.part_status;
     temporal.part_next=EBR_Contenedor.part_next;
     temporal.tipo='L';
   }
     for(i=0;i<24;i++)
    {

        if(Mounts[i].letra=='0')
        {
            Mounts[i].letra=letra;
            Mounts[i].path=path;
            Mounts[i].parts[0]=temporal;
            Mounts[i].parts[0].id=i+1;
            i=24;
            printf("Montada en Letra: %c y ID: %d\n",letra,1);
        }
        else if(strncasecmp(path, Mounts[i].path, strlen(Mounts[i].path)-1)==0)
        {
            int j=0;
            while(Mounts[i].parts[j].id!=0)
            {
                if(strncasecmp(nombre, Mounts[i].parts[j].part_name, strlen(nombre)-1)==0)
                {
                    printf("La partición que desea montar ya está montada\n");
                    return;
                }
                j++;
            }
            Mounts[i].parts[j]=temporal;
            Mounts[i].parts[j].id=j+1;
            printf("Montada en Letra: %c y ID: %d\n",Mounts[i].letra,j+1);
             i=24;
            return;
        }
        letra++;
    }
     printf("Termino \n");
}
void MontarParticion(char *path, char *nombre)
{
    iguales=0;
    CargarDisco(path);
    Buscar_nombre(nombre);
    if(iguales==1)
    {
      if(strncasecmp(nombre,contenedor.mbr_partition_1.part_name , strlen(nombre)-1)==0)
      {
          lista[0]=contenedor.mbr_partition_1;
          MontarAuxiliar(path,nombre,'N');
      }
      else if (strncasecmp(nombre,contenedor.mbr_partition_2.part_name , strlen(nombre)-1)==0)
      {
           lista[0]=contenedor.mbr_partition_2;
        MontarAuxiliar(path,nombre,'N');
      }
      else if (strncasecmp(nombre,contenedor.mbr_partition_3.part_name , strlen(nombre)-1)==0)
      {
           lista[0]=contenedor.mbr_partition_3;
           MontarAuxiliar(path,nombre,'N');
      }
      else if (strncasecmp(nombre,contenedor.mbr_partition_4.part_name , strlen(nombre)-1)==0)
      {
           lista[0]=contenedor.mbr_partition_4;
           MontarAuxiliar(path,nombre,'N');
      }
      else{
         BuscarEBR(path);
         while(EBR_Contenedor.part_size!=0)
         {
             if (strncasecmp(nombre,EBR_Contenedor.part_name , strlen(nombre)-1)==0)
             {
                    MontarAuxiliar(path,nombre,'L');
                    return;
             }
             CargarEBR(path,EBR_Contenedor.part_next);
         }
      }

    }
    else{
          printf("La partición que se quiere montar no existe.\n");
    }

}
void DesmontarParticion(char unidad,int id)
{
    int i=0;
    for(letra='a';letra<='z';letra++)
    {
        if(unidad==letra)
        {
            if(Mounts[i].parts[id-1].id!=0)
            {
                Mounts[i].parts[id-1].id==0;
                printf("Desmontando Particion en la unidad con Letra: %c Id: %d \n",unidad,id);
                return;
            }
            else{
                 printf("No esta montanda la partición con id: %d en la unidad: %c\n", id,unidad);
                return;
            }
        }
        i++;
    }
    printf("No se encontro  la partición con id: %d en la unidad: %c\n", id,unidad);
                return;
}
void ReportMBR(char *path, char *pathImagen)
{
    int i,j;
    CargarDisco(path);
    freopen("archivo.dot","w",stdout);
	printf("digraph G{\n");
	printf("node [shape=record];\n");
    printf("rankdir=LR;\n");
     printf(" struct1 [label=\" MBR | mbr_tamaño: %d| mbr_signature: %d\"];\n",contenedor.mbr_tamano,contenedor.mbr_disk_signature);
    for(i=0;i!=4;i++)
    {
        if(lista[i].part_size!=0)
        {
        printf(" struct%d [label=\"Particion %d |Nombre: %s|Tamaño: %d|Pos. Inicial: %d|Fit: %c|Status: %c|Tipo: %c\"];\n",i+2,i+1,lista[i].part_name,lista[i].part_size,lista[i].part_start,lista[i].part_fit,lista[i].part_status,lista[i].part_type);
        if(lista[i].part_type=='E')
        {
           CargarEBR(path,lista[i].part_start);
            j=6;
           while(EBR_Contenedor.part_size!=0)
           {
              printf(" struct%d [label=\"EBR%d |Nombre: %s|Tamaño: %d|Pos. Inicial: %d|Fit: %c|Status: %c|Siguiente: %d\"];\n",j,j-5,EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_start,EBR_Contenedor.part_fit,EBR_Contenedor.part_status,EBR_Contenedor.part_next);
           CargarEBR(path,EBR_Contenedor.part_next);
           j++;
           }
        }
       }
    }
    printf("}");
    fclose(stdout);
    char *instruccion;
    instruccion= strndup("dot -Tpng archivo.dot -o ", 25);
    strcat(instruccion,pathImagen);
    system(instruccion);
    char *comando;
    comando=strndup("xdg-open ",9 );
   strcat(comando,pathImagen);
	system(comando);
}
void ReporteDisco(char *path)
{
    freopen("archivo.dot","w",stdout);
	printf("digraph G{\n");
	printf("node [shape=record];\n");
	printf("struct1 [label=\"");
	CargarDisco(ruta);
	ordenarLista();
	printf("MBR  &#92;n tamaño: %d  &#92;n signature %d| ",contenedor.mbr_tamano,contenedor.mbr_disk_signature);

	int i=4-llenos();
	int cont=0;

    if(lista[i].part_start-sizeof(contenedor)>1)
    {
        printf("Espacio &#92;n Libre: &#92;n %d(Bytes)| ",lista[i].part_start-sizeof(contenedor));
    }
    if(lista[i].part_type=='E')
    {
         printf("{Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c |{ ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
         CargarEBR(ruta,lista[i].part_start);
         while(EBR_Contenedor.part_size!=0)
         {
             if(cont==0)
             {
              printf("Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);
             }
             else{ printf("|Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);}
          cont++;
          CargarEBR(ruta,EBR_Contenedor.part_next);
         }
         if(i!=3)
         {
              printf("}}|");
         }
         else if(contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start)>1)
                 {
                    printf("}}|");
                    printf("Espacio Libre: %d(Bytes)",contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start));
                 }
                 else{
                    printf("}}");
                 }
    }
    else{
            if(i!=3)
         {
             printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c| ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
         }
         else if(contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start)>1)
                 {
                    printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c| ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                    printf("Espacio Libre: %d(Bytes)",contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start));
                 }
                 else{
                   printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);;
                 }

    }
    for(i=i+1;i<4;i++)
    {
        if(i-1>=0)
        {
            if(lista[i].part_start-(lista[i-1].part_size+lista[i-1].part_start+1)!=0)
            {
                printf("Espacio Libre: %d(Bytes)|",lista[i].part_start-(lista[i-1].part_size+lista[i-1].part_start+1));
            }
        }
             if(i==3)
             {

                 if(contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start)>1)
                 {
                        if(lista[i].part_type=='E')
                        {
                            cont=0;
                        printf("{Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c |{ ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                        CargarEBR(ruta,lista[i].part_start);
                        while(EBR_Contenedor.part_size!=0)
                        {
                            if(cont==0)
                            {
                                printf("Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);
                            }
                            else{ printf("|Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);}
                            cont++;
                            CargarEBR(ruta,EBR_Contenedor.part_next);
                        }
                        printf("}}|");
                        } else{
                    printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c| ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                   } printf("Espacio Libre: %d(Bytes)",contenedor.mbr_tamano-(lista[i].part_size+lista[i].part_start));
                 }
                 else{
                     if(lista[i].part_type=='E')
                        {
                            cont=0;
                        printf("{Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c |{ ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                        CargarEBR(ruta,lista[i].part_start);
                        while(EBR_Contenedor.part_size!=0)
                        {
                            if(cont==0)
                            {
                                printf("Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);
                            }
                            else{ printf("|Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);}
                            cont++;
                            CargarEBR(ruta,EBR_Contenedor.part_next);
                        }
                        printf("}}");
                        } else{
                    printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                   }
                 }
             }
             else{
              if(lista[i].part_type=='E')
                        {
                            cont=0;
                        printf("{Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c |{ ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                        CargarEBR(ruta,lista[i].part_start);
                        while(EBR_Contenedor.part_size!=0)
                        {
                            if(cont==0)
                            {
                                printf("Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);
                            }
                            else{ printf("|Particion: %s &#92;n Size: %d &#92;n Next: %d &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c ",EBR_Contenedor.part_name,EBR_Contenedor.part_size,EBR_Contenedor.part_next,EBR_Contenedor.part_start,EBR_Contenedor.part_status,EBR_Contenedor.part_fit);}
                            cont++;
                            CargarEBR(ruta,EBR_Contenedor.part_next);
                        }
                        printf("}}|");
                        } else{
                    printf("Particion: %s &#92;n Size: %d &#92;n Tipo: %c &#92;n Start: %d &#92;n Status: %c &#92;n Fit: %c| ",lista[i].part_name,lista[i].part_size,lista[i].part_type,lista[i].part_start,lista[i].part_status,lista[i].part_fit);
                   }

        }
    }
    printf("\"];\n}");
    fclose(stdout);
    char *instruccion;
    instruccion= strndup("dot -Tpng archivo.dot -o ", 25);
    strcat(instruccion,path);
    system(instruccion);
    char *comando;
    comando=strndup("xdg-open ",9 );
    strcat(comando,path);
	system(comando);
}
void Reportes(char *comando)
{
char *token;
char *temp;
char *letras;
char *nombre;
char *pathImagen;
char *id;
char unit;
  int id_int,len,p=0,n=0;
    while ((token = strsep(&comando, " ")) != NULL)
  {
         if( strncmp(token, "-id::", 4)==0)
         {
             len=strlen(token);
              temp=strndup(token+7, len-6);
              letras=strndup(temp, 1);
              id=strndup(temp+1, strlen(temp)-1);
              id_int=strtol(id,(char**)NULL, 10);
              unit=letras[0];
              printf("Letra: %c\n", unit);
         }
         else if( strncasecmp(token, "-Path::", 6)==0)
         {
             if((p+n)==0)
             {len=strlen(token)-2;
              temp=strndup(token+8, len-7);
               printf("Path: %s\n", temp);
              p=1;
             }
             else{
                len=strlen(token)-2;
              temp=strndup(token+8, len-8);
              printf("Path: %s\n", temp);
              p=1;
             }
              pathImagen=temp;

         }
          else if( strncasecmp(token, "-Name::", 6)==0)
         {
            if((p+n)==0)
             {len=strlen(token)-2;
              temp=strndup(token+7, len-5);
              printf("Name: %s\n", temp);
              n=1;
             }
             else{
                len=strlen(token)-1;
              temp=strndup(token+7, len-7);
              printf("Name: %s\n", temp);
              n=1;
             }
              nombre=temp;
         }

         else {
            Lexico(token,comando);
         }
  }
  if(strncasecmp(nombre, "mbr", 2)==0)
  {
      BuscarRuta(unit);
    if(iguales==1)
    {
        printf("Path encontrado: %s\n", ruta);
        ReportMBR(ruta,pathImagen);
        return;
    }
    else{
         printf("No se encontraron datos de la unidad : %c\n", unit);
    }
  }
  else  if(strncasecmp(nombre, "disk", 3)==0)
  {
       BuscarRuta(unit);
    if(iguales==1)
    {
        printf("Path encontrado: %s\n", ruta);
        ReporteDisco(pathImagen);
        return;
    }
    else{
         printf("No se encontraron datos de la unidad : %c\n", unit);
    }
  }
}
void BuscarRuta(char unidad)
{
    iguales=0;
    int pos=0;
    for(letra='a';letra<='z';letra++)
    {
        if(letra==unidad)
        {
            iguales=1;
            ruta=Mounts[pos].path;
            return;
        }
        pos++;
    }
}
