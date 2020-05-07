#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("VICTOR H GARCIA O");
MODULE_DESCRIPTION("Módulo de kernel simple");
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

struct task_struct * khilo;

int hilo_kernel( void* data ) {
    int id = *(int *)data;
    return id;
}

static int __init function_inicio(void) {
    static int id_thread = 10;

    printk( KERN_INFO "\n --------------------\n" );
    khilo = kthread_create( hilo_kernel, (void*)&id_thread, "kmi_hilo" );


    if( IS_ERR(khilo) ) {
        printk("KERN_ERR Error en la creación del hilo...\n");
        return PTR_ERR(khilo);
    }

    wake_up_process( khilo );
    printk( KERN_INFO "Hilo de Kernel creado %s con PID %d", khilo->comm, khilo->pid );
    
    return 0;
}


static void __exit function_exit(void) {
    int ret_hilo_kernel = kthread_stop( khilo );

    if( ret_hilo_kernel == -EINTR )
        printk( KERN_INFO "Error en la terminación del hilo" );
    else
        printk( KERN_INFO "Hilo de kernel con id: %d detenido\n", ret_hilo_kernel );
}

module_init(function_inicio);
module_exit(function_exit);

