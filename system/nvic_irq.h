/******************************************************************************
 * @file    nvic_handler.h 
 * @author  Rémi Pincent - INRIA
 * @date    31 oct. 2014   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : wimu
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef NVIC_IRQ_H_
#define NVIC_IRQ_H_

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nrf51.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
//First 15 exception entries that occurs in Cortex Core
#define NB_CORTEX_CORE_EXCEPTIONS      16
#define NB_NRF51_PERIPHERAL_EXCEPTIONS 32
/**************************************************************************
 * Type Definitions
 **************************************************************************/
//RP - 31/10/2014 place dynamic handler definition here instead of modifying .s file
typedef void (*dynamic_handler_t)(void);
typedef dynamic_handler_t pin_callback_t;

/**************************************************************************
 * Global Functions Declarations
 **************************************************************************/
void nvic_enable_irq(IRQn_Type irq_num);
void nvic_disable_irq(IRQn_Type irq_num);
void nvic_set_priority(IRQn_Type irq_num, uint32_t priority);


#endif /* NVIC_IRQ_H_ */
