#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DESC "Módulo de kernel simple y muchas cosas..."
#define AUTOR "José Pach ¡AJUA!"
#define LIC "GPL"

MODULE_LICENSE(AUTOR);
MODULE_LICENSE(LIC);
MODULE_LICENSE(DESC);
MODULE_INFO(driver, "PULSO CARDIACO");

static int __init function_start( void ) {
	printk( KERN_INFO "Hola mundo en el módulo 3\n" );
	return 0;
}


static void __exit function_exit( void ) {
	printk( KERN_INFO "Terminando la ejecución del módulo 3\n" );
}


module_init( function_start );
module_exit( function_exit );
