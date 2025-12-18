/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Interface of module wakeup timer.
 *
 *  $Id: wakeup_timer.h 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _WAKEUP_TIMER_H
#define _WAKEUP_TIMER_H


/* Standard includes */
#include <hal_common.h>


/* -- Public function declarations
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Initializes the real time clock.
 *  \param  count: wakeup timer count register
 *                 16 kHz : x -> 1 Hz (0x3E80 -> 1000 ms)
 */
void wakeup_init(uint32_t count_register);

#endif
