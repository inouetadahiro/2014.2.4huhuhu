
.SUFFIXES : .x .o .c .s

CC = arm-linux-gcc

CFLAGS = -O0 -I$(INC) -I$(INCSYS) -I$(INCUSR)

OBJDIR = ../../../obj
LIBDIR = ../../../lib

ROOT = /usr/local/arm_linux_4.2
LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
LIB1 =$(ROOT)/arm-linux/lib
LIBUSR = ../../../lib
INCSYS:=$(ROOT)/arm-linux/sys-include
INC :=$(ROOT)/arm-linux/include
INCUSR = ../../inc

OBJECT = gpio.o
TARGET = libgpio.so

SRCS := gpio.c

LIBS= -lc -lgcc -lc

all: $(LIBDIR)/$(TARGET)

$(OBJECT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c -o $@

$(LIBDIR)/$(TARGET): $(OBJECT)
	$(CC) -shared -Wl,-soname,libipc.so -o $@

clean:
	rm -f *.o 
	rm -f *.x 
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)	
	rm -f *.gdb

	rm -f $(OBJS)
	rm -f $(LIBDIR)/$(TARGET)
