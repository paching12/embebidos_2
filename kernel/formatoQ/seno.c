#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MUESTRAS 4096

void generar_seno (short int seno[] );

void guarda_datos_float (float datos[], char * archivo);

void guarda_datos_int(short int datos[], char * archivo );

void hamming( short int []);

void procesamiento( short int * seno, short int * ventana, short int * seno_proc );

int main() {
	int fd, len;
	short int seno[MUESTRAS], seno_proc[MUESTRAS], ventana[MUESTRAS];

	fd = open( "/dev/ESCUM_device", O_RDWR );
	if( fd == -1 )
	{
		perror("Error al abrir el DDC \n");
		exit( EXIT_FAILURE );
  	}

	generar_seno( seno );
	guarda_datos_int( seno, "seno.dat" );
	
	hamming( ventana );
	guarda_datos_int( ventana, "ventana.dat" );

	len = write( fd, seno, sizeof(short int) * MUESTRAS );
	printf("Muestras enviadas: %d \n", len);

	sleep(1);
	
	len = write( fd, ventana, sizeof(short int) * MUESTRAS );
	printf("Muestras enviadas: %d \n", len);

	sleep(1);

	len = read( fd, seno_proc, sizeof(short int) * MUESTRAS );
	printf("Muestras recibidas: %d \n", len);

	// for( len = 0; len < MUESTRAS; len++ )
	// 	printf("%d \n", seno_proc[len] );

	// procesamiento( seno, ventana, seno_proc );
	guarda_datos_int( seno_proc, "convolucion.dat" );

	close( fd );

	return 0;
}

void procesamiento( short int * seno, short int * ventana, short int * seno_proc ) {
	register int n;
	int proc;

	for( n = 0; n < MUESTRAS; n++ ) {
		proc = seno[ n ] * ventana[ n ]; 
		//       Q0      * Q15 = Q15
		//       (16 bits) * (16 bits) = 32 bits
		seno_proc[n] = (short int)(proc >> 15);
	}
}

void generar_seno( short int seno[] ) {
	float f = 1.3, fs = 512, muestra;
	register int n;
	for( n = 0; n < MUESTRAS; n++ ) { 
		muestra = sinf(2*n*M_PI*f/fs);
		seno[n] = (int)(muestra*2048); // Q0 ADC 12 bits

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

// void autocorrelacion( short int x[] ) {

// 	register int n, l = 0;

// 	for( l = 0; l < MUESTRAS-1; l++) {
// 		rxx[ l ] = 0;
// 		for( n = l; n < MUESTRAS-1; n++ )
// 			rxx[ l ] += (x[ n ] >> 10) * (x[ n - l ] >> 10);
// 	}
// 	return;
// } // autocorrelacion

void hamming( short int hamming[] ) { // Ventana de hamming
	register int n  = 0;
	const float a = 0.53836, b = 0.46164;
	float v = 0.0;

	for( n = 0; n < MUESTRAS-1; n++ ) {
		v = a - ( b * ( cosf( (2*M_PI*n)/(MUESTRAS-1) ) ) );
		hamming[n] = (short int)(v*(1 << 15)); //Q15
	}

	return;
}

