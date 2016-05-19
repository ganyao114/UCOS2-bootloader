all:
	make -C ./bootloader
	make -C ./UCOS2
clean:
	make clean -C ./bootloader
	make clean -C ./UCOS2
