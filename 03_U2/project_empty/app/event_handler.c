/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module event_handler.
 * --
 * -- $Id: event_handler.c 5605 2023-01-05 15:52:42Z frtt $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <reg_ctboard.h>

/* user includes */
#include "event_handler.h"
#include "timer.h"




/* -- Macros for accessing CT Board
 * ------------------------------------------------------------------------- */

#define PORT_INPUT (CT_GPIO->IN.BYTE.P1)

/* -- Macros to mask input ports
 * ------------------------------------------------------------------------- */

#define MASK_FLOATER_HIGH     (0x01)
#define MASK_FLOATER_LOW      (0x04)
#define MASK_BUTTON_STOP      (0x08)
#define MASK_BUTTON_SPIN      (0x10)
#define MASK_BUTTON_WASH      (0x20)
#define MASK_THERMOSTAT       (0x40)
#define MASK_DOOR_OPEN        (0x80)
#define MASK_CLEAR_INPUTPIN_1 (0xFD)

/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
event_t eh_get_event(void)
{
    event_t event = NO_EVENT;

    static uint8_t port_value_old = 0xff;
    uint8_t port_value;
    uint8_t port_value_control;
    uint8_t edge_positive = 0;
    uint8_t edge_negative = 0;

    static uint16_t timer_old = 0u;
    uint16_t timer;

    /* Read the input port */
    port_value = PORT_INPUT & MASK_CLEAR_INPUTPIN_1;
    port_value_control = PORT_INPUT & MASK_CLEAR_INPUTPIN_1;

    /* Compare the input to ignore spikes */
    /* Detect edges: Set the corresponding bit for each detected edge */
    if (port_value == port_value_control) {
        edge_positive = ~port_value_old & port_value;
        edge_negative = ~(~port_value_old | port_value);
        port_value_old = port_value;
    }

    /* Get the current timer value */
    timer = timer_read();

    /// Add here the events that will be needed for your washing machine




    /// END: To be programmed

    /* Update timer_old */
    timer_old = timer;

    return event;
}
