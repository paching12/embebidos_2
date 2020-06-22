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
static struct class * dev_class;
static struct device *dev_file;
static struct cdev dev_cdev;

static int driver_open     ( struct inode *inode, struct file *file );
static int driver_release  ( struct inode *inode, struct file *file );
static ssize_t driver_read ( struct inode *filp, char __user * buf, size_t len, loff_t * off);
static ssize_t driver_write ( struct inode *filp, const char * buf, size_t len, loff_t * off);

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open  = driver_open,
	.read  = driver_read,
	.write = driver_write,
	.release = driver_release,
};

int driver_open ( struct inode * inode, struct file * file ) {
	printk( KERN_INFO, "Llamada a la operacion open del DOC\n" );
	return 0;
}


static int driver_release( struct inode *inode, struct file *file ) {
	printk( KERN_INFO " Llamada a la operacion release del DOC \n" );
	return 0;
}

static ssize_t driver_read( struct inode *filp, char __user * buf, size_t len, loff_t * off) {
	printk( KERN_INFO " Llamada a la operacion read del DOC \n" );
	return 0;

}

static ssize_t driver_write( struct inode *filp, const char * buf, size_t len, loff_t * off) {
	printk( KERN_INFO " Llamada a la operacion write del DOC \n" );
	return 0;
}

void cdev_init( struct cdev * cdev, const struct file_operations * fops ) {
	memset( cdev, 0, sizeof() );
}

int cdev_add( struct cdev * p, dev_t dev, unsigned count ) {
	int error;

}

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
