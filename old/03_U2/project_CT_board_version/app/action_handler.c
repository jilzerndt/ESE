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

#define MASK_DOOR_LOCK          (0x00000001);
#define MASK_VALVE              (0x00000010);
#define MASK_HEATER             (0x00000100);
#define MASK_PUMP               (0x00001000);
#define MASK_MOTOR_RIGHT_SLOW   (0x00010000);
#define MASK_MOTOR_RIGHT_FAST   (0x00100000);
#define MASK_MOTOR_LEFT_SLOW    (0x01000000);
#define MASK_MOTOR_LEFT_FAST    (0x10000000);
#define MASK_MOTOR_OFF          (0x0000FFFF);


/* Public function definitions
 * ------------------------------------------------------------------------- */
/*
 * See header file
 */
void action_handler_init(void)
{
    /// STUDENTS: To be programmed

    PORT_OUTPUT = 0;
	
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


void ah_lock_door()
{
    PORT_OUTPUT |= MASK_DOOR_LOCK;
}

void ah_unlock_door()
{
    PORT_OUTPUT &= ~MASK_DOOR_LOCK;
}


void ah_open_valve()
{
    PORT_OUTPUT |= MASK_VALVE;
}

void ah_close_valve()
{
    PORT_OUTPUT &= ~MASK_VALVE;
}


void ah_heater_on()
{
    PORT_OUTPUT |= MASK_HEATER;
}

void ah_heater_off()
{
    PORT_OUTPUT &= ~MASK_HEATER;
}


void ah_pump_on()
{
    PORT_OUTPUT |= MASK_PUMP;
}

void ah_pump_off()
{
    PORT_OUTPUT &= ~MASK_PUMP;
}


void ah_motor_on(motor_direction_t motor_direction, motor_speed_t motor_speed)
{
    if(motor_direction == LEFT)
    {
        if(motor_speed == SLOW)
        {
            PORT_OUTPUT |= MASK_MOTOR_LEFT_SLOW;
        } 
        else
        {
            PORT_OUTPUT |= MASK_MOTOR_LEFT_FAST;
        }
    }
    else
    {
        if(motor_speed == SLOW)
        {
            PORT_OUTPUT |= MASK_MOTOR_RIGHT_SLOW;
        } 
        else
        {
            PORT_OUTPUT |= MASK_MOTOR_RIGHT_FAST;
        }
    }
}

void ah_motor_off()
{
    PORT_OUTPUT &= MASK_MOTOR_OFF;
}
