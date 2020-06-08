/*
	ddc 2 Asignación dinámica de device driver character
*/

#include <linux/module.h>
#include <linux/moduleparam.h> /*Necesaria para el manejo de parámetros*/
#include <linux/kernel.h>
#include <linux/fs.h> /*Necesaria para las funciones de registro*/
#include <linux/kdev_t.h> /*Necesarias para las macros mkdev, minor, major*/

#define DESC "Módulo de kernel simple y muchas cosas..."
#define AUTOR "José Pach ¡AJUA!"
#define LIC "GPL"
#define MAX 100

MODULE_LICENSE(AUTOR);
MODULE_LICENSE(LIC);
MODULE_LICENSE(DESC);
MODULE_INFO(driver, "Character Device Driver");
MODULE_DESCRIPTION("Asignación de números en DDC");

dev_t dispositivo = 0;

static int __init function_start( void ) {

	int ret;

	printk( KERN_INFO "Iniciando el módulo para asignar números estáticos 1\n" );
	ret = alloc_chrdev_region( &dispositivo, 0, 1, "ESCOM_DDC" );

	if( ret < 0 ) {
		printk( KERN_ERR "Error al asginar el número mayor y menor del device driver" );
		return ret;
	}

	printk( KERN_ERR "Números asignados correctamente, Mayor: %d, Menor: %d \n", MAJOR(dispositivo), MINOR(dispositivo) );

	return 0;
}


static void __exit function_exit( void ) {
	printk( KERN_INFO "Terminando la asignación de números en DDC\n" );
	unregister_chrdev_region( dispositivo, 1 );
}


module_init( function_start );
module_exit( function_exit );

/*
Los Código de error se encuentran en la librería 
<asm-generic/errno-base.h>
*/