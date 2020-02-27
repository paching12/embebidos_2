#include <stdint.h>
#include <stdio.h>

#ifndef IMAGEN_H
#define IMAGEN_H
#define NUM_THREADS 4

typedef struct bmpFileHeader
{
  /* 2 bytes de identificación */
  uint32_t size;        /* Tamaño del archivo */
  uint16_t resv1;       /* Reservado */
  uint16_t resv2;       /* Reservado */
  uint32_t offset;      /* Offset hasta hasta los datos de imagen */
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  uint32_t headersize;      /* Tamaño de la cabecera */
  uint32_t width;       /* Ancho */
  uint32_t height;      /* Alto */
  uint16_t planes;          /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* bits por pixel */
  uint32_t compress;        /* compresión */
  uint32_t imgsize;     /* tamaño de los datos de imagen */
  uint32_t bpmx;        /* Resolución X en bits por metro */
  uint32_t bpmy;        /* Resolución Y en bits por metro */
  uint32_t colors;      /* colors used en la paleta */
  uint32_t imxtcolors;      /* Colores importantes. 0 si son todos */
} bmpInfoHeader;

bmpInfoHeader info; // width and height image
unsigned char *imageRGB, *imageGray, *blur;


unsigned char *abrirBMP(char *filename, bmpInfoHeader *bInfoHeader);
void guardarBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata);
void displayInfo(bmpInfoHeader *info);

/* Filtros */

void RGBToGray( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
void GrayToRGB( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );

// More efficient
void GrayToRGB2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );
void RGBToGray2( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );

// Brightness
void imageBrightness( unsigned char * imageRGB, unsigned char * imageGray, uint32_t width, uint32_t height );

// Gaussian's filter
void gaussian_filter( unsigned char * imageGray, unsigned char * blur, uint32_t width, uint32_t height );
void umbralGlobal( unsigned char * imageGray, uint32_t width, uint32_t height );
void umbralDinamico( unsigned char * imageGray, uint32_t width, uint32_t height );
void * gaussian_parallel( void * );

// Gradient's filter
void gradient_filter( unsigned char * imageGray, unsigned char * blur, uint32_t width, uint32_t height );
void * gradient_parallel( void * );
// Kernel Gauss
void kernel_gaussian();
int * kernelGauss( int * factor, int dim, float desv );

/* Helpers */
unsigned char * reserveMemory( uint32_t with, uint32_t height );
void print_mat( int * numbers, int limit, int every );



#endif
