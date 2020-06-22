#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MUESTRAS 1024

void generar_seno ( short int seno[] );

void guarda_datos (short int datos[], char * archivo);


int main() {
	
	int fd, len;
	short int seno[MUESTRAS], seno_proc[MUESTRAS];


 	fd = open( "/dev/ESCUM_device", O_RDWR );
	if( fd == -1 )
	{
		perror("Error al abrir el DDC \n");
		exit( EXIT_FAILURE );
  	}

	generar_seno( seno );
	guarda_datos( seno, "seno.dat" );

	len = write( fd, seno, sizeof(short int) * MUESTRAS );
	printf("Muestras enviadas: %d \n", len);

	sleep(1);

	len = read( fd, seno_proc, sizeof(short int) * MUESTRAS );
	printf("Muestras recibidas: %d \n", len);

	guarda_datos( seno, "seno_proc.dat" );

	close( fd );

	return 0;
}


void generar_seno( short int seno[] ) {
	float f = 1000, fs = 45000, muestra;
	register int n;
	for( n = 0; n < MUESTRAS; n++ ){
		muestra    = sinf(2*n*M_PI*f/fs);
		seno[n]    = (short int)(muestra*2048);
	}

}

void guarda_datos( short int datos[], char * archivo ) {
	FILE * ap_arch;
	register int n;

	ap_arch = fopen( archivo, "w" );
	if( !ap_arch ){
		perror( "Error al abrir archivo" );
		exit(EXIT_FAILURE);
	}

	for( n = 0; n < MUESTRAS; n++ )
		fprintf(ap_arch, "%d \n", datos[n] );

	fclose( ap_arch );
} 

