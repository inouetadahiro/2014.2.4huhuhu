
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

OBJS = dbgx.o
TARGET = dbgx

SRCS = dbgx.c

LIBS= -lc -lgcc -lrt -lipc -lx_dbg
#LIBS= -lc -lgcc -lrt

all: $(BINDIR)/$(TARGET)

$(OBJDIR)/$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c -o $@

$(BINDIR)/$(TARGET): $(OBJDIR)/$(OBJS)
	$(CC) $(CFLAGS) $(WEC_LDFLAGS) $(LIBS) $(OBJDIR)/$(OBJS) -o $@
#	$(CC) $(CFLAGS) $(WEC_LDFLAGS) $(LIBS) $(OBJDIR)/$(OBJS) $(OBJDIR)/x_dbg.o $(OBJDIR)/ipc.o -o $@ -static
	$(STRIP) $(BINDIR)/$(TARGET) 

clean:
	rm -f $(OBJDIR)/$(OBJS)
	rm -f $(BINDIR)/$(TARGET)
