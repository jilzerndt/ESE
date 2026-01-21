/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module action_handler.
 * --
 * -- Sets or clears pins on outport.
 * --
 * -- $Id: action_handler.h 1991 2015-04-29 04:43:29Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _ACTION_HANDLER_H
#define _ACTION_HANDLER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    LEFT,
    RIGHT
} motor_direction_t;

typedef enum {
    SLOW,
    FAST
} motor_speed_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initializes the outport.
 */
void action_handler_init(void);


/*
 * Writes a string at the given position.
 */
void ah_lcd_write(char text[]);


/*
 * Lock the door.
 */
void ah_lock_door(void);

/*
 * Unlock the door.
 */
void ah_unlock_door(void);


/*
 * Open the water valve.
 */
void ah_open_valve(void);

/*
 * Close the water valve.
 */
void ah_close_valve(void);


/*
 * Turn heater on.
 */
void ah_heater_on(void);

/*
 * Turn heater off.
 */
void ah_heater_off(void);


/*
 * Turn motor on with specified direction and speed.
 */
void ah_motor_on(motor_direction_t motor_direction, motor_speed_t motor_speed);

/*
 * Turn motor off.
 */
void ah_motor_off(void);


/*
 * Turn pump on.
 */
void ah_pump_on(void);

/*
 * Turn pump off.
 */
void ah_pump_off(void);

#endif
