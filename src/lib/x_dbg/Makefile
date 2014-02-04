
.SUFFIXES : .x .o .c .s

CC = arm-linux-gcc

CFLAGS = -O0 -fPIC -I$(INC) -I$(INCSYS) -I$(INCUSR)

OBJDIR = ../../../obj
LIBDIR = ../../../lib

ROOT = /usr/local/arm_linux_4.2
LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
LIB1 =$(ROOT)/arm-linux/lib
LIBUSR = ../../../lib
INCSYS:=$(ROOT)/arm-linux/sys-include
INC :=$(ROOT)/arm-linux/include
INCUSR = ../../inc

STRIP=arm-linux-strip

OBJS = x_dbg.o
TARGET = libx_dbg.so

SRCS = x_dbg.c

all: $(LIBDIR)/$(TARGET)

$(OBJDIR)/$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c -o $@

$(LIBDIR)/$(TARGET): $(OBJDIR)/$(OBJS)
	$(CC) -shared -Wl,-soname,libx_dbg.so $(OBJDIR)/$(OBJS) -o $@
	$(STRIP) -S $(LIBDIR)/$(TARGET) 

clean:
	rm -f $(OBJDIR)/$(OBJS)
	rm -f $(LIBDIR)/$(TARGET)
