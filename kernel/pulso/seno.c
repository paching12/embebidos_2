#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MUESTRAS 4096

void generar_seno ( short int seno[] );

void guarda_datos_float (float datos[], char * archivo);

void guarda_datos_int( short int datos[], char * archivo );

void autocorrelacion( short int rxx[] );
void hamming( void  );

short int rxx[MUESTRAS];

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
	guarda_datos_int( seno, "seno.dat" );

	len = write( fd, seno, sizeof(short int) * MUESTRAS );
	printf("Muestras enviadas: %d \n", len);

	sleep(1);

	len = read( fd, seno_proc, sizeof(short int) * MUESTRAS );
	printf("Muestras recibidas: %d \n", len);

	guarda_datos_int( seno_proc, "seno_proc.dat" );

	// hamming();

	// autocorrelacion( seno );
	
	// guarda_datos_int( rxx, "auto.dat" );

	close( fd );

	return 0;
}

/*Autocorrelación: Lunes 29 de Junio*/


void generar_seno( short int seno[] ) {
	float f = 1.3, fs = 512, muestra;
	register int n;
	for( n = 0; n < MUESTRAS; n++ ) { 
		muestra = sinf(2*n*M_PI*f/fs);
		seno[n] = (short int)(muestra*2048);
	} // end for

}

void guarda_datos_float( float datos[], char * archivo ) {
	FILE * ap_arch;
	register int n;

	ap_arch = fopen( archivo, "w" );
	if( !ap_arch ){
		perror( "Error al abrir archivo" );
		exit(EXIT_FAILURE);
	}

	for( n = 0; n < MUESTRAS; n++ )
		fprintf(ap_arch, "%f \n", datos[n] );

	fclose( ap_arch );
} 

void guarda_datos_int( short int datos[], char * archivo ) {
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

void autocorrelacion( short int x[] ) {

	register int n, l = 0;

	for( l = 0; l < MUESTRAS-1; l++) {
		rxx[ l ] = 0;
		for( n = l; n < MUESTRAS-1; n++ )
			rxx[ l ] += (x[ n ] >> 10) * (x[ n - l ] >> 10);
	}
	return;
} // autocorrelacion

void hamming( void ) {
	register int n  = 0;
	const float a = 0.53836, b = 0.46164;

	for( n = 0; n < MUESTRAS-1; n++ ) 
		rxx[n] = (a-b) * ( cosf( (2*M_PI*n)/MUESTRAS-1 ) );

	return;
}

