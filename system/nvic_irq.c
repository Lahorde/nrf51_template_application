/******************************************************************************
 * @file    nvic_irq.c 
 * @author  Rémi Pincent - INRIA
 * @date    31 oct. 2014   
 *
 * @brief NVIC vectors
 * 
 * Project : wimu
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nvic_irq.h"
#include "nrf_soc.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Global Functions Declarations
 **************************************************************************/

/**
 * \brief Default dynamic interrupt handler for unused IRQs.
 */
extern void Default_Dynamic_Handler(void);

/**
 * Handler called when vector has not been statically defined. Refer starup_nrf51.s
 */
extern void Dynamic_Handler(void);

/**************************************************************************
 * Variables
 **************************************************************************/
extern int BLE_used;

/* exception number mirrors vector table */
dynamic_handler_t exception_handlers[NB_NRF51_PERIPHERAL_EXCEPTIONS + NB_CORTEX_CORE_EXCEPTIONS] =
{
    0,                        /* No Exception - Executing Thread */
	Default_Dynamic_Handler,  /* Reset */
	Default_Dynamic_Handler,  /* NMI */
	Default_Dynamic_Handler,  /* Hard Fault */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
	Default_Dynamic_Handler,  /* SVC */
    0,                        /* Reserved */
    0,                        /* Reserved */
	Default_Dynamic_Handler,  /* PendSV */
	Default_Dynamic_Handler   /* SysTick */
};

/* interrupt number: negative for non-configuable interrupts, positive for configurable interrupts */
extern dynamic_handler_t *dynamic_handlers = &exception_handlers[NB_CORTEX_CORE_EXCEPTIONS];
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

void nvic_enable_irq(IRQn_Type irq_num)
{
    if (BLE_used == 0) {
        NVIC_EnableIRQ(irq_num);
    } else if (BLE_used == 1) {
        sd_nvic_EnableIRQ(irq_num);
    }
    nvic_set_priority(irq_num, 3);
}

void nvic_disable_irq(IRQn_Type irq_num)
{
    if (BLE_used == 0) {
        NVIC_DisableIRQ(irq_num);
    } else if (BLE_used == 1) {
        sd_nvic_DisableIRQ(irq_num);
    }
}

void nvic_set_priority(IRQn_Type irq_num, uint32_t priority)
{
    if (BLE_used == 0) {
        NVIC_SetPriority(irq_num, priority);
    } else if (BLE_used == 1) {
        sd_nvic_SetPriority(irq_num, priority);
    }
}

void Default_Dynamic_Handler(void)
{
    while (1)
      ;
}

void Dynamic_Handler(void)
{
	// call the dynamic exception handler registered for the exception number of the the active vector
	// field of the interrupt control and state register of the system control block
	exception_handlers[SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk]();
}
