#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "imagen.h"

#define PUERTO 5000
#define TAM_BUFFER 100
#define DIR_IP "10.100.71.187"


void receive( unsigned char * img, bmpInfoHeader * info, int fd );

int main(int argc, char **argv)
{


	int tamano_direccion, sockfd;
	struct sockaddr_in direccion_servidor;
	char leer_mensaje[TAM_BUFFER];
/*	
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
 *  en arquitecturas little-endian se deben revertir los bytes
 */	
	memset (&direccion_servidor, 0, sizeof (direccion_servidor));
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_port = htons(PUERTO);
/*	
 *	inet_pton - Convierte direcciones de texto IPv4 en forma binaria
 */	
	if( inet_pton(AF_INET, DIR_IP, &direccion_servidor.sin_addr) <= 0 )
	{
		perror("Ocurrio un error al momento de asignar la direccion IP");
		exit(1);
	}
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
 *	Inicia el establecimiento de una conexion mediante una apertura
 *	activa con el servidor
 *  connect - ES BLOQUEANTE
 */
	printf ("Estableciendo conexion ...\n");
	if( connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor) ) < 0) 
	{
		perror ("Ocurrio un problema al establecer la conexion");
		exit(1);
	}
/*
 *	Inicia la transferencia de datos entre cliente y servidor
 */
	printf("Se aceptó un servidor, atendiendolo \n");
	if( read ( sockfd, &info, sizeof( bmpInfoHeader ) ) < 0 ) {
		perror ("Ocurrio algun problema al recibir datos del cliente");
		exit(1);
	}

	displayInfo( &info );
	
	pthread_t tids[NUM_THREADS];
	int nhs[NUM_THREADS];
	register int nh;
	int *hilo;


	blur      = reserveMemory( info.width, info.height );
	imageGray = reserveMemory( info.width, info.height );
	imageRGB  = reserveMemory( info.width, info.height*3 );

	//for( register int i = 0; i < (info.width * info.height); i++ )
		//blur[i] = 255;

	receive( imageGray, &info, sockfd );

	// Parallel gaussian filter
	for(nh = 0; nh < NUM_THREADS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, gradient_parallel, (void *) &nhs[nh]);
	} // end for

	for(nh = 0; nh < NUM_THREADS; nh++){
		pthread_join(tids[nh], (void **)&hilo);
		printf("hilo %d ha terminado\n",*hilo);
	} // end for

	GrayToRGB2( imageRGB, blur, info.width, info.height );
	guardarBMP( "photo.bmp", &info, imageRGB );

	// printf ("Recibiendo contestacion del servidor ...\n");
	// if (read (sockfd, &leer_mensaje, TAM_BUFFER) < 0)
	// {	
	// 	perror ("Ocurrio algun problema al recibir datos del cliente");
	// 	exit(1);
	// }
	
	// printf ("El servidor envio el siguiente mensaje: \n%s\n", leer_mensaje);
	// printf ("Cerrando la aplicacion cliente\n");
/*
 *	Cierre de la conexion
 */
	close(sockfd);
	free(blur);
	free(imageGray);
	free(imageRGB);
	return 0;
}
	
void receive( unsigned char * img, bmpInfoHeader * info, int fd ) {

	int size         = info->width*info->height;
	int receiveBytes = 0;
	int leftBytes    = size;
	int chunk_size   = 1300;
	int totalBytes   = 0;


	while( (receiveBytes = read( fd, img, chunk_size )) > 0 ) {
		if(  receiveBytes < 0 ) {
			perror( "Ocurrio algun problema al recibir datos del cliente" );
			exit(EXIT_FAILURE);
      	} // end if

      	totalBytes   += receiveBytes;
      	printf("Bytes restantes: %d - recibidos:%d\n", leftBytes, receiveBytes);
      	leftBytes     = leftBytes - receiveBytes;

      	img           = img + receiveBytes;
	}
	printf("Bytes restantes: %d, Bytes procesados: %d\n", leftBytes, totalBytes );

} // end receive
