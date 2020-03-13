#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "imagen.h"

#define PUERTO 			5000	//Número de puerto asignado al servidor
#define COLA_CLIENTES 	5 		//Tamaño de la cola de espera para clientes
#define TAM_BUFFER 		100

void sendChunks( bmpInfoHeader * info, int fd );

int main(int argc, char **argv)
{


   printf( "PID: %d,", getpid() );
   
   //system("raspistill -n -t 500 -e bmp -w 1200 -h 900 -o foto.bmp");
   system("convert -depth 24 foto.bmp foto.bmp");
   
   sleep(1);
   
   printf("Abriendo imagen...\n");
   
   imageRGB = abrirBMP( argv[1], &info );
   displayInfo( &info );
   

   imageGray = reserveMemory( info.width, info.height );


   //printf("width: %d | height: %d\n",info.width, info.height );
   RGBToGray2( imageRGB, imageGray, info.width, info.height );


   int sockfd, cliente_sockfd;
   struct sockaddr_in direccion_servidor; //Estructura de la familia AF_INET, que almacena direccion
   char leer_mensaje[TAM_BUFFER];
/*	
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
 *  en arquitecturas little-endian se deben revertir los bytes
 *  INADDR_ANY - Se elige cualquier interfaz de entrada
 */	
   	memset (&direccion_servidor, 0, sizeof (direccion_servidor));	//se limpia la estructura con ceros
   	direccion_servidor.sin_family 		= AF_INET;
   	direccion_servidor.sin_port 		= htons(PUERTO);
   	direccion_servidor.sin_addr.s_addr 	= INADDR_ANY;

/*
 *	Creacion de las estructuras necesarias para el manejo de un socket
 *  SOCK_STREAM - Protocolo orientado a conexión
 */
   	printf("Creando Socket ....\n");
   	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
   	}
/*
 *  bind - Se utiliza para unir un socket con una dirección de red determinada
 */
   	printf("Configurando socket ...\n");
   	if( bind(sockfd, (struct sockaddr *) &direccion_servidor, sizeof(direccion_servidor)) < 0 )
	{
      printf("PID: %d\n", getpid());
		perror ("Ocurrio un problema al configurar el socket");
		exit(1);
   	}
/*
 *  listen - Marca al socket indicado por sockfd como un socket pasivo, esto es, como un socket
 *  que será usado para aceptar solicitudes de conexiones de entrada usando "accept"
 *  Habilita una cola asociada la socket para alojar peticiones de conector procedentes
 *  de los procesos clientes
 */
   	printf ("Estableciendo la aceptacion de clientes...\n");
	if( listen(sockfd, COLA_CLIENTES) < 0 )
	{
		perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
		exit(1);
   	}
/*
 *  accept - Aceptación de una conexión
 *  Selecciona un cliente de la cola de conexiones establecidas
 *  se crea un nuevo descriptor de socket para el manejo
 *  de la nueva conexion. Apartir de este punto se debe
 *  utilizar el nuevo descriptor de socket
 *  accept - ES BLOQUEANTE 
 */
   	printf ("En espera de peticiones de conexión ...\n");
   	if( (cliente_sockfd = accept(sockfd, NULL, NULL)) < 0 ) {
		perror("Ocurrio algun problema al atender a un cliente");
		exit(1);
   	}

/*
 *	Inicia la transferencia de datos entre cliente y servidor
 */

      printf ("Enviando mensaje al cliente ...\n");
      if( write( cliente_sockfd, &info, sizeof( bmpInfoHeader ) ) < 0 )
      {
         perror("Ocurrio un problema en el envio de un mensaje al cliente");
         exit(1);
      }

   sendChunks( &info, cliente_sockfd );      


/*
 *	Cierre de las conexiones
 */

   close (sockfd);
   close (cliente_sockfd);
   free( imageGray );
   free( imageRGB );
	return 0;
}



void sendChunks( bmpInfoHeader * info, int fd ) {
   int all_size             = (info->width * info->height);
   int actually_size     = 0;
   int chunk_size        = 1500; // Max MTU size 1500 
   int chunks            = all_size/chunk_size;
   int iterator          = 0;
   int writeBytes        = 0;

   unsigned char * chunk = ( unsigned char *) malloc( sizeof( unsigned char *) * chunk_size );

   if( chunk == NULL ) {
      perror("Asignación de memoria incorrecta");
      exit(EXIT_FAILURE);
   } // END IF


   for( register int i = 0; i < chunks; i++ ) {
      // imageRGB[ i ]
      int j = 0;
      for( register int l = 0; l < chunk_size; l++ )
         chunk[l] = 0;

      while( j < chunk_size && iterator < all_size ) {
         chunk[j]        = imageGray[iterator];
         iterator++;
         j++;
      } // end while
      writeBytes = write( fd, chunk, j); 
      if( writeBytes < 0 ) {
         perror( "Ocurrio algun problema al escribir datos al cliente" );
         exit(EXIT_FAILURE);
      } // end if


      actually_size += chunk_size;
      printf( "Iterador %d - Actually: %d - Enviados: %d - Total:%d\n", iterator, actually_size, writeBytes, all_size );
   } // end for



}

// void recibirImagen( int cfv, unsigned char * imageGray,  ) {
//    int bytesFaltantes = longitud, bytesRcv;
//    while( bytesFaltantes > 0 ) {
//       bytesRcv = read( sfd, imageGray, bytesFaltantes );
//       if(  bytesRcv < 0 ) {
//          perror( "Ocurrio algun problema al recibir datos del cliente" );
//          exit(EXIT_FAILURE);
//       } // end if
//       printf("Bytes recibidos: %d\n", bytesRcv );
//       bytesFaltantes = bytesFaltantes - bytesRcv;
//       imageGray      = imageGray + bytesRcv;
//    } // end while
// } // end recibirImagen 
