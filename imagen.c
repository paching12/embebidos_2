/* Referencia:
	 https://poesiabinaria.net/2011/06/leyendo-archivos-de-imagen-en-formato-bmp-en-c/
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "imagen.h"

extern bmpInfoHeader info;
extern unsigned char *imageRGB, *imageGray, *blur;

unsigned char *abrirBMP(char *filename, bmpInfoHeader *bInfoHeader)
{

		FILE *f;
		bmpFileHeader header;     /* cabecera */
		unsigned char *imgdata;   /* datos de imagen */
	uint16_t type;        /* 2 bytes identificativos */

		f = fopen (filename, "r");
	if( f  == NULL )
	{
		perror("Error al abrir el archivo en lectura");
		exit(EXIT_FAILURE);
	}

		/* Leemos los dos primeros bytes */
		fread( &type, sizeof(uint16_t), 1, f );
		if( type != 0x4D42 )        /* Comprobamos el formato */
			{
		printf("Error en el formato de imagen, debe ser BMP de 24 bits");
					fclose(f);
					return NULL;
			}

		/* Leemos la cabecera de fichero completa */
		fread( &header, sizeof(bmpFileHeader), 1, f );

		/* Leemos la cabecera de información completa */
		fread( bInfoHeader, sizeof(bmpInfoHeader), 1, f );

		/* Reservamos memoria para la imagen, ¿cuánta?
			Tanto como indique imgsize */
		imgdata = (unsigned char *)malloc( bInfoHeader->imgsize );
	if( imgdata == NULL )
	{
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}
		/* Nos situamos en el sitio donde empiezan los datos de imagen,
		nos lo indica el offset de la cabecera de fichero*/
		fseek(f, header.offset, SEEK_SET);

		/* Leemos los datos de imagen, tantos bytes como imgsize */
		fread(imgdata, bInfoHeader->imgsize,1, f);

		/* Cerramos el apuntador del archivo de la imagen*/
		fclose(f);

		/* Devolvemos la imagen */
		return imgdata;
}

void guardarBMP( char *filename, bmpInfoHeader *info, unsigned char *imgdata )
{
	bmpFileHeader header;
		FILE *f;
		uint16_t type;

		f = fopen(filename, "w+");
	if( f  == NULL )
	{
		perror("Error al abrir el archivo en escritura");
		exit(EXIT_FAILURE);
	}

		header.size = info->imgsize + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);
		/* header.resv1=0; */
		/* header.resv2=1; */
		/* El offset será el tamaño de las dos cabeceras + 2 (información de fichero)*/
		header.offset = sizeof(bmpFileHeader) + sizeof(bmpInfoHeader) + 2;
		/* Escribimos la identificación del archivo */
		type = 0x4D42;
		fwrite( &type, sizeof(type), 1, f );
		/* Escribimos la cabecera de fichero */
		fwrite(&header, sizeof(bmpFileHeader),1,f);
		/* Escribimos la información básica de la imagen */
		fwrite(info, sizeof(bmpInfoHeader),1,f);
		/* Escribimos la imagen */
		fwrite(imgdata, info->imgsize, 1, f);
		fclose(f);
}

void displayInfo( bmpInfoHeader *info )
{
		printf("Tamaño de la cabecera: %u\n", info->headersize);
		printf("Anchura: %d\n", info->width);
		printf("Altura: %d\n", info->height);
		printf("Planos (1): %d\n", info->planes);
		printf("Bits por pixel: %d\n", info->bpp);
		printf("Compresión: %d\n", info->compress);
		printf("Tamaño de datos de imagen: %u\n", info->imgsize);
		printf("Resolucón horizontal: %u\n", info->bpmx);
		printf("Resolucón vertical: %u\n", info->bpmy);
		printf("Colores en paleta: %d\n", info->colors);
		printf("Colores importantes: %d\n", info->imxtcolors);
}


void RGBToGray( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	
	/*

		________________________________________
		|    |    |    |    |    |    |    |
		| R  | G  | B  | R  | G  | B  | R  | ...
		|____|____|____|____|____|____|____|____
			PIXEL 0    |    PIXEL 1   |    PIXEL 2
	
			_____________ _______________________
		___|_____________|______________|________
		|    |    |    |    |    |    |    |
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
	int grayIndex = 0;
	int nBytesImage = width * height * 3;
	for (i = 0; i < nBytesImage; i += 3, grayIndex++) {
		grayLevel = ( (imageRGB[i]*30) + (imageRGB[i+1]*59) + (imageRGB[i+2]*11))/100;
		imageGray[grayIndex] = grayLevel;
	} // end for
}
/*
*/

void gaussian_filter( unsigned char * xn, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	
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
		imageGray[ i ] = 0;
 
	int center   = 0;
	int sum      = 0;
	int subIndex = 0;

	for ( y = 0; y <= (height-dim); y++ ) {
		for ( x = 0; x <= (width-dim); x++ ) {
			sum    = 0;
			subIndex = 0;
			for ( yb = 0; yb < dim; yb++ ) {
				for ( xb = 0; xb < dim; xb++ ) {
					indice   =  ((y+yb) * width) + (x+xb);
					sum     +=  hn[ subIndex++ ] * xn[ indice ];
				} // end for
			} // end for

			
			center   =  dim >> 1;
			center   =  (y+center) * width + (x+center); 
			sum /= totalHn;
			imageGray[ center ] = sum;

		} // end for

	} // end for
} // end filtro

void * gaussian_parallel( void *nh ) {

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

	for( register int i  = 0; i < height*width; i++ ) {
		blur[ i ] = 0;
	}
 
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


void gradient_filter( unsigned char * xn, unsigned char * imageGray, uint32_t width, uint32_t height ) {
	
	register int x, y, xb, yb;
	int dim = 3, indice;


	int mat1[9] = { 1, 0, -1,
									2, 0, -2,
									1, 0, -1};

 	
	int mat2[9] = { -1, -2, -1,
									 0,  0,  0,
									 1,  2,  1};

	for( register int i  = 0; i < height*width; i++ )
		imageGray[ i ] = 0;
 
	int center   = 0;
	int sum1 		 = 0; 
	int sum2     = 0;
	int subIndex1 = 0;
	int subIndex2 = 0;

	for ( y = 0; y <= (height-dim); y++ ) {
		for ( x = 0; x <= (width-dim); x++ ) {
			sum1     = 0;
			sum2     = 0;
			subIndex1 = 0;
			subIndex2 = 0;
			for ( yb = 0; yb < dim; yb++ ) {
				for ( xb = 0; xb < dim; xb++ ) {
					indice   =  ((y+yb) * width) + (x+xb);
					sum1    +=  mat1[ subIndex1++ ] * xn[ indice ];
					sum2    +=  mat2[ subIndex2++ ] * xn[ indice ];
				} // end for
			} // end for

			
			center              = dim >> 1;
			center              = (y+center) * width + (x+center);
			sum1               /= 4;
			sum2               /= 4;
			imageGray[ center ] = sqrt( (sum1*sum1)+(sum2*sum2) );

		} // end for
	} // end for

} // end gradient filter

int * kernelGauss( int * factor, int dim, float desv ) {
	int * kernelGauss;
	float coef, norm = 0.0;
	int index = 0;
	kernelGauss = (int *) malloc( dim*dim*sizeof(int) );
	if( kernelGauss == NULL ) {
		perror("Error al asignar memoria al kernel Gaussiano");
		exit( EXIT_FAILURE );
	} // end if
	*factor =  0;
	int rango = dim >> 1;
	/*
		Caso 3 :
		 0011 -> 0001
		Caso 5:
		 0101 -> 0010
		Caso 7:
		 0111 -> 0011
	*/
	for( int y = 0; y < dim; y++ )
		for( int x = 0; x < dim; x++ ) {
			coef = expf( -( (x-rango)*(x-rango) + (y-rango)*(y-rango) )/(2.0*desv*desv) );
			if( !index )
				norm = coef;
			kernelGauss[ index ] = ( unsigned char )(coef/norm);
			// printf( "Coeficiente %d\n", kernelGauss[ index ]  );
			*factor += kernelGauss[ index++ ];  
		} // end for
		
	return kernelGauss;
} // end kernelGauss
 
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


unsigned char * reserveMemory( uint32_t width, uint32_t height ) {
	unsigned char *imagen;
	imagen = (unsigned char *)malloc( width*height*sizeof(char) );

	if( imagen == NULL ) {
		perror( "Error al asignar memoria" );
		exit( EXIT_FAILURE );
	}

	return imagen;
} // end reserveMemory


void print_mat( int * numbers, int limit, int every ) {
	for ( register int i = 0; i < limit; i++) {
		printf( "%3d ", numbers[i] );
		if( (i+1) % every == 0 )
			printf(" \n" );
		
	} // end for
} // end print_mat

