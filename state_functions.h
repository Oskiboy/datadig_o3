#ifndef STATE_FUNC_H
#define STATE_FUNC_H

#define INTERACT    0b1
#define TRANSITION  0b10
#define REDUCE_TIME 0b100

struct timestamp;

typedef enum State {
    SET_SEC,
    SET_MIN,
    SET_HOUR,
    COUNT,
    ALARM,
    NUM_STATES
} state_t;

state_t set_sec_func(void);
state_t set_min_func(void);
state_t set_hour_func(void);
state_t count_func(void);
state_t alarm_func(void);


#endif
