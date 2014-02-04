#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_PORTA		1
#define GPIO_PORTB		2
#define GPIO_PORTC		4
#define GPIO_PORTD		8
#define GPIO_PORTE		16

int             Ioperm (void);
void            Outl(unsigned int b, unsigned long int port);
unsigned int    Inl(unsigned long int port);

extern int gpio_open(unsigned char port);
extern int gpio_readport(unsigned char port, unsigned short *val);
extern int gpio_setportdir(unsigned char port, unsigned short mask, unsigned short dir);
extern int gpio_setportval(unsigned char port, unsigned short mask, unsigned short val);
extern int gpio_setportpull(unsigned char port, unsigned short mask, unsigned short pull);

#endif /* __GPIO_H__ */
