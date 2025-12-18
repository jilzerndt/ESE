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

// #define PORT_OUTPUT (CT_GPIO->OUT.BYTE.P1)
#define PORT_OUTPUT (CT_LED->BYTE.LED7_0)


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
    PORT_OUTPUT = ~(0xff);
    /// STUDENTS: To be programmed
    ah_lcd_write("waiting for command, my master");

	
    /// END: To be programmed
}

void action_handler_fillWater(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_VALVE_OPEN);
    ah_lcd_write("filling water");
}

void action_handler_heatWater(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_HEATER_ON);
    ah_lcd_write("heating water");
}

void action_handler_rotateRight(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_MOTOR_SLOW & MASK_MOTOR_RIGHT);
    ah_lcd_write("rotating right for  4 seconds");
}

void action_handler_rotateLeft(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_MOTOR_SLOW); //rotate left active high
    ah_lcd_write("rotating left for   4 seconds");
}

void action_handler_emptyWater(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_PUMP_ON);
    ah_lcd_write("emptying water");
}

void action_handler_spinDry(void)
{
    PORT_OUTPUT = ~(MASK_DOOR_LOCK & MASK_MOTOR_FAST);
    ah_lcd_write("spinning");
}


/*
 * See header file
 */
void ah_lcd_write(char text[])
{
    hal_ct_lcd_clear();
    hal_ct_lcd_write(0, text);
}
