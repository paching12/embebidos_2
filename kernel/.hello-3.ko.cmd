cmd_/home/nimda/Desktop/kernel/hello-3.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 --build-id  -T ./scripts/module-common.lds -o /home/nimda/Desktop/kernel/hello-3.ko /home/nimda/Desktop/kernel/hello-3.o /home/nimda/Desktop/kernel/hello-3.mod.o;  true