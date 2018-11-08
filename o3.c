#include "o3.h"
#include "gpio.h"
#include "systick.h"

/**************************************************************************//**
 * @brief Konverterer nummer til string 
 * Konverterer et nummer mellom 0 og 99 til string
 *****************************************************************************/
void int_to_string(char *timestamp, unsigned int offset, int i) {
    if (i > 99) {
        timestamp[offset]   = '9';
        timestamp[offset+1] = '9';
        return;
    }

    while (i > 0) {
	    if (i >= 10) {
		    i -= 10;
		    timestamp[offset]++;
		
	    } else {
		    timestamp[offset+1] = '0' + i;
		    i=0;
	    }
    }
}

/**************************************************************************//**
 * @brief Konverterer 3 tall til en timestamp-string
 * timestamp-argumentet må være et array med plass til (minst) 7 elementer.
 * Det kan deklareres i funksjonen som kaller som "char timestamp[7];"
 * Kallet blir dermed:
 * char timestamp[7];
 * time_to_string(timestamp, h, m, s);
 *****************************************************************************/
void time_to_string(char *timestamp, int h, int m, int s) {
    timestamp[0] = '0';
    timestamp[1] = '0';
    timestamp[2] = '0';
    timestamp[3] = '0';
    timestamp[4] = '0';
    timestamp[5] = '0';
    timestamp[6] = '\0';

    int_to_string(timestamp, 0, h);
    int_to_string(timestamp, 2, m);
    int_to_string(timestamp, 4, s);
}


//Define flags, pins and ports needed for the project.
#define INTERACT    0b1
#define TRANSITION  0b10
#define REDUCE_TIME 0b100

#define LED_PORT	GPIO_PORT_E
#define LED0_PIN	2
#define LED1_PIN	3

#define BUTTON_PORT GPIO_PORT_B
#define PB0_PIN		9
#define PB1_PIN		10

//Instanziation of the modules needed.
GPIO_PORT(gpio_m)
SYSTICK_REG(systick_m)

//FLAGS contains the flags for transitioning logic.
volatile uint8_t FLAGS = 0;
///The current state will contain the current state which corresponds to its function in the function pointer array.
state_t _current_state = SET_SEC;

///Holds the amount of time until the alarm is triggered.
static timestamp_t _timestamp = {0, 0, 0};

//A pin used for debugging.
port_pin_t	led0 = {LED_PORT, LED1_PIN};

///Defines an array of function pointers that will be used as different states.
state_t (*state_function_array[NUM_STATES])(void) = {
    set_sec_func,
    set_min_func,
    set_hour_func,
    count_func,
    alarm_func
};

///The statue function will be run every loop of the program and will return the next state.
state_t (*state_function)(void) = set_sec_func;
char time_string[7] =  {};


int main(void) {
    init();
    time_to_string(time_string, 0, 0, 0);
    lcd_write(time_string);

    set_up_gpio();

    init_systick();

    FLAGS = 0;

    while(1) {
        _current_state = state_function();
        state_function = state_function_array[_current_state];
        time_to_string(time_string, _timestamp.hour, _timestamp.min, _timestamp.sec);
        lcd_write(time_string);
    }
    return 0;
}

//PB0 interrupt handler
void GPIO_ODD_IRQHandler(void) {
    FLAGS |= INTERACT;
    toggle_led(led0);
    gpio_m->IFC |= (1 << PB0_PIN);
}

//PB1 interrupt handler
void GPIO_EVEN_IRQHandler(void) {
    FLAGS |= TRANSITION;
    toggle_led(led0);
    gpio_m->IFC |= (1 << PB1_PIN);
}

void init_systick() {
	//Add the correct mask for the SysTick to work.
	systick_m->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);
	//Load the systick with the correct frequency for each reset.
	systick_m->LOAD = FREQUENCY;
}

void enable_systick() {
	systick_m->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void disable_systick() {
	systick_m->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    FLAGS |= REDUCE_TIME;
    toggle_led(led0);
}

state_t set_sec_func(void) {
    if(FLAGS & INTERACT) {
        FLAGS &= ~INTERACT;
        _timestamp.sec++;
        return SET_SEC;
    }

    if(FLAGS & TRANSITION) {
        FLAGS &= ~TRANSITION;
        return SET_MIN;
    }
    return SET_SEC;
}

state_t set_min_func(void) {
    if(FLAGS & INTERACT) {
        FLAGS &= ~INTERACT;
        _timestamp.min++;
        return SET_MIN;
    }
    if (FLAGS & TRANSITION) {
        FLAGS &= ~TRANSITION;
        return SET_HOUR;
    }
    return SET_MIN;
}

state_t set_hour_func(void) {
    if (FLAGS & INTERACT) {
        FLAGS &= ~INTERACT;
        _timestamp.hour++;
        return SET_HOUR;
    }
    if (FLAGS & TRANSITION) {
        FLAGS &= ~TRANSITION;
        enable_systick();
        return COUNT;
    }
    return SET_HOUR;
}

state_t count_func(void) {
    if (FLAGS & REDUCE_TIME) {
        FLAGS &= ~REDUCE_TIME;
        reduce_timestamp(&_timestamp);
        return COUNT;
    }
    if (zero_time(&_timestamp)) {
        disable_systick();
        return ALARM;
    }
    return COUNT;
}

state_t alarm_func(void) {
    port_pin_t pin = {LED_PORT , LED0_PIN};
    toggle_led(pin);
    if (FLAGS & TRANSITION) {
    	FLAGS &= ~TRANSITION;
    	gpio_m->ports[LED_PORT].DOUTCLR |= (1 << LED0_PIN);
        return SET_SEC;
    }
    return ALARM;
}

void toggle_led(port_pin_t led_pin) {
    gpio_m->ports[led_pin.port].DOUTTGL |= (1 << led_pin.pin);
}

void reduce_timestamp(timestamp_t* ts) {
    if (ts->sec) {
        ts->sec--;
    }
    else {
        if (ts->min) {
            ts->min--;
            ts->sec = 60;
        }
        else {
            if (ts->hour) {
                ts->hour--;
                ts->min = 60;
            }
        }

    }
}

int zero_time(timestamp_t* ts) {
    if (!(ts->hour) && !(ts->min) && !(ts->sec)) {
        return 1;
    }
    else {
        return 0;
    }
}

void set_up_gpio(void) {
    //Enable input on button 0, PORT_B pin 9
	gpio_m->ports[BUTTON_PORT].MODEH &= ~(0b1111 << 4);
    gpio_m->ports[BUTTON_PORT].MODEH |= (GPIO_MODE_INPUT << 4);
    //Enable input on button 1, PORT_B pin 10
    gpio_m->ports[BUTTON_PORT].MODEH &= ~(0b1111 << 8);
    gpio_m->ports[BUTTON_PORT].MODEH |= (GPIO_MODE_INPUT << 8);
    //Enable output on led 0, PORT_E pin 0
    gpio_m->ports[LED_PORT].MODEL &= (0b1111 << 12);
    gpio_m->ports[LED_PORT].MODEL |= (GPIO_MODE_OUTPUT << 12);
    //Enable led 1, PORT_E pin 2
    gpio_m->ports[LED_PORT].MODEL &= (0b1111 << 8);
    gpio_m->ports[LED_PORT].MODEL |= (GPIO_MODE_OUTPUT << 8);

    //Enable pin 9 and 10 interrupt on port B
    gpio_m->EXTIPSELH |= 	(0b0001 << 4) | (0b0001 << 8);
    //Interrupts happen on falling edges
    gpio_m->EXTIFALL |= 	(1 << PB0_PIN) | (1 << PB1_PIN);
    //Clear the interrupt flag register
    gpio_m->IFC |=          (1 << PB0_PIN) | (1 << PB1_PIN);
    //Enable interrupts
    gpio_m->IEN |= 			(1 << PB0_PIN) | (1 << PB1_PIN);

}
