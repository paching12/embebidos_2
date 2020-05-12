 

obj-m += thread.o
obj-m += hello-2.o

all:
	make -C /lib/modules/`uname -r`/build/ M=`pwd` modules
clean:
	make -C /lib/modules/*/build/ M=`pwd` clean


## Después de compilar 
## Correr sudo insmod hello-3.ko
## lsmod | grep hello3-3.ko
## Log - dmesg
## modinfo hello-3.ko 
## rmmod hello-3.ko
