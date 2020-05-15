#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

#define DESC "Módulo de kernel simple y muchas cosas"
#define AUTOR "José Pach"
#define LIC "GPL"
#define MAX 7

// Línea para probar:
// sudo insmod promedio_ordenamiento.ko option=2 numbers=5,8,7,4,6,1,4

struct task_struct * khilo;

MODULE_AUTHOR(AUTOR);
MODULE_LICENSE(LIC);
MODULE_DESCRIPTION(DESC);
MODULE_INFO(driver, "Promedio y ordenamiento de números");

static int option 	 = 1;
static int numbers[] = { 70, 20, 30, 10, 40, 50, 60 };
static int numberElements = MAX;

module_param( option, int, 0660 );
module_param_array( numbers, int, &numberElements, 0660 );

MODULE_PARM_DESC( option, "Elegir el comportamiento deseado." );
MODULE_PARM_DESC( numbers, "Elementos para dicha funcionalidad elegida" );
MODULE_PARM_DESC( datos, "Muestras de pulso cárdiaco" );

int average_numbers( void * data ) {
	int id = *(int *)data;
	register int i = 0, average_value = 0;
	
	printk( KERN_INFO "Promedio\n" );

	while( !kthread_should_stop() ) 
		if( !average_value ) {
			for( i = 0; i < MAX; i++ ) {
				average_value += numbers[i];
			} 

			printk( KERN_INFO "El promedio del arreglo es: %d\n", average_value/numberElements );
			ssleep(1);
		}
	
	return id;
} 

static void printArray(void) {
	register int i;
	for( i = 0; i < numberElements; i++ )
		printk( KERN_INFO "%2d ", numbers[i] );

	printk(KERN_INFO "\n");
}

static void swap_numbers( int *ptrElement1, int * ptrElement2 ) {
	int store;

	store = *ptrElement1;
	*ptrElement1 = *ptrElement2;
	*ptrElement2 = store;
} 

int order( void * data ) {
	int id = *(int *)data;

	printk( KERN_INFO "Ordenamiento de números\n" );

	while( !kthread_should_stop() ) {

		register int i = 0, j = 0;

		// Burble sort
		if( !i )
			for( i = 1; i < numberElements; i++ )
				for( j = 0; j < numberElements - 1; j++ )
					if( numbers[ j ] > numbers[ j + 1 ] )
						swap_numbers( &numbers[ j ], &numbers[ j + 1 ] );

	}

	// Print
	printArray();

	return id;
}

static int (*f[2])(void *) = { average_numbers, order };

static int __init function_start(void) {
    static int id_thread = 10;

    printk( KERN_INFO "\n --------------------\n" );
    if( option > 0 && option < 3 )
    	khilo = kthread_run( (*f[option-1]), (void*)&id_thread, "kmi_hilo" );


    if( IS_ERR(khilo) ) {
        printk("KERN_ERR Error en la creación del hilo...\n");
        return PTR_ERR(khilo);
    }

    printk( KERN_INFO "Hilo de Kernel creado %s con PID %d", khilo->comm, khilo->pid );
    
    return 0;
}


static void __exit function_exit(void) {
    int ret_hilo_kernel = kthread_stop( khilo );

    if( ret_hilo_kernel == -EINTR )
        printk( KERN_INFO "Error en la terminación del hilo" );
    else
        printk( KERN_INFO "Hilo de kernel con id: %d detenido\n", ret_hilo_kernel );
}

module_init( function_start );
module_exit( function_exit );
