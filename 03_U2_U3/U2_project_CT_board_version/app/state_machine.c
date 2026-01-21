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

#define TEXT_DOOR_OPEN       "DOOR_OPEN      "
#define TEXT_IDLE            "IDLE           "
#define TEXT_FILL_WATER      "FILL_WATER     "
#define TEXT_HEAT_WATER      "HEAT_WATER     "
#define TEXT_ROTATE_RIGHT    "ROTATE_RIGHT   "
#define TEXT_ROTATE_LEFT     "ROTATE_LEFT    "
#define TEXT_EMPTY_WATER     "EMPTY_WATER    "
#define TEXT_SPIN_DRY        "SPIN_DRY       "
#define TEXT_SHUT_DOWN       "SHUT_DOWN      "

#define HIGH    1u
#define LOW     0u


/* Local variables
 * ------------------------------------------------------------------------- */

static state_t state = DOOR_OPEN;
static uint8_t floater_position = LOW;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void fsm_init(void)
{
    action_handler_init();

    // Initial actions according to state diagram
    ah_heater_off();
    ah_close_valve();
    ah_motor_off();
    ah_pump_off();
    ah_unlock_door();

    ah_lcd_write(TEXT_DOOR_OPEN);
    state = DOOR_OPEN;
}


/*
 * See header file
 */
void fsm_handle_event(event_t event)
{
    // State machine implementation according to your model from last week's exercise.

    // Track floater position across all events
    if (event == FLOATER_HIGH) {
        floater_position = HIGH;
    }
    else if (event == FLOATER_LOW) {
        floater_position = LOW;
    }

    // State machine logic
    switch (state) {
        case DOOR_OPEN:
            if (event == DOOR_CLOSED) {
                ah_lcd_write(TEXT_IDLE);
                state = IDLE;
            }
            break;

        case IDLE:
            switch (event) {
                case DOOR_OPENED:
                    ah_lcd_write(TEXT_DOOR_OPEN);
                    state = DOOR_OPEN;
                    break;
                case BUTTON_WASH:
                    ah_lock_door();
                    ah_open_valve();
                    ah_lcd_write(TEXT_FILL_WATER);
                    state = FILL_WATER;
                    break;
                case BUTTON_SPIN:
                    ah_pump_on();
                    ah_lcd_write(TEXT_EMPTY_WATER);
                    state = EMPTY_WATER;
                    break;
                default:
                    break;
            }
            break;

        case FILL_WATER:
            switch (event) {
                case FLOATER_HIGH:
                    if (floater_position == HIGH) {
                        ah_close_valve();
                        ah_heater_on();
                        ah_lcd_write(TEXT_HEAT_WATER);
                        state = HEAT_WATER;
                    }
                    break;
                case BUTTON_STOP:
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                case BUTTON_SPIN:
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_EMPTY_WATER);
                    state = EMPTY_WATER;
                    break;
                default:
                    break;
            }
            break;

        case HEAT_WATER:
            switch (event) {
                case TEMPERATURE_HOT:
                    ah_heater_off();
                    timer_start(TIMER_DURATION_RIGHT);
                    ah_motor_on(RIGHT, SLOW);
                    ah_lcd_write(TEXT_ROTATE_RIGHT);
                    state = ROTATE_RIGHT;
                    break;
                case BUTTON_SPIN:
                    ah_heater_off();
                    ah_pump_on();
                    ah_lcd_write(TEXT_EMPTY_WATER);
                    state = EMPTY_WATER;
                    break;
                case BUTTON_STOP:
                    ah_heater_off();
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                default:
                    break;
            }
            break;

        case ROTATE_RIGHT:
            switch (event) {
                case TIME_OUT:
                    ah_motor_off();
                    ah_motor_on(LEFT, SLOW);
                    timer_start(TIMER_DURATION_LEFT);
                    ah_lcd_write(TEXT_ROTATE_LEFT);
                    state = ROTATE_LEFT;
                    break;
                case BUTTON_STOP:
                    ah_motor_off();
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                default:
                    break;
            }
            break;

        case ROTATE_LEFT:
            switch (event) {
                case TIME_OUT:
                    ah_motor_off();
                    ah_pump_on();
                    ah_lcd_write(TEXT_EMPTY_WATER);
                    state = EMPTY_WATER;
                    break;
                case BUTTON_STOP:
                    ah_motor_off();
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                default:
                    break;
            }
            break;

        case EMPTY_WATER:
            switch (event) {
                case FLOATER_LOW:
                    if (floater_position == LOW) {
                        ah_pump_off();
                        timer_start(TIMER_DURATION_SPIN);
                        ah_motor_on(LEFT, FAST);
                        ah_lcd_write(TEXT_SPIN_DRY);
                        state = SPIN_DRY;
                    }
                    break;
                case BUTTON_STOP:
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                default:
                    break;
            }
            break;

        case SPIN_DRY:
            switch (event) {
                case TIME_OUT:
                    ah_motor_off();
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                case BUTTON_STOP:
                    ah_motor_off();
                    ah_close_valve();
                    ah_pump_on();
                    ah_lcd_write(TEXT_SHUT_DOWN);
                    state = SHUT_DOWN;
                    break;
                default:
                    break;
            }
            break;

        case SHUT_DOWN:
            if (event == FLOATER_LOW) {
                if (floater_position == LOW) {
                    ah_pump_off();
                    ah_unlock_door();
                    ah_lcd_write(TEXT_IDLE);
                    state = IDLE;
                }
            }
            break;
    }
}
