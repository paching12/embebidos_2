#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>		//Registro de numeros
#include <linux/kdev_t.h>	//Macros de min/maj
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pach");
MODULE_DESCRIPTION("Device Driver example");
MODULE_VERSION("0.666");
MODULE_INFO(driver, "Pulso cardiaco");

dev_t dispositivo = 0;
static struct class *dev_class;
static struct device *dev_file;

static struct class * dev_class;
static struct device *dev_file;
static struct cdev dev_cdev;

static int     driver_open     ( struct inode *inode, struct file *file );
static int     driver_release  ( struct inode *inode, struct file *file );
static ssize_t driver_read     ( struct file *filp, char __user * buf, size_t len, loff_t * off);
static ssize_t driver_write    ( struct file *filp, const char * buf, size_t len, loff_t * off);

static struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.open    = driver_open,
	.read    = driver_read,
	.write   = driver_write,
	.release = driver_release,
};

int driver_open ( struct inode * inode, struct file * file ) {
	printk( KERN_INFO "Llamada a la operacion open del DOC\n" );
	return 0;
}


static int driver_release( struct inode *inode, struct file *file ) {
	printk( KERN_INFO " Llamada a la operacion release del DOC \n" );
	return 0;
}

static ssize_t driver_read( struct file *filp, char __user * buf, size_t len, loff_t * off) {
	printk( KERN_INFO " Llamada a la operacion read del DOC \n" );
	return 0;

}

static ssize_t driver_write( struct file *filp, const char * buf, size_t len, loff_t * off) {
	printk( KERN_INFO " Llamada a la operacion write del DOC \n" );
	return len;
}

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
	
	cdev_init( &dev_cdev, &fops );
	ret = cdev_add( &dev_cdev, dispositivo, 1 );

	if(ret < 0){
		unregister_chrdev_region(dispositivo, 1);
		printk(KERN_ERR "Error al registrar las operaciones (fops)\n");
		return ret;
	}
	
	printk(KERN_INFO "operaciones (fops) Correctamente\n");

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
	cdev_del( &dev_cdev );
	unregister_chrdev_region(dispositivo,1);
}

module_init(funcion_inicio);
module_exit(funcion_exit);
