/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module action_handler.
 * --
 * -- $Id: action_handler.c 3790 2016-11-22 10:20:17Z kesr $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <reg_ctboard.h>
#include <hal_ct_lcd.h>

/* user includes */
#include "action_handler.h"


/* -- Macros for accessing CT Board
 * ------------------------------------------------------------------------- */

#define PORT_OUTPUT (CT_GPIO->OUT.BYTE.P1)


/* -- Macros for washing machine
 * ------------------------------------------------------------------------- */

#define MASK_DOOR_LOCK   (0x7f)
#define MASK_VALVE_OPEN  (0xbf)
#define MASK_HEATER_ON   (0xdf)
#define MASK_MOTOR_SLOW  (0xf7)
#define MASK_MOTOR_FAST  (0xfb)
#define MASK_MOTOR_RIGHT (0xfd)
#define MASK_PUMP_ON     (0xef)


/* Public function definitions
 * ------------------------------------------------------------------------- */
/*
 * See header file
 */
void action_handler_init(void)
{

    /* Deactivate low-active output */
    PORT_OUTPUT = 0xff;
    /// STUDENTS: To be programmed

    // All outputs are deactivated (high = inactive for low-active outputs)
    // Initial state: everything off, door unlocked, valve closed

    /// END: To be programmed
}


/*
 * See header file
 */
void ah_lcd_write(char text[])
{
    hal_ct_lcd_clear();
    hal_ct_lcd_write(0, text);
}


/*
 * See header file
 */
void ah_lock_door(void)
{
    PORT_OUTPUT &= MASK_DOOR_LOCK;  // Clear bit 7 to lock
}

/*
 * See header file
 */
void ah_unlock_door(void)
{
    PORT_OUTPUT |= ~MASK_DOOR_LOCK;  // Set bit 7 to unlock
}


/*
 * See header file
 */
void ah_open_valve(void)
{
    PORT_OUTPUT &= MASK_VALVE_OPEN;  // Clear bit 6 to open
}

/*
 * See header file
 */
void ah_close_valve(void)
{
    PORT_OUTPUT |= ~MASK_VALVE_OPEN;  // Set bit 6 to close
}


/*
 * See header file
 */
void ah_heater_on(void)
{
    PORT_OUTPUT &= MASK_HEATER_ON;  // Clear bit 5 to turn on
}

/*
 * See header file
 */
void ah_heater_off(void)
{
    PORT_OUTPUT |= ~MASK_HEATER_ON;  // Set bit 5 to turn off
}


/*
 * See header file
 */
void ah_motor_on(motor_direction_t motor_direction, motor_speed_t motor_speed)
{
    // First turn off motor to clear any previous state
    ah_motor_off();

    // Set motor direction and speed
    // Direction: bit 1 (0=right, 1=left)
    // Speed: bit 3 for slow, bit 2 for fast
    if (motor_direction == RIGHT) {
        PORT_OUTPUT &= MASK_MOTOR_RIGHT;  // Clear bit 1 for right
        if (motor_speed == SLOW) {
            PORT_OUTPUT &= MASK_MOTOR_SLOW;  // Clear bit 3 for slow
        } else {
            PORT_OUTPUT &= MASK_MOTOR_FAST;  // Clear bit 2 for fast
        }
    } else {  // LEFT
        // Keep bit 1 set for left (don't clear it)
        if (motor_speed == SLOW) {
            PORT_OUTPUT &= MASK_MOTOR_SLOW;  // Clear bit 3 for slow
        } else {
            PORT_OUTPUT &= MASK_MOTOR_FAST;  // Clear bit 2 for fast
        }
    }
}

/*
 * See header file
 */
void ah_motor_off(void)
{
    // Set all motor control bits high to turn off (low-active)
    PORT_OUTPUT |= ~MASK_MOTOR_RIGHT;  // Set bit 1
    PORT_OUTPUT |= ~MASK_MOTOR_SLOW;   // Set bit 3
    PORT_OUTPUT |= ~MASK_MOTOR_FAST;   // Set bit 2
}


/*
 * See header file
 */
void ah_pump_on(void)
{
    PORT_OUTPUT &= MASK_PUMP_ON;  // Clear bit 4 to turn on
}

/*
 * See header file
 */
void ah_pump_off(void)
{
    PORT_OUTPUT |= ~MASK_PUMP_ON;  // Set bit 4 to turn off
}
