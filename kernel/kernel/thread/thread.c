#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
// #include <licmp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VICTOR H GARCIA O");
MODULE_DESCRIPTION("Módulo de kernel simple");
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static int irq = 7;
// MODULE_PARAM_DESC( irq, "Numero de interrupcion" );

struct task_struct * khilo, * khilo2;

int hilo_kernel( void* data ) {

    int id = *(int *)data;
    register int contador = 0, cpu;
    
    while( !kthread_should_stop() ) {
    	
        cpu = get_cpu();
        put_cpu();
        schedule();
		printk( KERN_INFO "Ejecutando codigo hilo %d, cpu %d\n", contador++, cpu );

    }

    return id;
}

static int __init function_inicio(void) {
    static int id_thread = 10, id_thread2 = 20;

    printk( KERN_INFO "\n --------------------\n" );
    khilo = kthread_create( hilo_kernel, (void*)&id_thread, "superHilo" );


    if( IS_ERR(khilo) ) {
        printk("KERN_ERR Error en la creación del hilo...\n");
        return PTR_ERR(khilo);
    }

    kthread_bind( khilo, 7 ); //vincular el proceso con un core específico

    
    
    wake_up_process( khilo );
    printk( KERN_INFO "Hilo de Kernel creado %s con PID %d", khilo->comm, khilo->pid );
    
    khilo2 = kthread_create( hilo_kernel, (void*)&id_thread2, "superHilo" );


    if( IS_ERR(khilo2) ) {
        printk("KERN_ERR Error en la creación del hilo...\n");
        return PTR_ERR(khilo2);
    }

    kthread_bind( khilo2, 6 ); //vincular el proceso con un core específico

    
    
    wake_up_process( khilo2 );
    printk( KERN_INFO "Hilo de Kernel creado %s con PID %d", khilo2->comm, khilo2->pid );
    

    return 0;
}


static void __exit function_exit(void) {
    int ret_hilo_kernel  = kthread_stop( khilo );
    int ret_hilo_kernel2 = kthread_stop( khilo2 );

    if( ret_hilo_kernel == -EINTR || ret_hilo_kernel2 == -EINTR )
        printk( KERN_INFO "Error en la terminación del hilo" );
    else
        printk( KERN_INFO "Hilo de kernel con id: %d detenido || %d\n", ret_hilo_kernel, ret_hilo_kernel2 );
}

module_init(function_inicio);
module_exit(function_exit);

