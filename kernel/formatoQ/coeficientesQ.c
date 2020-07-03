#include <stdio.h>
#define Qn 15
#define M 3


int main() {
	register int i;
	float cf[M] = { 0.5, 9.53125, 4.140625 };
	int ce[M], y,  x[M] = { 23, 7, 11 };

	for( i = 0; i < M; i++ ) {
		ce[i] = ( int )( cf[i] * (1 << Qn) );
		printf("Formato Q %d, ce %d \n", i, ce[i]);
	}

	y = 0;


	for( i = 0; i < M; i++ )
		y += ce[i] * x[i];

	y = y >> Qn;

	printf( "El resultado es:  %d \n", y );


	return 0;
}
