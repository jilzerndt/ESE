/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module event_handler.
 * --
 * -- Generates events based on rising or falling edges on inport.
 * --
 * -- $Id: event_handler.h 1991 2015-04-29 04:43:29Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

/* standard includes */
#include <stdint.h>


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    NO_EVENT,
    TIME_OUT,
    // List events needed for your implementation
    DOOR_OPENED,
    DOOR_CLOSED,
    BUTTON_WASH,
    BUTTON_SPIN,
    BUTTON_STOP,
    FLOATER_HIGH,
    FLOATER_LOW,
    TEMPERATURE_HOT
} event_t;


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Check inport and timer and generate event
 */
event_t eh_get_event(void);
#endif
