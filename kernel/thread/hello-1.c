
#include <linux/module.h>
#include <linux/kernel.h>


int init_module(void) {
    printk(KERN_ALERT "Hola mundo en el modulo 1.\n");
    return 0;
}


void cleandup_module( void ) {
    printk( KERN_ALERT "Terminando la ejecución del módulo 1.\n" );
}
