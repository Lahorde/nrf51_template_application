/******************************************************************************
 * @file    variant.cpp 
 * @author  Rémi Pincent - INRIA
 * @date    22 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#include "variant.h"

uint32_t Pin_nRF51822_to_Arduino(uint32_t pin)
{
	/** check pin */
	if(0 < pin < PINS_COUNT)
	{
		return pin;
	}
	else
	{
		/** bad pin given */
		return INVALID_PIN;
	}
}

