#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "imagen.h"
// Cada pixel ocupa 4 bytes por RGB


int main() {


	printf("Abriendo imagen...\n");
	imageRGB = abrirBMP( "calle1.bmp", &info );
	displayInfo( &info );

	
	pthread_t tids[NUM_THREADS];
	int nhs[NUM_THREADS];
	register int nh;
	int *hilo;



	imageGray = reserveMemory( info.width, info.height );
	blur 	  = reserveMemory( info.width, info.height );

	// Grayscale conversion
	RGBToGray2( imageRGB, imageGray, info.width, info.height );

	// Parallel gaussian filter
	// for(nh = 0; nh < NUM_THREADS; nh++){
	// 	nhs[nh] = nh;
	// 	pthread_create(&tids[nh], NULL, gaussian_parallel, (void *) &nhs[nh]);
	// }
	// for(nh = 0; nh < NUM_THREADS; nh++){
	// 	pthread_join(tids[nh], (void **)&hilo);
	// 	printf("hilo %d ha terminado\n",*hilo);
	// }

	gradient_filter( imageGray, blur, info.width, info.height );
	
	GrayToRGB2( imageRGB, blur, info.width, info.height );

	guardarBMP( "blur.bmp", &info, imageRGB );
	

	free( imageRGB );
	free( imageGray );
	free( blur );

	return 0;
} // end main

