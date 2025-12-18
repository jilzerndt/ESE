/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Interface of module state_machine.
 * --
 * -- Reacts on events and triggers actions.
 * --
 * -- $Id: state_machine.h 1991 2015-04-29 04:43:29Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

/* standard includes */
#include <stdint.h>

/* user includes */
#include "event_handler.h"


/* -- Type definitions
 * ------------------------------------------------------------------------- */

typedef enum {
    OPENED,
    // add your states
    CLOSED,
	STOPPING,
	FILLING,
	HEATING,
	AGITATE_RIGHT,
	AGITATE_LEFT,
	DRAIN,
	SPIN
} state_t;



/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/*
 * Initialize the state machine
 */
void fsm_init(void);


/*
 * Process the given event, based on actual state.
 */
void fsm_handle_event(event_t event);
#endif
