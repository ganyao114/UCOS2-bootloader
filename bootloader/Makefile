BL1.bin: start.o mem_setup.o mmc_relocate.o
	arm-linux-ld -TBL1.lds -o BL1.elf $^
	arm-linux-objcopy -O binary BL1.elf BL1.bin
	arm-linux-objdump -D BL1.elf > BL1_elf.dis
	gcc mkv210_image.c -o mkmini210
	./mkmini210 BL1.bin bootloader.bin
	
%.o : %.S
	arm-linux-gcc -o $@ $< -c 

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis mkmini210 -f
