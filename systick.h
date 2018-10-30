
#ifndef SYSTICK_H
#define SYSTICK_H

// SysTick address
#define SYSTICK_BASE    0xE000E010

// Frequency to get 1 sec SysTick interrupts
#define FREQUENCY 14000000 

// CTRL-register-masker
#define SysTick_CTRL_CLKSOURCE_Msk  0b100
#define SysTick_CTRL_TICKINT_Msk    0b010
#define SysTick_CTRL_ENABLE_Msk     0b001

//Struct for holding the SysTick registers.
typedef struct {
	uint32_t	CTRL;
	uint32_t	LOAD;
	uint32_t	VAL;
	uint32_t	CALIB;
}	systick_t;

//Instanziation of the SysTick register.
#define SYSTICK_REG(_name) volatile systick_t* _name = (systick_t*)SYSTICK_BASE;

#endif
