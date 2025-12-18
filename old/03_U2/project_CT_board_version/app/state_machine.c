/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module state_machine.
 * --
 * -- $Id: state_machine.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "state_machine.h"
#include "action_handler.h"
#include "timer.h"


/* -- Macros used by student code
 * ------------------------------------------------------------------------- */

#define TIMER_DURATION_LEFT  400u
#define TIMER_DURATION_RIGHT 400u
#define TIMER_DURATION_SPIN  500u

#define TEXT_OPENED                 "OPENED         "
#define TEXT_CLOSED					"CLOSED         "
#define TEXT_LOCKED					"LOCKED         "

#define HIGH    1u
#define LOW     0u


/* Local variables
 * ------------------------------------------------------------------------- */

static state_t state = OPENED;
static uint8_t floater_position = LOW;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void fsm_init(void)
{
    action_handler_init();
    ah_lcd_write(TEXT_OPENED);
    state = OPENED;
}


/*
 * See header file
 */
void fsm_handle_event(event_t event)
{
    // State machine implementation according to your model from last week's exercise.
	
    if ((event == BUTTON_STOP) && (state != OPENED))
    {
        ah_motor_off();
        ah_close_valve();
        ah_heater_off();
        ah_pump_on();
        timer_stop();
        state = STOPPING;
    }
    
    if (event == FLOATER_HIGH)
    {
        floater_position = HIGH;
    }
    else if (event == FLOATER_LOW)
    {
        floater_position = LOW;
    }
    
    switch (state)
    {
        case OPENED:
            if (event == DOOR_CLOSED)
            {
                ah_lcd_write(TEXT_CLOSED);
                state = CLOSED;
            }
            break;
        case CLOSED:
            switch (event)
            {
                case DOOR_OPENED:
                    ah_lcd_write(TEXT_OPENED);
                    state = OPENED;
                    break;
                case BUTTON_WASH:
                    ah_lcd_write(TEXT_LOCKED);
                    ah_lock_door();
                    ah_open_valve();
                    state = FILLING;
                    break;
                case BUTTON_SPIN:
                    ah_lcd_write(TEXT_LOCKED);
                    ah_lock_door();
                    ah_motor_on(LEFT, FAST);
                    timer_start(TIMER_DURATION_SPIN);
                    state = SPIN;
                    break;
                default:
                    break;
            }
            break;
        case FILLING:
            if (floater_position == HIGH)
            {
                ah_close_valve();
                ah_heater_on();
                state = HEATING;
            }
            break;
        case HEATING:
            if (event == TEMPERATURE_HOT)
            {
                ah_heater_off();
                ah_motor_on(RIGHT, SLOW);
                timer_start(TIMER_DURATION_RIGHT);
                state = AGITATE_RIGHT;
            }
            break;
        case AGITATE_RIGHT:
            if (event == TIME_OUT)
            {
                ah_motor_off();
                ah_motor_on(LEFT, SLOW);
                timer_start(TIMER_DURATION_LEFT);
                state = AGITATE_LEFT;
            }
            break;
        case AGITATE_LEFT:
            if (event == TIME_OUT)
            {
                ah_motor_off();
                ah_pump_on();
                state = DRAIN;
            }
            break;
        case DRAIN:
            if (floater_position == LOW)
            {
                ah_pump_off();
                ah_motor_on(LEFT, FAST);
                timer_start(TIMER_DURATION_SPIN);
                state = SPIN;
            }
            break;
        case SPIN:
            if (event == TIME_OUT)
            {
                ah_motor_off();
                ah_unlock_door();
                ah_lcd_write(TEXT_CLOSED);
                state = CLOSED;
            }
            break;
        case STOPPING:
            if (floater_position == LOW)
            {
                ah_pump_off();
                ah_unlock_door();
                ah_lcd_write(TEXT_CLOSED);
                state = CLOSED;
            }
            break;
    }
}
