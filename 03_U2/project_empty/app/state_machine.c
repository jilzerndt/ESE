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

#define TIMER_DURATION_LEFT  400u
#define TIMER_DURATION_RIGHT 400u
#define TIMER_DURATION_SPIN  500u

#define TEXT_OPENED          "OPENED         "


/* Local variables
 * ------------------------------------------------------------------------- */

static state_t state = OPENED;


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void fsm_init(void)
{
    action_handler_init();
    ah_lcd_write(TEXT_OPENED);
    state = OPENED;
}


/*
 * See header file
 */
void fsm_handle_event(event_t event)
{
    // State machine implementation according to your model from last week's exercise.
}
