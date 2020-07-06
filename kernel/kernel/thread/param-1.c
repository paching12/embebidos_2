#include <linux/module.h>
#include <linux/moduleparam.h> /*Necesaria para el manejo de parámetros*/
#include <linux/kernel.h>
#include <linux/init.h>

#define DESC "Módulo de kernel simple y muchas cosas..."
#define AUTOR "José Pach ¡AJUA!"
#define LIC "GPL"
#define MAX 6

MODULE_LICENSE(AUTOR);
MODULE_LICENSE(LIC);
MODULE_LICENSE(DESC);
MODULE_INFO(driver, "PULSO CARDIACO");
MODULE_DESCRIPTION("Uso de parámetros");


static char comando[MAX];

// Para ver este valor en espacio de usuario - ls /sys/module/param_1/params
// inicializar vars  sudo insmod param-1.ko irq=10



// module_param( irq, int, 0660 );
// module_param( comando, charp, 0660 );
// module_param_array( datos, int, &numele, 0660 ); 


// MODULE_PARM_DESC( irq, "Número de interrupción (int)" );
// MODULE_PARM_DESC( comando, "Comando del módulo" );
// MODULE_PARM_DESC( datos, "Muestras de pulso cárdiaco" );

module_param_cb( comando, &mis_param_ops, comando, 0660 );
// MODULE_PARM_DESC( comando, "Cadena parámetro" );

int get_param( char * buffer, const struct kernel_param * ops ) {

	char * cadena = (char *)(ops->arg);

	int ret;

	printk( KERN_INFO "funcion call back de lectura ejecutada" );

	strcpy( buffer, cadena );
	ret = strlen( buffer );
	printk( KERN_INFO "Valor de retorno %s...\n", buffer );

	return ret;
}


int set_param( const char * val, const struct kernel_param * kp ) {

	char * parametro = (char *)kp->arg;
	int ret = 0;
	int lon = strlen(val);


	printk( KERN_INFO "Writing Callback function\n" );
	printk( KERN_INFO "Parámetro val: %s\n", val);
	printk( KERN_INFO "Parámetro kp->arg: %s, comando: %s\n", parametro, comando);

	// Con el mágico uso de helpers en moduleparam.h (Convertí de cadena a entero)
	if( lon > MAX ) {
		printk( KERN_ERR "Parámetro (%s) muy largo \n", val );
		return -ENOSPC;
	}else if( lon == 1 ) {
		printk( KERN_ERR "Falta Parámetro \n" );
		return -EINVAL;
	}
	// else if(  )

	strcpy( comando, val );
	return ret;
}

const struct kernel_param_ops mis_param_ops = 
{
	.set = set_param,
	.get = get_param,
};



static int __init function_start( void ) {

	strcpy( comando, "atras");
	printk( KERN_INFO "Iniciando el módulo con prámetros 1\n" );
	printk( KERN_INFO "Comando: %s\n", comando );
	

	return 0;
}


static void __exit function_exit( void ) {
	
	printk( KERN_INFO "Terminando la ejecución con parámetros con funciones callback\n" );
	printk( KERN_INFO "Comando: %s\n", comando );

}


module_init( function_start );
module_exit( function_exit );

/*
Los Código de error se encuentran en la librería 
<asm-generic/errno-base.h>
*/