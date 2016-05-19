EXECUTABLE   := UCOS210.elf
LIBS         := lib/libc.a
#INCLUDEDIR 	:= $(shell pwd)/include				#设置 头文件 路径
INCLUDEDIR 	:=  -I./include -I./ucosspv210 -I./					#设置 头文件 路径
SRC_DIR      := ./*.c ./*.S ./ucosspv210/*.c ./ucosspv210/*.S				#只要在这里添加文件夹 即可设置源文件
LINKERSCRIPT := map.lds
CROSSCOMPILE := arm-linux-

CFLAGS 		:= -Wall -O2 -fno-builtin
CPPFLAGS   	:= -nostdinc $(INCLUDEDIR)

CC      = $(CROSSCOMPILE)gcc
LD      = $(CROSSCOMPILE)ld
AR      = $(CROSSCOMPILE)ar
OBJCOPY = $(CROSSCOMPILE)objcopy
OBJDUMP = $(CROSSCOMPILE)objdump

#export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 	#这是为了 给 make lib/libc.a 外部输出的 这里不用

SOURCE := $(wildcard $(SRC_DIR))
#objs := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(nodir $(SOURCE)))) $(LIBS)
objs := $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SOURCE))) $(LIBS)

all:$(EXECUTABLE)
	${OBJCOPY} -O binary -S $(EXECUTABLE) $(patsubst %.elf,%.bin,$(EXECUTABLE))
	${OBJDUMP} -D UCOS210.elf >$(patsubst %.elf,%.dis,$(EXECUTABLE))

$(EXECUTABLE): $(objs)
	${LD} -T$(LINKERSCRIPT) -o $(EXECUTABLE) $^
	
#.PHONY : lib/libc.a
#lib/libc.a:
#	cd lib; make; cd ..
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
#	make  clean -C lib
	rm -rf *.bin *.elf *.dis *.o *~ 
