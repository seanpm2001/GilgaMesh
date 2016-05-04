#pragma once

#if defined(TESTING) && !defined(TIMER_TEST)

#else

#include <app_timer.h>
#include <error.h>
#include <logger.h>

#include <led.h>


void timer_initialize(void);
void turn_off_voting_notification(void);
void turn_on_buffer_clear_timeout(void);
void turn_off_buffer_clear_timeout(void);


#endif