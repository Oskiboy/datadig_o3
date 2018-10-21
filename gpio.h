#ifndef GPIO_H
#define GPIO_H
#include "o3.h"
// Pin-datatype
typedef struct{
	unsigned int   port;
	unsigned int   pin;
} port_pin_t;

// GPIO pin modes
#define GPIO_MODE_INPUT  0b0001
#define GPIO_MODE_OUTPUT 0b0100

// GPIO-adresse
#define GPIO_BASE 0x40006000

#define GPIO_PORT_CTRL       0     // Port Control Register
#define GPIO_PORT_MODEL      4     // Port Pin Mode Low Register
#define GPIO_PORT_MODEH      8     // Port Pin Mode High Register
#define GPIO_PORT_DOUT       12    // Port Data Out Register
#define GPIO_PORT_DOUTSET    16    // Port Data Out Set Register
#define GPIO_PORT_DOUTCLR    20    // Port Data Out Clear Register
#define GPIO_PORT_DOUTTGL    24    // Port Data Out Toggle Register
#define GPIO_PORT_DIN      	 28    // Port Data In Register
#define GPIO_PORT_PINLOCKN   32    // Port Unlocked Pins Register_

// GPIO port-nummere
#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2
#define GPIO_PORT_D 3
#define GPIO_PORT_E 4
#define GPIO_PORT_F 5

//GPIO port definitions
#define PORT_BASE(port_num) 		(GPIO_BASE+(port_num)*sizeof(word))
#define GPIO_TOGGLE_REG(port_num) 	(*((volatile unsigned int*)(PORT_BASE(port_num)+GPIO_PORT_DOUTTGL)))
#define GPIO_CTRL_REG(port_num) 	(*((volatile unsigned int*)(PORT_BASE(port_num)+GPIO_PORT_CTRL)))
#define GPIO_DIN_REG(port_num)		(*((volatile unsigned int*)(PORT_BASE(port_num)+GPIO_PORT_DIN)))
#endif
