#include "state_functions.h"

volatile static int FLAGS = 0;
struct timestamp _timestamp = {0,0,0};


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
        _timestamp.min += 60;
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
        _timestamp.hour += 3600;
        return SET_HOUR;
    }
    if (FLAGS & TRANSITION) {
        FLAGS &= ~TRANSITION;
        start_systick();
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
    if (zero_time(_timestamp)) {
        disable_systick();
        return ALARM;
    }
    return COUNT;
}

state_t alarm_func(void) {
    port_pin_t pin = {GPIO_PORT_B, LED_PIN};
    toggle_led(pin);
    return ALARM;
}
