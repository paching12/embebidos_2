#include <linux/module.h>
#include <linux/moduleparam.h> /*Necesaria para el manejo de parámetros*/
#include <linux/kernel.h>
#include <linux/init.h>

#define DESC "Módulo de kernel simple y muchas cosas..."
#define AUTOR "José Pach ¡AJÚA!"
#define LIC "GPL"
#define MAX 8

MODULE_LICENSE(AUTOR);
MODULE_LICENSE(LIC);
MODULE_LICENSE(DESC);
MODULE_INFO(driver, "PULSO CARDIACO");
MODULE_DESCRIPTION("Uso de CallBack");


static int option 	   = 1;
static int average_val = 0;
static int numbers[] = { 70, 20, 30, 10, 40, 50, 60, 80 };
static int numberElements = MAX;

// static char comando[MAX];

// Para ver este valor en espacio de usuario - ls /sys/module/param_1/params
// inicializar vars  sudo insmod param-1.ko irq=10


module_param_array( numbers, int, &numberElements, 0660 );

MODULE_PARM_DESC( option, "Número de option (int)" );
MODULE_PARM_DESC( numbers, "Elementos para dicha funcionalidad elegida" );

static void average_numbers(void) {
	register int i = 0;
	average_val = 0;
	
	printk( KERN_INFO "Promedio\n" );

	for( i = 0; i < MAX; i++ )
		average_val += numbers[i];

	average_val = average_val/numberElements;
	// printk( KERN_INFO "El promedio del arreglo es: %d\n", average_val );
	
	return;
} 

static void printArray(void) {
	register int i;
	for( i = 0; i < numberElements; i++ )
		printk( KERN_INFO "%2d \n", numbers[i] );

	printk(KERN_INFO "\n");
}

static void swap_numbers( int *ptrElement1, int * ptrElement2 ) {
	int store;

	store = *ptrElement1;
	*ptrElement1 = *ptrElement2;
	*ptrElement2 = store;
} 

static void order(void) {

	printk( KERN_INFO "Ordenamiento de números\n" );

	register int i = 0, j = 0;
	
		// Burble sort
	for( i = 1; i < numberElements; i++ )
		for( j = 0; j < numberElements - 1; j++ )
			if( numbers[ j ] > numbers[ j + 1 ] )
				swap_numbers( &numbers[ j ], &numbers[ j + 1 ] );
	
	printArray();

	// Print
	return;
}

void (*f[2])(void) = { average_numbers, order };

int get_param( char * buffer, const struct kernel_param * kp ) {

	// char * cadena = (char *)(ops->arg);
	int ret;

	printk( KERN_INFO "funcion call back de lectura ejecutada" );
	printk( KERN_INFO "buffer antes de la conversión: %s, arg = %d...", buffer, *(int *)kp->arg);

	option = average_val;
	ret = param_get_int( buffer, kp );
	printk( KERN_INFO "Valor de retorno %d...\n", ret );

	if( !ret )
		return -EPERM;


	printk( KERN_INFO "buffer después de la conversión: %s...", buffer );

	return ret;
}

int set_param( const char * val, const struct kernel_param * kp ) {

	int parametro = *(int *)kp->arg;
	int ret = 0;
	int lon = strlen(val);


	printk( KERN_INFO "Writing Callback function \n" );
	printk( KERN_INFO "Param: %d\n", parametro );
	// printk( KERN_INFO "Parámetro val: %s\n", val);
	// printk( KERN_INFO "Parámetro kp->arg: %s\n", parametro);
	ret = param_set_int( val, kp );

	if( ret ) {
		printk( KERN_ERR "Error conversión de entero\n" );
		return -EINVAL;
	}

	if( option > 2 || option < 1 ) {
		printk( KERN_ERR "Opción no existente\n" );
		return -EINVAL;
	}

	(*f[option-1])();


	return ret;
}

const struct kernel_param_ops mis_param_ops = 
{
	.set = set_param,
	.get = get_param,
};

module_param_cb( option, &mis_param_ops, &option, 0660 );


static int __init function_start( void ) {

	// strcpy( comando, "atras");
	printk( KERN_INFO "Iniciando el módulo con parámetros 1\n" );
	// printk( KERN_INFO "Comando: %s\n", comando );
	
	return 0;
}


static void __exit function_exit( void ) {
	
	printk( KERN_INFO "Terminando la ejecución con parámetros con funciones callback\n" );
	// printk( KERN_INFO "Comando: %s\n", comando );

}


module_init( function_start );
module_exit( function_exit );

/*
Los Código de error se encuentran en la librería 
<asm-generic/errno-base.h>
*/