/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : ESE Lab 2 Energy Measurement
 * -- Description : Toggle between AES calculations, deep sleep
 * --               mode and do nothing to measure the energy
 * --               consumption in the different states.
 * -- 
 * -- $Id:          main.c 2024-08-28 wimd
 * -- $Revision:    1) 20xx-xx-xx Kürzel: "change"$
 * --------------------------------------------------------------- */


/****************************************************************************************
 * Includes
 ***************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <hal_gpio.h>
#include "power_mode.h"

// BEGIN STUDENTS: Add required header files


// END STUDENTS


/****************************************************************************************
 * Macros
 ***************************************************************************************/
#define STM32F429xx
#define CLOCK (CLOCK_168MHZ)


// BEGIN STUDENTS: Global variables initialization


// END STUDENTS

/* -- Local function declarations
 * ------------------------------------------------------------------------- */
static void enable_peripherals(void);
static void init_gpio(void);


/****************************************************************************************
 * @brief main
 ***************************************************************************************/
int main(int argc, char *argv[]) {
	
	/* Enable used peripherals: GPIOs and SYSCFG */
    enable_peripherals();
  
    /* Set unused Ports as output */
    init_gpio();

    /* Setup system clock */
    power_set_clock(CLOCK);

    /* Initialize modules */
    power_init();
	
	// BEGIN STUDENTS: To be programmed 
	
	
	// END STUDENTS
	while(1) {
		// BEGIN STUDENTS: To be programmed 
		
		
		// END STUDENTS
	}
}

/* -- Local function definitions
 * ------------------------------------------------------------------------- */

/**
 *  \brief  Enables all used peripherals.
 */
static void enable_peripherals(void)
{
    PWR_ENABLE();
    GPIOA_ENABLE();
    GPIOB_ENABLE();
    GPIOC_ENABLE();
    GPIOD_ENABLE();
    GPIOE_ENABLE();
    GPIOG_ENABLE();
    SYSCFG_ENABLE();
}

/**
 *  \brief  Set unused Pins as output.
 */
static void init_gpio(void)
{
    /* Initialize gpio pin as output */
    hal_gpio_output_t gpio_output;
  
    gpio_output.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output.pupd = HAL_GPIO_PUPD_NOPULL;
  
    /* Init pins of GPIOA as output. Pins 0xF000 are used for debugging */
    gpio_output.pins = (0x0FFE << 0u);
    hal_gpio_init_output(GPIOA, gpio_output);
  
    /* Init pins of GPIOB as output */
    gpio_output.pins = (0xDFFF << 0u);
    hal_gpio_init_output(GPIOB, gpio_output);
    
    /* Init pins of GPIOC as output */
    gpio_output.pins = (0xFFDF << 0u);
    hal_gpio_init_output(GPIOC, gpio_output);
  
    /* Init pins of GPIOD as output */
    gpio_output.pins = (0xFFFF << 0u);
    hal_gpio_init_output(GPIOD, gpio_output);
    
    /* Init pins of GPIOE as output */
    gpio_output.pins = (0xFFFF << 0u);
    hal_gpio_init_output(GPIOE, gpio_output);
  
}
