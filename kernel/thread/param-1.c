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
static int datos[MAX] = {1, 2, 3, 4};
static int numele = MAX;

// Para ver este valor en espacio de usuario - ls /sys/module/param_1/params
// inicializar vars  sudo insmod param-1.ko irq=10



module_param( irq, int, 0660 );
module_param( comando, charp, 0660 );
module_param_array( datos, int, &numele, 0660 ); 

MODULE_PARM_DESC( irq, "Número de interrupción (int)" );
MODULE_PARM_DESC( comando, "Comando del módulo" );
MODULE_PARM_DESC( datos, "Muestras de pulso cárdiaco" );


static int __init function_start( void ) {
	register int i;

	printk( KERN_INFO "Iniciando el módulo con prámetros 1\n" );
	printk( KERN_INFO "Interrupción: %d, Comando: %s\n", irq, comando );
	for( i = 0; i < numele; i++ ) {
		printk(KERN_INFO "Datos[%d] = %d \n", i, datos[i]);
	}
	return 0;
}


static void __exit function_exit( void ) {
	
	register int i;
	printk( KERN_INFO "Terminando la ejecución con parámetros 1\n" );
	printk( KERN_INFO "Interrupción: %d, Comando: %s\n", irq, comando );
	for( i = 0; i < numele; i++ ) {
		printk(KERN_INFO "Datos[%d] = %d \n", i,  datos[i]);
	}
}


module_init( function_start );
module_exit( function_exit );
