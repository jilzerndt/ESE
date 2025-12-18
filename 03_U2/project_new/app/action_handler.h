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


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initializes the outport.
 */
void action_handler_init(void);

void action_handler_fillWater(void);
void action_handler_heatWater(void);
void action_handler_rotateRight(void);
void action_handler_rotateLeft(void);
void action_handler_emptyWater(void);
void action_handler_spinDry(void);


/*
 * Writes a string at the given position.
 */
void ah_lcd_write(char text[]);

#endif
