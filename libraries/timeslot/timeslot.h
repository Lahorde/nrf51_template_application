/******************************************************************************
 * @file    timeslot.h 
 * @author  Rémi Pincent - INRIA
 * @date    25 nov. 2015   
 *
 * @brief handle timeslots on nrf51 in order to get full access
 * 
 * Project : nrf51_arduino_core
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Modified from :
 * https://devzone.nordicsemi.com/tutorials/16/
 * 
 * LICENSE :
 * nrf51_arduino_core (c) by Rémi Pincent
 * nrf51_arduino_core is licensed under a
 * Creative Commons Attribution-NonCommercial 3.0 Unported License.
 *
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
 *****************************************************************************/
#ifndef TIMESLOT_H__
#define TIMESLOT_H__

/**************************************************************************
 * Include Files
 **************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/
typedef struct{
	uint32_t slot_length;
	uint8_t request_type;     /**< Type of request, see @ref NRF_RADIO_REQUEST_TYPE. */
	uint8_t callback_action;  /**< The action requested by the application when returning from the signal callback, see @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION. */
}TsNextAction;
/**************************************************************************
 * Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

/**@brief Radio event handler
*/
void RADIO_timeslot_IRQHandler(void);


/**@brief Request next timeslot event in earliest configuration
 */
uint32_t request_next_event_earliest(uint32_t timeslot_length);

/**@brief Request next timeslot event in normal configuration
 */
uint32_t request_next_event_normal(uint32_t timeslot_length);

/**@brief Configure next timeslot event in earliest configuration
 */
void configure_next_event_earliest(uint32_t timeslot_length);


/**@brief Configure next timeslot event in normal configuration
 */
void configure_next_event_normal(uint32_t timeslot_length);


/**@brief Timeslot signal handler
 */
void nrf_evt_signal_handler(uint32_t evt_id);


/**@brief Timeslot event handler
 */
nrf_radio_signal_callback_return_param_t * radio_callback(uint8_t signal_type);


/**@brief Function for initializing the timeslot API.
 */
uint32_t timeslot_sd_init();

/**
 * Callback called when timeslot starts
 * @param onTimeslotStartCb callback returning next action to perform from type @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION
 */
void register_timeslot_start_cb(void (*onTimeslotStartCb)(TsNextAction*));

/**
 * Callback called when Timer0 interrupt get
 * @param onTimer0TimeslotIRQ callback returning next action to perform from type @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION
 */
void register_timer0_timeslot_IRQ(void (*onTimer0TimeslotIRQ)(TsNextAction*));

#endif
