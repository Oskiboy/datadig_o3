#ifndef O3_H
#define O3_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

typedef uint32_t word;
typedef uint8_t  byte;

///Defines all the MCUs states
typedef enum State {
    SET_SEC,
    SET_MIN,
    SET_HOUR,
    COUNT,
    ALARM,
    NUM_STATES
} state_t;

typedef struct {
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
} timestamp_t;

// Prototypes for library functions
void init(void);
void lcd_write(char* string);
void int_to_string(char *timestamp, unsigned int offset, int i);
void time_to_string(char *timestamp, int h, int m, int s);

//My prototypes
void set_up_gpio(void);
state_t set_sec_func(void);
state_t set_min_func(void);
state_t set_hour_func(void);
state_t count_func(void);
state_t alarm_func(void);
void reduce_timestamp(timestamp_t* ts);
int zero_time(timestamp_t* ts);
void init_systick(void);
void enable_systick(void);
void disable_systick(void);
void toggle_led(port_pin_t led_pin);

#endif
