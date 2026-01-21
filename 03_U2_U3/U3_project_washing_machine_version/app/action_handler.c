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

#define PORT_OUTPUT (CT_LED->WORD)


/* -- Macros for washing machine
 * ------------------------------------------------------------------------- */

#define MASK_DOOR_LOCK          (0x00000001)
#define MASK_VALVE              (0x00000010)
#define MASK_HEATER             (0x00000100)
#define MASK_PUMP               (0x00001000)
#define MASK_MOTOR_RIGHT_SLOW   (0x00010000)
#define MASK_MOTOR_RIGHT_FAST   (0x00100000)
#define MASK_MOTOR_LEFT_SLOW    (0x01000000)
#define MASK_MOTOR_LEFT_FAST    (0x10000000)
#define MASK_MOTOR_OFF          (0x0000FFFF)


/* Public function definitions
 * ------------------------------------------------------------------------- */
/*
 * See header file
 */
void action_handler_init(void)
{
    /// STUDENTS: To be programmed

    /* Initialize LED output to all off (high-active: 0 = off) */
    PORT_OUTPUT = 0;
    // Initial state: all LEDs off = everything off, door unlocked, valve closed

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
    PORT_OUTPUT |= MASK_DOOR_LOCK;  // Set LED 0 to lock
}

/*
 * See header file
 */
void ah_unlock_door(void)
{
    PORT_OUTPUT &= ~MASK_DOOR_LOCK;  // Clear LED 0 to unlock
}


/*
 * See header file
 */
void ah_open_valve(void)
{
    PORT_OUTPUT |= MASK_VALVE;  // Set LED 4 to open valve
}

/*
 * See header file
 */
void ah_close_valve(void)
{
    PORT_OUTPUT &= ~MASK_VALVE;  // Clear LED 4 to close valve
}


/*
 * See header file
 */
void ah_heater_on(void)
{
    PORT_OUTPUT |= MASK_HEATER;  // Set LED 8 to turn on heater
}

/*
 * See header file
 */
void ah_heater_off(void)
{
    PORT_OUTPUT &= ~MASK_HEATER;  // Clear LED 8 to turn off heater
}


/*
 * See header file
 */
void ah_motor_on(motor_direction_t motor_direction, motor_speed_t motor_speed)
{
    // Turn on specific LED for each motor configuration
    if (motor_direction == LEFT) {
        if (motor_speed == SLOW) {
            PORT_OUTPUT |= MASK_MOTOR_LEFT_SLOW;  // Set LED 24 for left-slow
        } else {
            PORT_OUTPUT |= MASK_MOTOR_LEFT_FAST;  // Set LED 28 for left-fast
        }
    } else {  // RIGHT
        if (motor_speed == SLOW) {
            PORT_OUTPUT |= MASK_MOTOR_RIGHT_SLOW;  // Set LED 16 for right-slow
        } else {
            PORT_OUTPUT |= MASK_MOTOR_RIGHT_FAST;  // Set LED 20 for right-fast
        }
    }
}

/*
 * See header file
 */
void ah_motor_off(void)
{
    // Clear all motor LEDs (upper 16 bits, LEDs 16-31)
    PORT_OUTPUT &= MASK_MOTOR_OFF;
}


/*
 * See header file
 */
void ah_pump_on(void)
{
    PORT_OUTPUT |= MASK_PUMP;  // Set LED 12 to turn on pump
}

/*
 * See header file
 */
void ah_pump_off(void)
{
    PORT_OUTPUT &= ~MASK_PUMP;  // Clear LED 12 to turn off pump
}
