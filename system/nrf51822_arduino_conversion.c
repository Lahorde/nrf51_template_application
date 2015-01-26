/******************************************************************************
 * @file    nrf51822_arduino_conversion.c 
 * @author  Rémi Pincent - INRIA
 * @date    26 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nrf51822_arduino_conversion.h"
#include "assert.h"
/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**************************************************************************
 * Global Functions Definitions
 **************************************************************************/
nrf_gpio_pin_pull_t inputPinModeToNRF51Pull(EPinMode arg_e_pinMode)
{
	nrf_gpio_pin_pull_t loc_e_pinPull = NRF_GPIO_PIN_NOPULL;
	if(arg_e_pinMode == INPUT || arg_e_pinMode == INPUT_NOPULL)
	{
		loc_e_pinPull = NRF_GPIO_PIN_NOPULL;
	}
	else if(arg_e_pinMode == INPUT_PULLDOWN)
	{
		loc_e_pinPull = NRF_GPIO_PIN_PULLDOWN;
	}
	else if(arg_e_pinMode == INPUT_PULLUP)
	{
		loc_e_pinPull = NRF_GPIO_PIN_PULLUP;
	}
	else
	{
		assert(false);
	}
	return loc_e_pinPull;
}

uint32_t pinModeToNRF51Drive(EPinMode arg_e_pinMode)
{
	uint32_t loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_S0S1;
	if(arg_e_pinMode == OUTPUT || arg_e_pinMode == OUTPUT_H0H1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_H0H1;
	}
	else if(arg_e_pinMode == OUTPUT_S0S1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_S0S1;
	}
	else if(arg_e_pinMode == OUTPUT_H0S1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_H0S1;
	}
	else if(arg_e_pinMode == OUTPUT_S0H1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_S0H1;
	}
	else if(arg_e_pinMode == OUTPUT_D0S1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_D0S1;
	}
	else if(arg_e_pinMode == OUTPUT_D0H1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_D0H1;
	}
	else if(arg_e_pinMode == OUTPUT_S0D1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_S0D1;
	}
	else if(arg_e_pinMode == OUTPUT_H0D1)
	{
		loc_u32_pinDrive = GPIO_PIN_CNF_DRIVE_H0D1;
	}
	else
	{
		assert(false);
	}
	return loc_u32_pinDrive;
}

nrf_gpiote_polarity_t pinTriggerToNRF51GPIOTEPol(EPinTrigger arg_e_pinTrigger)
{
	nrf_gpiote_polarity_t loc_e_gpiotePol = NRF_GPIOTE_POLARITY_TOGGLE;
	if(arg_e_pinTrigger == CHANGE)
	{
		loc_e_gpiotePol = NRF_GPIOTE_POLARITY_TOGGLE;
	}
	else if(arg_e_pinTrigger == RISING)
	{
		loc_e_gpiotePol = NRF_GPIOTE_POLARITY_LOTOHI;
	}
	else if(arg_e_pinTrigger == FALLING)
	{
		loc_e_gpiotePol = NRF_GPIOTE_POLARITY_HITOLO;
	}
	else
	{
		assert(false);
	}
	return loc_e_gpiotePol;
}

/**************************************************************************
 * Local Functions Definitions
 **************************************************************************/

