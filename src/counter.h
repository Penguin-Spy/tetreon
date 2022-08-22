#include <tice.h>

#define TIMER_MAX 32768
#define COUNTER_TICK (TIMER_MAX / 16)

uint8_t counter_move = 0;
uint8_t counter_gravity = 0;
uint8_t counter_lock = 0;

#define counter_move_set_start()  counter_move = 3
#define counter_move_set_hold()   counter_move = 1
#define counter_gravity_reset()   counter_gravity = 8
#define counter_lock_set()        counter_lock = 0
#define counter_lock_reset()      counter_lock = 8

#define check_counter_move        counter_move == 0
#define check_counter_gravity     counter_gravity == 0
#define check_counter_lock        counter_lock == 0

#define decrement_counters() \
if(counter_move > 0)    counter_move--; \
if(counter_gravity > 0) counter_gravity--; \
if(counter_lock > 0)    counter_lock--;