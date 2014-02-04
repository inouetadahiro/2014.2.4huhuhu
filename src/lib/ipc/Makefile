
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

WEC_LDFLAGS=-L$(LIB) -L$(LIB1) -L$(LIBUSR)
STRIP=arm-linux-strip

OBJS = ipc.o
TARGET = libipc.so

SRCS = ipc.c

LIBS= -lc -lgcc -lrt

all: $(LIBDIR)/$(TARGET)

$(OBJDIR)/$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c -o $@

$(LIBDIR)/$(TARGET): $(OBJDIR)/$(OBJS)
	$(CC) -shared -Wl,-soname,libipc.so $(OBJDIR)/$(OBJS) -o $@ $(WEC_LDFLAGS) $(LIBS)
	$(STRIP) -S $(LIBDIR)/$(TARGET) 

clean:
	rm -f $(OBJDIR)/$(OBJS)
	rm -f $(LIBDIR)/$(TARGET)
