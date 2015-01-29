/******************************************************************************
 * @file    interrupt_controller.h
 * @author  Rémi Pincent - INRIA
 * @date    31 oct. 2014   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef INTERRUPT_CONTROLLER_H_
#define INTERRUPT_CONTROLLER_H_

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nrf51.h"

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/
//RP - 31/10/2014 place dynamic handler definition here instead of modifying .s file
typedef void (*dynamic_handler_t)(void);

/**************************************************************************
 * Global Functions Declarations
 **************************************************************************/
void linkInterrupt( uint8_t IRQn, dynamic_handler_t handler);
void unlinkInterrupt( uint8_t IRQn );

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_CONTROLLER_H_ */
