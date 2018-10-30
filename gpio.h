#ifndef GPIO_H
#define GPIO_H
#include "o3.h"

typedef struct{
	unsigned int   port;
	unsigned int   pin;
} port_pin_t;


// GPIO pin modes
#define GPIO_MODE_INPUT  0b0001
#define GPIO_MODE_OUTPUT 0b0100

// GPIO address
#define GPIO_BASE 			0x40006000

// GPIO instanziation
#define GPIO_PORT(_name)	volatile gpio_t *_name = (gpio_t*)GPIO_BASE;

//Struct for holding port specific registers.
struct gpio_port_t {
	uint32_t	CTRL;
	uint32_t	MODEL;
	uint32_t	MODEH;
	uint32_t	DOUT;
	uint32_t	DOUTSET;
	uint32_t	DOUTCLR;
	uint32_t	DOUTTGL;
	uint32_t	DIN;
	uint32_t	PINLOCKN;
};

//The entire gpio register set.
typedef struct {
	struct gpio_port_t 	ports[6];
	uint32_t	not_used[10];
	uint32_t 	EXTIPSELL;
	uint32_t 	EXTIPSELH;
	uint32_t 	EXTIRISE;
	uint32_t 	EXTIFALL;
	uint32_t 	IEN;
	uint32_t 	IF;
	uint32_t 	IFS;
	uint32_t 	IFC;
	uint32_t 	ROUTE;
	uint32_t 	INSENSE;
	uint32_t 	LOCK;
	uint32_t 	CTRL;
	uint32_t 	CMD;
	uint32_t 	EM4WUEN;
	uint32_t 	EM4WUPOL;
	uint32_t 	EM4WUCAUSE;
} gpio_t;

// GPIO port numbers
#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2
#define GPIO_PORT_D 3
#define GPIO_PORT_E 4
#define GPIO_PORT_F 5

#endif
