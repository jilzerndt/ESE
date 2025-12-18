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

#define TIMER_DURATION_ROTATE  400u
#define TIMER_DURATION_SPIN    500u

#define TEXT_DOOR_OPEN      "DOOR_OPEN      "
#define TEXT_IDLE           "IDLE           "
#define TEXT_FILL_WATER     "FILL_WATER     "
#define TEXT_HEAT_WATER     "HEAT_WATER     "
#define TEXT_ROTATE         "ROTATE         "
#define TEXT_EMPTY_WATER    "EMPTY_WATER    "
#define TEXT_SPIN_DRY       "SPIN_DRY       "
#define TEXT_SHUT_DOWN      "SHUT_DOWN      "

#define HIGH    1u
#define LOW     0u


/* Local variables
 * ------------------------------------------------------------------------- */

static state_t state = DOOR_OPEN;
static uint8_t water_level = LOW;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void fsm_init(void)
{
    action_handler_init();
    // Initial state actions
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
    // State machine implementation according to personal washing machine design

    // Track water level based on floater events
    if (event == FLOATER_HIGH)
    {
        water_level = HIGH;
    }
    else if (event == FLOATER_LOW)
    {
        water_level = LOW;
    }

    // Handle STOP button - transitions to SHUT_DOWN from most states
    if ((event == BUTTON_STOP) && (state != DOOR_OPEN) && (state != IDLE) && (state != SHUT_DOWN))
    {
        ah_motor_off();
        ah_heater_off();
        ah_close_valve();
        ah_pump_on();
        timer_stop();
        ah_lcd_write(TEXT_SHUT_DOWN);
        state = SHUT_DOWN;
        return;  // Exit early to handle stop immediately
    }

    switch (state)
    {
        case DOOR_OPEN:
            if (event == DOOR_CLOSED)
            {
                ah_lcd_write(TEXT_IDLE);
                state = IDLE;
            }
            break;

        case IDLE:
            if (event == DOOR_OPENED)
            {
                ah_lcd_write(TEXT_DOOR_OPEN);
                state = DOOR_OPEN;
            }
            else if (event == BUTTON_WASH)  // START_BTN
            {
                ah_lock_door();
                ah_open_valve();
                ah_lcd_write(TEXT_FILL_WATER);
                state = FILL_WATER;
            }
            break;

        case FILL_WATER:
            if (water_level == HIGH)  // WATER_FULL
            {
                ah_close_valve();
                ah_heater_on();
                ah_lcd_write(TEXT_HEAT_WATER);
                state = HEAT_WATER;
            }
            break;

        case HEAT_WATER:
            if (event == TEMPERATURE_HOT)  // TEMP_HOT
            {
                ah_heater_off();
                timer_start(TIMER_DURATION_ROTATE);
                ah_motor_on(LEFT, SLOW);
                ah_lcd_write(TEXT_ROTATE);
                state = ROTATE;
            }
            break;

        case ROTATE:
            if (event == TIME_OUT)
            {
                ah_motor_off();
                ah_pump_on();
                ah_lcd_write(TEXT_EMPTY_WATER);
                state = EMPTY_WATER;
            }
            break;

        case EMPTY_WATER:
            if (water_level == LOW)  // WATER_EMPTY
            {
                ah_pump_off();
                timer_start(TIMER_DURATION_SPIN);
                ah_motor_on(LEFT, FAST);
                ah_lcd_write(TEXT_SPIN_DRY);
                state = SPIN_DRY;
            }
            break;

        case SPIN_DRY:
            if (event == TIME_OUT)
            {
                ah_motor_off();
                ah_close_valve();
                ah_pump_on();
                ah_lcd_write(TEXT_SHUT_DOWN);
                state = SHUT_DOWN;
            }
            break;

        case SHUT_DOWN:
            if (water_level == LOW)  // WATER_EMPTY
            {
                ah_pump_off();
                ah_unlock_door();
                ah_lcd_write(TEXT_IDLE);
                state = IDLE;
            }
            break;
    }
}
