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
uint32_t request_next_event_earliest(void);


/**@brief Configure next timeslot event in earliest configuration
 */
void configure_next_event_earliest(void);


/**@brief Configure next timeslot event in normal configuration
 */
void configure_next_event_normal(void);


/**@brief Timeslot signal handler
 */
void nrf_evt_signal_handler(uint32_t evt_id);


/**@brief Timeslot event handler
 */
nrf_radio_signal_callback_return_param_t * radio_callback(uint8_t signal_type);


/**@brief Function for initializing the timeslot API.
 */
uint32_t timeslot_sd_init(void);

void registerOnTimeslotStartCb(void (*onTimeslotStartCb)(void));
void registerOnTimer0TimeslotIRQ(bool (*onTimer0TimeslotIRQ)(void));

#endif
