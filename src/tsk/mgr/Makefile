
.SUFFIXES : .x .o .c .s

CC = arm-linux-gcc

CFLAGS = -O0 -I$(INC) -I$(INCSYS) -I$(INCUSR) -I$(INCMGR)

OBJDIR = ../../../obj
BINDIR = ../../../bin

ROOT = /usr/local/arm_linux_4.2
LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
LIB1 =$(ROOT)/arm-linux/lib
LIBUSR = ../../../lib
INCSYS:=$(ROOT)/arm-linux/sys-include
INC :=$(ROOT)/arm-linux/include
INCUSR = ../../inc
INCMGR = ./inc

WEC_LDFLAGS=-L$(LIB) -L$(LIB1) -L$(LIBUSR)
STRIP=arm-linux-strip

OBJS = $(OBJDIR)/main.o\
       $(OBJDIR)/request.o\
       $(OBJDIR)/notify.o\
       $(OBJDIR)/timer.o\
       $(OBJDIR)/touch.o\
       $(OBJDIR)/key.o\
       $(OBJDIR)/opening.o\
       $(OBJDIR)/menu.o\
       $(OBJDIR)/winerror.o\
       $(OBJDIR)/win_tbl.o
TARGET = mgr

LIBS= -lc -lgcc -lipc -lx_dbg
#LIBS= -lc -lgcc

all: $(BINDIR)/$(TARGET)
#	$(CC) $(CFLAGS) $(WEC_LDFLAGS) gui/*.c win/*.c -o $(BINDIR)/$(TARGET) $(LIBS)
#	$(STRIP) $(BINDIR)/$(TARGET) 

#GUI Framework sources
$(OBJDIR)/main.o: gui/main.c
	$(CC) $(CFLAGS) gui/main.c -c -o $@
$(OBJDIR)/request.o: gui/request.c
	$(CC) $(CFLAGS) gui/request.c -c -o $@
$(OBJDIR)/notify.o: gui/notify.c
	$(CC) $(CFLAGS) gui/notify.c -c -o $@
$(OBJDIR)/timer.o: gui/timer.c
	$(CC) $(CFLAGS) gui/timer.c -c -o $@
$(OBJDIR)/touch.o: gui/touch.c
	$(CC) $(CFLAGS) gui/touch.c -c -o $@
$(OBJDIR)/key.o: gui/key.c
	$(CC) $(CFLAGS) gui/key.c -c -o $@

#Each window sources
$(OBJDIR)/opening.o: win/opening.c
	$(CC) $(CFLAGS) win/opening.c -c -o $@
$(OBJDIR)/menu.o: win/menu.c
	$(CC) $(CFLAGS) win/menu.c -c -o $@
$(OBJDIR)/winerror.o: win/winerror.c
	$(CC) $(CFLAGS) win/winerror.c -c -o $@
$(OBJDIR)/win_tbl.o: win/win_tbl.c
	$(CC) $(CFLAGS) win/win_tbl.c -c -o $@

#Linking MGR binary
$(BINDIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(WEC_LDFLAGS) $(LIBS) $(OBJS) -o $@
#	$(CC) $(CFLAGS) $(WEC_LDFLAGS) $(LIBS) $(OBJS) $(OBJDIR)/x_dbg.o $(OBJDIR)/ipc.o -o $@ -static
	$(STRIP) $(BINDIR)/$(TARGET) 

#Cleaning objs and binaries
clean:
	rm -f $(OBJS)
	rm -f $(BINDIR)/$(TARGET)
