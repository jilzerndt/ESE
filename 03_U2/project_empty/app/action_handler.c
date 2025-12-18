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
