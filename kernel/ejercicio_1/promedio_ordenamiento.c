#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>

#define DESC "Módulo de kernel simple y muchas cosas"
#define AUTOR "José Pach"
#define LIC "GPL"
#define MAX 7

// Línea para probar:
// sudo insmod promedio_ordenamiento.ko option=2 numbers=5,8,7,4,6,1,4

MODULE_AUTHOR(AUTOR);
MODULE_LICENSE(LIC);
MODULE_DESCRIPTION(DESC);
MODULE_INFO(driver, "Promedio y ordenamiento de números");

static int option 	 = 2;
static int numbers[] = { 70, 20, 30, 10, 40, 50, 60 };
static int numberElements = MAX;

module_param( option, int, 0660 );
module_param_array( numbers, int, &numberElements, 0660 );

MODULE_PARM_DESC( option, "Elegir el comportamiento deseado." );
MODULE_PARM_DESC( numbers, "Elementos para dicha funcionalidad elegida" );
MODULE_PARM_DESC( datos, "Muestras de pulso cárdiaco" );

static void average_numbers( int * numbers ) {
	register int i;
	int average_value = 0;
	
	for( i = 0; i < numberElements; i++ ) 
		average_value += numbers[i];
	
	printk( KERN_INFO "El promedio del arreglo es: %d\n", average_value/numberElements );	
	return;
} 

static void printArray( int * number ) {
	register int i;
	for( i = 0; i < numberElements; i++ )
		printk( KERN_INFO "%2d ", number[i] );

	printk(KERN_INFO "\n");
}

static void swap_numbers( int *ptrElement1, int * ptrElement2 ) {
	int store;

	store = *ptrElement1;
	*ptrElement1 = *ptrElement2;
	*ptrElement2 = store;

} 

static void order( int * numbers ) {
	printk( KERN_INFO "Ordenamiento de números\n" );
	register int i = 0, j = 0;

	// Burble sort
	for( i = 1; i < numberElements; i++ )
		for( j = 0; j < numberElements - 1; j++ )
			if( numbers[ j ] > numbers[ j + 1 ] )
				swap_numbers( &numbers[ j ], &numbers[ j + 1 ] );

	// Print
	printArray( numbers );
	return;
}

static void (*f[2])(int *) = { average_numbers, order };

static int __init function_start( void ) {
	
	printk( KERN_INFO "1.-Promedio de los elementos del arreglo\n" );
	printk( KERN_INFO "2.-Ordenamiento del arrelgo usando y burbuja.\n" );
	printk( KERN_INFO "Opción: %d \n", option );
	
	if( option < 3 && option > 0 )
		(*f[ option-1 ])( numbers );

	return 0;
}


static void __exit function_exit( void ) {
	printk( KERN_INFO "Terminando la ejecución del programa...\n" );
}


module_init( function_start );
module_exit( function_exit );
