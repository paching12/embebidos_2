/* @brief Este programa multiplica dos vectores Pi = Ai * Bi
 *  de forma secuencial
 */
#include "paralelismo.h"
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
 
#define N 32    //Bloque de datos
#define NUM_HILOS 2

int *A, *B, *P;
 
// void * funHilo(void *nh);
 
// int main()
// {
//     pthread_t tids[NUM_HILOS];
//     int nhs[NUM_HILOS];
//     register int nh;
//     int *hilo;
//     srand(time(NULL));
//     A = reservar_memoria();
//     llenar_arreglo( A );
// //  imprimir( A );
 
//     B = reservar_memoria();
//     llenar_arreglo( B );
// //  imprimir( B );
 
//     P = reservar_memoria();
//     for(nh = 0; nh < NUM_HILOS; nh++){
//         nhs[nh] = nh;
//         pthread_create(&tids[nh], NULL, funcHilo, (void *) &nhs[nh]);
//     }
//     for(nh = 0; nh < NUM_HILOS; nh++){
//         pthread_join(tids[nh], (void **)&hilo);
//         printf("hilo %d ha terminado\n",*hilo);
//     }
//     //producto( );
//     imprimir( P );
 
//     free( A );
//     free( B );
//     free( P );
 
//     return 0;
// }
 
void producto( void )
{
    register int i;
    for( i = 0; i < N; i++ )
        P[i] = A[i] * B[i];
}
 
void llenar_arreglo( int *datos )
{
    register int i;
 
    for( i = 0; i < N; i++ )
        datos[i] = rand() % 256;
}
 
int * reservar_memoria( void )
{
    int *mem;
 
    mem = (int *)malloc( sizeof(int) * N );
    if( !mem )
    {
        perror("Error de asignación de memoria");
        exit(EXIT_FAILURE);
    }
 
    return mem;
}
 
void imprimir( int *datos )
{
    register int i = 0;
 
    for( i = 0; i < N; i++ )
    {
        printf("%6d ", datos[i]);
        if( !((i+1)%16) )
            printf("\n");
    }
}
 
 
void guardaDatosArch( int s[], char *archivo )
{
    FILE *apFile;
    register int i;
 
    apFile = fopen(archivo,"w");
    if( apFile == NULL )
    {
        perror("Error al abrir el archivo");
        exit(1);
    }
 
    for( i = 0; i < N; i++ )
    {
        //if( !(i % 16) )
        //  fputs(" \n", apFile);
 
        fprintf(apFile, "%d \n", s[i]);
    }
    fclose(apFile);
}
 
// void * funHilo(void *nh){
//     int core = *(int *) nh;
//     int register prod;
//     for(register int i = core; i<N; i+=NUM_HILOS){
//         prod = A[i] * B[i];
//         P[i] = prod;
//         printf("Thread %d Res %d \n",core, prod);
//     }
 
//     pthread_exit(nh);
// }


// void * funcHilo( void *nh ) {
// 	// int core = *(int *) nh;
// 	// int eleBloque = N / NUM_HILOS;
// 	register int i;

// 	for( i = 0; i < N; i++ ) {
// 		P[ i ] = A[i] * B[i];
// 	} // end for
// 	pthread_exit( nh );
// } // end funcHilo
