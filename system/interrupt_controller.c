/******************************************************************************
 * @file    interrupt_controller.c
 * @author  Rémi Pincent - INRIA
 * @date    31 oct. 2014   
 *
 * @brief Interrupt controller
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
#include <stddef.h>
#include "interrupt_controller.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "app_error.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
//First 15 exception entries that occurs in Cortex Core
#define NB_CORTEX_CORE_EXCEPTIONS      16
#define NB_NRF51_PERIPHERAL_EXCEPTIONS 32

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
dynamic_handler_t *dynamic_handlers = &exception_handlers[NB_CORTEX_CORE_EXCEPTIONS];
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/
bool IntController_enableIRQ(IRQn_Type arg_IRQn, uint32_t arg_priority)
{
	uint8_t softdevice_enabled;
	uint32_t err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if(softdevice_enabled == 0)
	{	NVIC_ClearPendingIRQ(arg_IRQn);
		NVIC_SetPriority(arg_IRQn, arg_priority);
		NVIC_EnableIRQ(arg_IRQn);
	}
	else
	{
		err_code = sd_nvic_ClearPendingIRQ(arg_IRQn);
		if(err_code != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(err_code);
			return false;
		}

		err_code = sd_nvic_SetPriority(arg_IRQn, arg_priority);
		if(err_code != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(err_code);
			return false;
		}

		err_code = sd_nvic_EnableIRQ(arg_IRQn);
		if(err_code != NRF_SUCCESS)
		{
			APP_ERROR_CHECK(err_code);
			return false;
		}
	}
	return true;
}

void IntController_linkInterrupt( uint8_t IRQn, dynamic_handler_t handler)
{
	dynamic_handlers[IRQn] = handler;
}

void IntController_unlinkInterrupt( uint8_t IRQn )
{
	dynamic_handlers[IRQn] = NULL;
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
