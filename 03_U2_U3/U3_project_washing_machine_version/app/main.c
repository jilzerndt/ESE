/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 lab - FSM washing machine
 * -- Description : Main program.
 * --
 * --               Task 1: -
 * --                       -
 * --               Task 2: -
 * --                       -
 * --               Task 3: -
 * --
 * --
 * -- $Id: main.c 4794 2019-05-03 12:47:54Z mady $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <reg_ctboard.h>

/* user includes */
#include "event_handler.h"
#include "state_machine.h"
#include "timer.h"




/* -- Macros
 * ------------------------------------------------------------------------- */

#define PORT_INPUT (CT_GPIO->IN.BYTE.P1)


/* -- M A I N
 * ------------------------------------------------------------------------- */

int main(void)
{
    /// STUDENTS: To be programmed

    // Initialize state machine and timer
    fsm_init();
    timer_init();

    // Main event loop
    while (1) {
        fsm_handle_event(eh_get_event());
    }

    /// END: To be programmed
}

