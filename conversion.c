#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "imagen.h"
// Cada pixel ocupa 4 bytes por RGB

bmpInfoHeader info;
unsigned char *imageRGB, *imageGray, *blur;
void * funcHilo( void *nh );

int main() {


	printf("Abriendo imagen...\n");
	imageRGB = abrirBMP( "huella1.bmp", &info );
	displayInfo( &info );

	
	pthread_t tids[NUM_THREADS];
	int nhs[NUM_THREADS];
	register int nh;
	int *hilo;



	imageGray = reserveMemory( info.width, info.height );
	blur 	  = reserveMemory( info.width, info.height );

	// Grayscale conversion
	RGBToGray2( imageRGB, imageGray, info.width, info.height );

	for(nh = 0; nh < NUM_THREADS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funcHilo, (void *) &nhs[nh]);
	}
	for(nh = 0; nh < NUM_THREADS; nh++){
		pthread_join(tids[nh], (void **)&hilo);
		printf("hilo %d ha terminado\n",*hilo);
	}
	
	GrayToRGB2( imageRGB, blur, info.width, info.height );

	guardarBMP( "blur.bmp", &info, imageRGB );
	

	free( imageRGB );
	free( imageGray );
	free( blur );

	return 0;
} // end main



void * funcHilo( void *nh ) {

	int width = info.width, height = info.height;

	int core      = *(int *) nh;
	int eleBloque = height / NUM_THREADS;
	int ini       = core * eleBloque;
	int fin       = eleBloque+ini;

	register int x, y, xb, yb;
	int dim = 9, indice;


	int * hn;
  	int factor;
  	float des = 1;
  	hn = kernelGauss( &factor, dim, des );

	int totalHn = 0;

	for( register int x = 0; x < dim*dim; x++ )
		totalHn += hn[x];

	for( register int i  = 0; i < height*width; i++ )
		blur[ i ] = 0;
 
  	int shift    = dim >> 1;
  	int sum      = 0;
  	int subIndex = 0;
  	int middle   = 0;

	for ( y = ini; y <= fin-dim; y++ ) {
		for ( x = 0; x <= (width-dim); x++ ) {
			sum    = 0;
			subIndex = 0;
			for ( yb = 0; yb < dim; yb++ ) {
				for ( xb = 0; xb < dim; xb++ ) {
					indice   =  ((y+yb) * width) + (x+xb);
					sum     +=  hn[ subIndex++ ] * imageGray[ indice ];
				} // end for
			} // end for

	  
		sum /= totalHn;
		middle   =  (y+shift) * width + (x+shift); 
		blur[ middle ] = sum;

	} // end for

  } // end for

	pthread_exit( nh );
} // end funcHilo
