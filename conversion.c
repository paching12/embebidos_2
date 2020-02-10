#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "imagen.h"
// Cada pixel ocupa 4 bytes por RGB

void RGBToGray( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
void GrayToRGB( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
void GrayToRGB2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
void RGBToGray2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
unsigned char * reserveMemory( uint32_t with, uint32_t height );
void imageBrightness( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );

void umbralGlobal( unsigned char * imageGray, uint32_t width, uint32_t height );
void umbralDinamico( unsigned char * imageGray, uint32_t width, uint32_t height );

void print_mat( int * numbers, int limit, int every );

void newL( unsigned char * imageGray, unsigned char * blur, uint32_t width, uint32_t height );

int main() {

	bmpInfoHeader info;
	unsigned char *imageRGB, *imageGray, *blur;// = "hola.bmp"

	printf("Abriendo imagen...\n");
	imageRGB = abrirBMP( "huella2.bmp", &info );
	displayInfo( &info );

	imageGray = reserveMemory( info.width, info.height );
	blur 	  = reserveMemory( info.width, info.height );



	// Grayscale conversion
	RGBToGray2( imageRGB, imageGray, info.width, info.height );

	// imageBrightness(imageRGB, imageGray, info.width, info.height );
	// umbralGlobal(imageRGB, info.width, info.height );
	
	umbralDinamico(imageGray, info.width, info.height );
	GrayToRGB2( imageRGB, imageGray, info.width, info.height );

	newL( imageRGB, blur, info.width, info.height );


	guardarBMP( "huella3.bmp", &info, imageRGB );
	guardarBMP( "blur.bmp", &info, blur );
	
	// guardarBMP( "color.bmp", &info, imageRGB );

	free( imageRGB );
	free( imageGray );
	free( blur );

	return 0;
} // end main

void RGBToGray( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	
	/*

		________________________________________
		|    |    |    |    |    |	  |    |
		| R  | G  | B  | R  | G  | B  | R  | ...
		|____|____|____|____|____|____|____|____
		 	PIXEL 0    |    PIXEL 1   |    PIXEL 2
	
			_____________ _______________________
		___|_____________|______________|________
		|    |    |    |    |    |	  |    |
		| R  | G  | B  | R  | G  | B  | R  | ...
		|____|____|____|____|____|____|____|____
		 	PIXEL 0    |    PIXEL 1   |    PIXEL 2


		__________________Formulas:____________________

							R+G+B
		Nivel de Gris = _____________
							  3

		Indice =  y * width + x
		Vars:     i           j

	*/
	int indexGray, indexRGB;
	unsigned char nivelGris;
	for( register int i = 0; i < height; i++ )
		for( register int j = 0; j < width; j++ ) {
			indexGray = (i*width) + j;
			indexRGB  = indexGray*3;
			nivelGris = (imageRGB[indexRGB] + imageRGB[indexRGB+1] + imageRGB[indexRGB+2])/3;
			imageGray[indexGray]   = nivelGris;
			// imageGray[indexGray+1] = nivelGris;
			// imageGray[indexGray+2] = nivelGris;
			// imageGray[indexGray] = nivelGris;
		} // end for

	return ;
}

void GrayToRGB( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	register int x, y;
	int indexGray, indexRGB;

	for( y = 0; y < height; y++ )
		for( x = 0; x < width; x++ ) {
			indexGray            = ( y * width  + x );
			indexRGB             = indexGray*3;
			imageRGB[indexRGB]   = imageGray[indexGray];
			imageRGB[indexRGB+1] = imageGray[indexGray];
			imageRGB[indexRGB+2] = imageGray[indexGray];
		}
} //End RGBToGray

void GrayToRGB2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	register int i;
	int nBytesImage = width * height*3;

	int indexGray;
	for( i = 0, indexGray = 0; i < nBytesImage; i += 3, indexGray++ ) {
			imageRGB[i]   = imageGray[indexGray];
			imageRGB[i+1] = imageGray[indexGray];
			imageRGB[i+2] = imageGray[indexGray];
		}
} //End RGBToGray


void RGBToGray2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {

	/*Ecuación de ajuste*/
	register int i;
	unsigned char grayLevel;
	int grayIndex;
	int nBytesImage = width * height * 3;
	for (grayIndex = 0,i = 0; i < nBytesImage; i += 3, grayIndex++) {
		grayLevel = ( (imageRGB[i]*30) + (imageRGB[i+1]*59) + (imageRGB[i+2]*11))/100;
		imageGray[grayIndex] = grayLevel;
	} // end for
}


unsigned char * reserveMemory( uint32_t width, uint32_t height ) {
	unsigned char *imagen;
	imagen = (unsigned char *)malloc( width*height*sizeof(char) );

	if( imagen == NULL ) {
		perror( "Error al asignar memoria" );
		exit( EXIT_FAILURE );
	}

	return imagen;
} // end reserveMemory


void imageBrightness( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	register int i, nBytesImage;
	nBytesImage = width * height;
	short int pixel;

	for( i = 0; i < nBytesImage; i++ ) {
		pixel = imageGray[ i ] + 70; 
		imageGray[ i ] = (pixel > 255) ? 255 : pixel;
	} // end for
}


void umbralGlobal( unsigned char * imageGray, uint32_t width, uint32_t height ) {
	register int i, nBytesImage;
	unsigned char umbral = 70;
	nBytesImage = width* height;
	for ( i = 0; i < nBytesImage; i++ )
		imageGray[i] = (imageGray[i] > umbral) ? 255 : 0;

} // end umbralGlobal

void umbralDinamico( unsigned char * imageGray, uint32_t width, uint32_t height ) {
	register int x, y, xb, yb;
	int media = 70;
	int bloque = 15, indice;
	// nBytesImage = width* height;
	for ( y = 0; y < height; y+=bloque ) 
		for ( x = 0; x < width; x+=bloque ) {
			media = 0;
			for ( yb = 0; yb < bloque; yb++ ) 
				for ( xb = 0; xb < bloque; xb++ ) {
					indice = ((y+yb) * width) + (x+xb);
					media += imageGray[indice];
				}
			media = media / (bloque*bloque);

			// aplica umbral a la subregión de la imagen
			for ( yb = 0; yb < bloque; yb++ ) 
				for ( xb = 0; xb < bloque; xb++ ) {
					indice = ( y + yb ) * width + (x+xb);
					imageGray[indice] = (imageGray[indice] > media) ? 255 : 0;
				}
			
		}

} // end umbralGlobal



void newL( unsigned char * imageGray, unsigned char * xn, uint32_t width, uint32_t height ) {
	
	register int x, y, xb, yb;
	int bloque = 3, indice;


	int hn[] = {1, 3,  1,   3, 5,  3,  1,  3, 1};
	int totalHn = 0;

	for( register int x = 0; x < sizeof(hn)/sizeof(hn[0]); x++ )
		totalHn += hn[x];

	// int xn[] = {15, 30, 66, 101, 2, 71, 41, 20, 6, 7, 8, 10, 8, 25, 31, 47}; 
	// int width  = 4;
	// int height = 4; 
	int yn[height*width];
 


	for ( y = 0; y < (height/bloque)+1; y++ ) 
		for ( x = 0; x < (width/bloque)+1; x++ ) {
			// media = 0;
			int sum 	 = 0;
			int subIndex = 0;
			for ( yb = 0; yb < bloque; yb++ ) {
				for ( xb = 0; xb < bloque; xb++ ) {
					indice   =  ((y+yb) * width) + (x+xb);
					subIndex =  yb*bloque + xb;
					sum     +=  hn[ subIndex ] * xn[ indice ];
					// printf( "hn[ %d ] * xn[ %d ] = %d\n", subIndex, indice, sum );
					// printf( "%d  * %d = %d\n", hn[subIndex], xn[indice], sum );
				} // end for
			} // end for

			int xc = ((bloque/2)+1) + x;
			int yc = ((bloque/2)+1) + (y*width);
			int center = (yc + xc) + 1;
			
			sum /= totalHn;
			yn[ center ] = sum;

		} // end for

} // end umbralGlobal


void print_mat( int * numbers, int limit, int every ) {
	for ( register int i = 0; i < limit; i++) {
		if( i % every == 0 )
			printf("%d \n", numbers[i] );
	} // end for
} // end print_mat