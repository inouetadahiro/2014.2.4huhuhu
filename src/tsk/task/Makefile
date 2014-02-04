
.SUFFIXES : .x .o .c .s

CC = arm-linux-gcc

CFLAGS = -O0 -I$(INC) -I$(INCSYS) -I$(INCUSR)

OBJDIR = ../../../obj
BINDIR = ../../../bin

ROOT = /usr/local/arm_linux_4.2
LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
LIB1 =$(ROOT)/arm-linux/lib
LIBUSR = ../../../lib
INCSYS:=$(ROOT)/arm-linux/sys-include
INC :=$(ROOT)/arm-linux/include
INCUSR = ../../inc

WEC_LDFLAGS=-L$(LIB) -L$(LIB1) -L$(LIBUSR)
STRIP=arm-linux-strip

OBJS = task.o

SRCS = task.c

LIBS= -lc -lgcc -lipc -lx_dbg

all: $(OBJDIR)/$(OBJS)

$(OBJDIR)/$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c -o $@

clean:
	rm -f $(OBJDIR)/$(OBJS)
