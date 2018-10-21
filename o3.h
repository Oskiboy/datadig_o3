/* En rask måte å unngå header recursion på er å sjekke om verdi, f.eks. 'O3_H',
   er definert. Hvis ikke, definer 'O3_H' og deretter innholdet av headeren 
   (merk endif på bunnen). Nå kan headeren inkluderes så mange ganger vi vil 
   uten at det blir noen problemer. */
#ifndef O3_H
#define O3_H

// Type-definisjoner fra std-bibliotekene
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

// Type-aliaser
typedef uint32_t word;
typedef uint8_t  byte;

///Defines all the mcus states
typedef enum State {
    SET_SEC,
    SET_MIN,
    SET_HOUR,
    COUNT,
    ALARM,
    NUM_STATES
} state_t;

struct timestamp {
    uint32_t sec,
    uint32_t min,
    uint32_t hour
};

// Prototyper for bibliotekfunksjoner
void init(void);
void lcd_write(char* string);
void int_to_string(char *timestamp, unsigned int offset, int i);
void time_to_string(char *timestamp, int h, int m, int s);

// Prototyper
// legg prototyper for dine funksjoner her
state_t set_sec_func(void);
state_t set_min_func(void);
state_t set_hour_func(void);
state_t count_func(void);
state_t alarm_func(void);
void reduce_timestamp(struct timestamp &ts);
int zero_time(struct timestamp &ts)
void init_systic(void);
void enable_systick(void);
void toggle_led(port_pin_t led_pin);


#endif
