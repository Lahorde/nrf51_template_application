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
#include "assert.h"

uint32_t arduinoToVariantPin(uint32_t arg_u32_pin)
{
	/** check pin */
	if(0 < arg_u32_pin && arg_u32_pin < PINS_COUNT)
	{
		return arg_u32_pin;
	}
	else
	{
		/** bad pin given */
		return INVALID_PIN;
	}
}
