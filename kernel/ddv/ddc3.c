#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>		//Registro de numeros
#include <linux/kdev_t.h>	//Macros de min/maj

MODULE_LICENSE("GPL");
MODULE_AUTHOR("El cacas");
MODULE_DESCRIPTION("El cacas de nuevo");
MODULE_VERSION("0.666");
MODULE_INFO(driver, "Pulso cardiaco");

dev_t dispositivo = 0;
static struct class *dev_class;
static struct device *dev_file;

static int __init funcion_inicio(void){
	int ret;
	printk(KERN_INFO "-------------\n");
	printk(KERN_INFO "Inicio\n");
	ret = alloc_chrdev_region(&dispositivo, 0, 1, "ESCUM");
	if(ret < 0){
		printk(KERN_INFO "Error register_chrdev_region\n");
		return ret;
	}
	printk(KERN_INFO "Ma: %d, Mi: %d\n", MAJOR(dispositivo), MINOR(dispositivo));
	dev_class = class_create(THIS_MODULE, "ESCUM_class");
	if( IS_ERR(dev_class) ){
		printk(KERN_ERR "Error al crear la clase de dispositivo\n");
		return PTR_ERR(dev_class);
	}

	printk(KERN_INFO "Clase de dispositivo OK\n");
	dev_file = device_create(dev_class, NULL, dispositivo, NULL, "ESCUM_device");

	if ( IS_ERR(dev_file) ){
		printk(KERN_ERR "ERror al crear archivo de dispositivo\n");
		unregister_chrdev_region(dispositivo, 1);
		class_destroy(dev_class);
		return PTR_ERR( dev_file );
	}

	printk(KERN_INFO "Todo OK\n");

	return 0;
}
static void __exit funcion_exit(void){
	printk(KERN_INFO "-------------\n");
	printk(KERN_INFO "Terminando modulo\n");
	device_destroy(dev_class, dispositivo);
	class_destroy(dev_class);
	unregister_chrdev_region(dispositivo,1);
}

module_init(funcion_inicio);
module_exit(funcion_exit);
