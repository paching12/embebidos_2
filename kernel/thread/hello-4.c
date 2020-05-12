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

static int mivar __initdata = 10;
static int mivar2 __initdata = 20;

static int __init function_start( void ) {
	printk( KERN_INFO "Hola mundo en el módulo 4, var1: %d, var2: %d \n", mivar, mivar2 );
	return 0;
}


static void __exit function_exit( void ) {
	printk( KERN_INFO "Terminando la ejecución del módulo 4\n" );
}


module_init( function_start );
module_exit( function_exit );
