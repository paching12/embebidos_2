/*
*  ddc6.c Algoritmo de autocorrelación, convolución entre una señal senoidal y una ventana de hamming, utilizando un archivo de dispositivo. 
*
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>		 //Registro de numeros
#include <linux/kdev_t.h>	 //Macros de min/maj
#include <linux/cdev.h>
#include <linux/slab.h>      // Reservar memoria dinamica kmalloc
#include <linux/uaccess.h>   // espacio de usuario
#include <linux/kthread.h>   // Para hilos en espacio de kernel
#include <linux/sched.h>     // Para la planificación de hilos
#include <linux/delay.h>     // Para las funciones de planificación sleep, usleep, msleep, ssleep
#define MAX_SIZE 4096
#define THREADS  4

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


/* Autocorrelación, ventana de hamming, seno */
static short int * buffer; // seno
static short int * rxx;
static short int * ventana;
static short int counter = 0;

/* Hilos */
struct task_struct * khilo[THREADS];

/* Funciones dispositivo de carácter */
static int     driver_open     ( struct inode *inode, struct file *file );
static int     driver_release  ( struct inode *inode, struct file *file );
static ssize_t driver_read     ( struct file *filp, char __user * buf, size_t len, loff_t * off);
static ssize_t driver_write    ( struct file *filp, const char * buf, size_t len, loff_t * off);

/* Convolución seno y ventana de hamming */
static int procesamiento(void);

/* Hilos */
int hilo_kernel( void* data ) {

    int id = *(int *)data;
    register int contador = 0, n;
    int proc = 0, cpu;
    
    while( !kthread_should_stop() ) {
        cpu = get_cpu();
        put_cpu();
        schedule();
		printk( KERN_INFO "Ejecutando codigo hilo %d, cpu %d\n", contador++, cpu );

		// Convolución entre una señal seinodal y ventana de hamming	
		for( n = id; n < MAX_SIZE; n+=THREADS ) {
			proc = buffer[ n ] * ventana[ n ];
		//       Q0      * Q15 = Q15
		//       (16 bits) * (16 bits) = 32 bits
			rxx[ n ] = (short int)(proc >> 15);
		}

    }

    return id;
}

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
	
	buffer  = kmalloc( sizeof(short int)*MAX_SIZE, GFP_KERNEL );
	rxx     = kmalloc( sizeof(short int)*MAX_SIZE, GFP_KERNEL );
	ventana = kmalloc( sizeof(short int)*MAX_SIZE, GFP_KERNEL );
	
	if( buffer == NULL ) {
		printk( KERN_ERR " Error al asignar buffer kmalloc \n" );
		return -ENOMEM;
	}

	if( rxx == NULL ) {
		printk( KERN_ERR " Error al asignar rxx kmalloc \n" );
		return -ENOMEM;
	}

	if( ventana == NULL ) {
		printk( KERN_ERR " Error al asignar rxx kmalloc \n" );
		return -ENOMEM;
	}

	return 0;
}


static int driver_release( struct inode *inode, struct file *file ) {
	printk( KERN_INFO " Llamada a la operacion release del DOC \n" );
	kfree( buffer );
	kfree( rxx );
	kfree( ventana );

	return 0;
}

static ssize_t driver_read( struct file *filp, char __user * buf, size_t len, loff_t * off) {
	int ret;
	printk( KERN_INFO " Llamada a la operacion read del DOC \n" );
	
	if( *off == 0 && len > 0 ) {
		
		ret = copy_to_user( buf, rxx, len );

		
		if( ret ) {
			return -EFAULT;
		}

		(*off) += len;

		return len;	
	}

	return 0;
}

static ssize_t driver_write( struct file *filp, const char * buf, size_t len, loff_t * off) {
	int ret;
	printk(KERN_INFO "LLamada a la funcion write del driver!!\n");
	

	if( counter % 2 == 0 ){
		ret = copy_from_user( buffer, buf, len );
		printk( KERN_INFO "Muestra  1\n" );
	}
	else{
		ret = copy_from_user( ventana, buf, len );
		printk( KERN_INFO "Muestra  2 - Proc\n");
		procesamiento();
	}

	counter++;

	if( ret ) {
		return -EFAULT; // bad address
	}
	
	
	return len;
}

int procesamiento ( void ) { /*Convolución*/

	static int id_thread[THREADS];
	register int i, n, l;
	char str[80];

	for( i = 0; i < 80; i++ )
		str[ i ] = 0;

    printk( KERN_INFO "\n ----------HILOS----------\n" );


    // Iniciando Hilos
    for( i = 0; i < THREADS; i++ ) {

    	sprintf( str, " Superduper hilo %d\n", i );

    	khilo[ i ] = kthread_create( hilo_kernel, (void*)&id_thread[ i ], str );
    	
    	if( IS_ERR( khilo[ i ] ) ) {
    		printk("KERN_ERR Error en la creación del hilo %d...\n", i);
        	return PTR_ERR( khilo[ i ] );
    	}

    	wake_up_process( khilo[ i ] );

    } // end for

    // Parar hilos
    for( i = 0; i < THREADS; i++ ) { 
		int ret_hilo_exit = kthread_stop( khilo[ i ] );

		if( ret_hilo_exit == -EINTR )
			printk( KERN_INFO "Error en la terminación del hilo \n" );
		else
			printk( KERN_INFO "Hilo de kernel con id: %d detenido \n", ret_hilo_exit );

	} // end for

	// Autocorrelación discreta
	for( l = 0; l < MAX_SIZE-1; l++) {
            rxx[ l ] = 0;
            for( n = l; n < MAX_SIZE-1; n++ )
                    rxx[ l ] += ( buffer[ n ] >> 10 ) * ( buffer[ n - l ] >> 10 );
    }

    return 0;

}

// buffer = kMALLOC( MEM_SIZE, GFP_KERNEL )

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
static void __exit funcion_exit(void) {
	printk(KERN_INFO "-------------\n");
	printk(KERN_INFO "Terminando modulo\n");
	
	// Destrucción de las variables relacionadas con el device driver
	device_destroy(dev_class, dispositivo);
	class_destroy(dev_class);
	cdev_del( &dev_cdev );
	unregister_chrdev_region(dispositivo,1);

}

module_init(funcion_inicio);
module_exit(funcion_exit);

/*
	sudo sh -c "echo 20 /dev/ESCUM_device"
	sudo cat /dev/ESCUM_device

*/