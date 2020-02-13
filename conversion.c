#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "imagen.h"
// Cada pixel ocupa 4 bytes por RGB

int main() {

	bmpInfoHeader info;
	unsigned char *imageRGB, *imageGray, *blur;// = "hola.bmp"

	printf("Abriendo imagen...\n");
	imageRGB = abrirBMP( "dark_forest3.bmp", &info );
	displayInfo( &info );

	imageGray = reserveMemory( info.width, info.height );
	blur 	  = reserveMemory( info.width, info.height );

	// Grayscale conversion
 	RGBToGray2( imageRGB, imageGray, info.width, info.height );
	gaussian_filter( imageGray, blur, info.width, info.height );
	GrayToRGB2( imageRGB, blur, info.width, info.height );

	// Not used
	// imageBrightness(imageRGB, imageGray, info.width, info.height );
	// umbralGlobal(imageRGB, info.width, info.height );
	// umbralDinamico(imageGray, info.width, info.height );



	// guardarBMP( "huella3.bmp", &info, imageRGB );
	
	guardarBMP( "blur.bmp", &info, imageRGB );
	
	// guardarBMP( "color.bmp", &info, imageRGB );

	free( imageRGB );
	free( imageGray );
	free( blur );

	return 0;
} // end main