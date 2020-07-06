 

obj-m += promedio_ordenamiento.o

all:
	make -C /lib/modules/*/build M=`pwd` modules
clean:
	make -C /lib/modules/*/build/ M=`pwd` clean


## Después de compilar 
## Correr sudo insmod hello-3.ko
## lsmod | grep hello3-3.ko
## Log - dmesg
## modinfo hello-3.ko 
## rmmod hello-3.ko
## Pasar argumentos
## sudo insmod param-1.ko irq=10 comando="atrás" datos=15,16,17,18
