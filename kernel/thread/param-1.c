#include <linux/module.h>
#include <linux/moduleparam.h> /*Necesaria para el manejo de parámetros*/
#include <linux/kernel.h>
#include <linux/init.h>

#define DESC "Módulo de kernel simple y muchas cosas..."
#define AUTOR "José Pach ¡AJUA!"
#define LIC "GPL"
#define MAX 4

MODULE_LICENSE(AUTOR);
MODULE_LICENSE(LIC);
MODULE_LICENSE(DESC);
MODULE_INFO(driver, "PULSO CARDIACO");
MODULE_DESCRIPTION("Uso de parámetros");


static int irq = 7;
static char *comando = "derecha";
// static int datos[MAX] = {1, 2, 3, 4};
// static int numele = MAX;

// Para ver este valor en espacio de usuario - ls /sys/module/param_1/params
// inicializar vars  sudo insmod param-1.ko irq=10



// module_param( irq, int, 0660 );
// module_param( comando, charp, 0660 );
// module_param_array( datos, int, &numele, 0660 ); 


// MODULE_PARM_DESC( irq, "Número de interrupción (int)" );
// MODULE_PARM_DESC( comando, "Comando del módulo" );
// MODULE_PARM_DESC( datos, "Muestras de pulso cárdiaco" );

int get_param( char * val, const struct kernel_param * ops ) {

	printk( KERN_INFO "Reading Callback function\n" );
	return 0;
}


int set_param( const char * val, const struct kernel_param * kp ) {

	int parametro = *(int *)kp->arg;
	int ret;


	printk( KERN_INFO "Writing Callback function\n" );
	printk( KERN_INFO "Parámetro val: %s\n", val);
	printk( KERN_INFO "Parámetro kp->arg: %d\n", parametro);

	// Con el mágico uso de helpers en moduleparam.h (Convertí de cadena a entero)
	ret = param_set_int( val, kp );

	if( !ret )
		printk( KERN_INFO "Parámetro irq: %d \n", irq);


	return ret;
}

const struct kernel_param_ops mis_param_ops = 
{
	.set = set_param,
	.get = get_param,
};

module_param_cb( irq, &mis_param_ops, &irq, 0660 );

static int __init function_start( void ) {

	printk( KERN_INFO "Iniciando el módulo con prámetros 1\n" );
	printk( KERN_INFO "Interrupción: %d, Comando: %s\n", irq, comando );
	

	return 0;
}


static void __exit function_exit( void ) {
	
	printk( KERN_INFO "Terminando la ejecución con parámetros con funciones callback\n" );
	printk( KERN_INFO "Interrupción: %d, Comando: %s\n", irq, comando );
}


module_init( function_start );
module_exit( function_exit );
