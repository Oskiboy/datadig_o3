
#ifndef SYSTICK_H
#define SYSTICK_H

// Sys-Tick adresse
#define SYSTICK_BASE    0xE000E010
#define SYSTICK_CTRL    0
#define SYSTICK_LOAD    4
#define SYSTICK_VAL     8
#define SYSTICK_CALIB   12

// Antall klokkesignaler per sekund
#define FREQUENCY 14000000 

// CTRL-register-masker
#define SysTick_CTRL_CLKSOURCE_Msk  0b100
#define SysTick_CTRL_TICKINT_Msk    0b010
#define SysTick_CTRL_ENABLE_Msk     0b001

#define SYSTICK_CTRL_REG (*((volatile unsigned int*)(SYSTICK_BASE+SYSTICK_CTRL)))
#define SYSTICK_LOAD_REG (*((volatile unsigned int*)(SYSTICK_BASE+SYSTICK_LOAD)))
#define SYSTICK_VAL_REG  (*((volatile unsigned int*)(SYSTICK_BASE+SYSTICK_VAL)))7



#endif
