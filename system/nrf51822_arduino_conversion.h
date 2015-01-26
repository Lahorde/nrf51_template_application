/******************************************************************************
 * @file    nrf51822_arduino_conversion.h 
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
#ifndef SYSTEM_NRF51822_ARDUINO_CONVERSION_H_
#define SYSTEM_NRF51822_ARDUINO_CONVERSION_H_

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif
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

/**
 * Convert given pin mode to nRF51822 pull mode
 * @param arg_e_pinMode
 * @return
 */
nrf_gpio_pin_pull_t inputPinModeToNRF51Pull(EPinMode arg_e_pinMode);

/**
 * Convert given pin mode to nRF51822 drive (refer reference manual section 13.2.8)
 * @param arg_e_pinMode
 * @return
 */
uint32_t pinModeToNRF51Drive(EPinMode arg_e_pinMode);

/**
 * Convert given trigger to GPIOTE polarity (refer reference manual section 14.2.1)
 * @param arg_e_pinTrigger
 * @return
 */
nrf_gpiote_polarity_t pinTriggerToNRF51GPIOTEPol(EPinTrigger arg_e_pinTrigger);

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_NRF51822_ARDUINO_CONVERSION_H_ */
