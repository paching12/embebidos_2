
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


static int __init function_start( void ) {
	printk( KERN_INFO "Hola mundo en el módulo 2\n" );
	return 0;
}


static void __exit function_exit( void ) {
	printk( KERN_INFO "Terminando la ejecución del módulo 2\n" );
}


module_init( function_start );
module_exit( function_exit );