/** @brief: Este programa muestra el uso del divice driver de caracter
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define N  1024
char buffer[N], buffer_read[N];

int main()
{
	int fd;

 	fd = open( "/dev/ESCUM_device", O_RDWR );
	if( fd == -1 )
	{
		perror("Error al abrir el DDC \n");
		exit( EXIT_FAILURE );
  	}

	printf("Dame el mensaje:");
	scanf("%s", buffer);
	write( fd, buffer, strlen(buffer)+1 );

	sleep( 5 );

	read( fd, buffer_read, N );
	printf("Mensaje recibido el DDC: %s\n", buffer_read );

	sleep( 5 );

	close( fd );

	return 0;
}


