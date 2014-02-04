
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/mman.h>

//#include <asm/page.h>         //
#include <sys/sysctl.h>

#include <linux/version.h>

#define    	GPIO_BA   		0xB8001000	/* GPIO Control */

#define     GPIO_PORT_BASE		0x10

#define     GPIO_OMD_OFFSET		0x00
#define     GPIO_PUEN_OFFSET	0x04
#define     GPIO_DOUT_OFFSET	0x08
#define     GPIO_PIN_OFFSET		0x0c

static struct {
    unsigned long int	base;
    unsigned long int	io_base;
} io;

#define IO_ADDR(port)	(io.base + (port))

int Ioperm ()
{
    int psize;

    psize = getpagesize();

    io.io_base = GPIO_BA;
    io.base = 0;

    if (! io.base) {
        int fd;

        fd = open ("/dev/mem", O_RDWR);
        if (fd < 0)
    	    return -1;

        io.base = (unsigned long int) mmap (0, psize ,
					  PROT_READ | PROT_WRITE,
					  MAP_SHARED, fd, io.io_base);
        close (fd);
        if ((long) io.base == -1) {
            printf("mmap failed\n");
	    	return -1;
        }
     }

    return 0;
}


void
Outl(unsigned int b, unsigned long int port)
{
    *((volatile unsigned long *) (IO_ADDR (port))) = b;
}


unsigned int
Inl(unsigned long int port)
{
    return *((volatile unsigned long *) (IO_ADDR (port)));
}


int gpio_open(unsigned char port)
{
/*
	アドレスがGPIO_BA圏外なので未実装。モード変更くらいはカーネルで
*/
	return -1;

}

int gpio_readport(unsigned char port, unsigned short *val)
{
	*val = *((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_PIN_OFFSET ));
	return 0;
}

int gpio_setportdir(unsigned char port, unsigned short mask, unsigned short dir)
{
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_OMD_OFFSET )) &= ~(mask & (mask ^ val));
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_OMD_OFFSET )) |= (mask & val);
	return(0);
}

int gpio_setportval(unsigned char port, unsigned short mask, unsigned short val)
{
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_DOUT_OFFSET )) &= ~(mask & (mask ^ val));
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_DOUT_OFFSET )) |= (mask & val);
	return(0);
}

int gpio_setportpull(unsigned char port, unsigned short mask, unsigned short pull)
{
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_PUEN_OFFSET )) &= ~(mask & (mask ^ val));
	*((volatile unsigned long *) IO_ADDR ( (long) (port - 1) * GPIO_PORT_BASE + GPIO_PUEN_OFFSET )) |= (mask & val);
	return(0);
}

