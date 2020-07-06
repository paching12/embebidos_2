#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MUESTRAS 512

void generar_seno ( short int seno[] );

void guarda_datos (short int datos[]);


int main() {
	short int seno[MUESTRAS];

	generar_seno( seno );
	guarda_datos();

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

void guarda_datos( short int datos[] ) {
	FILE * ap_arch;
	register int n;

	ap_arch = fopen( "seno.dat", "w" );
	if( !ap_arch ){
		perror( "Error al abrir archivo" );
		exit(EXIT_FAILURE);

	}

	for( n = 0; n < MUESTRAS; n++ )
		fprintf(ap_arch, "%d \n", datos[n] );

	fclose( ap_arch );
} 

