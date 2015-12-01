/******************************************************************************
 * @file    timeslot.c 
 * @author  Rémi Pincent - INRIA
 * @date    25 nov. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 * 
 * LICENSE :
 * nrf51_template_application (c) by Rémi Pincent
 * nrf51_template_application is licensed under a
 * Creative Commons Attribution-NonCommercial 3.0 Unported License.
 *
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "timeslot.h"
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "softdevice_handler.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
typedef enum{
	IDLE,
	NOT_IDLE
}ETimeslotSessionStatus;

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
#define UNINITIALIZED_TIMESLOT_LENGTH (NRF_RADIO_LENGTH_MAX_US + 1)

/**************************************************************************
 * Variables
 **************************************************************************/
static nrf_radio_request_t  m_timeslot_request;
static uint32_t             m_slot_length = UNINITIALIZED_TIMESLOT_LENGTH;
static void (*onTimeslotStartCb)(TsNextAction*) = NULL;
static void (*onTimer0TimeslotIRQ)(TsNextAction*) = NULL;
static nrf_radio_signal_callback_return_param_t signal_callback_return_param;
static bool session_idle = true;

/**************************************************************************
 * Static Functions declarations
 **************************************************************************/
static void scheduleNextTimeslot(const TsNextAction*);

/**************************************************************************
 * Functions definitions
 **************************************************************************/

/**@brief Radio event handler
*/
void RADIO_timeslot_IRQHandler(void)
{
    //No implementation needed
}

uint32_t request_next_event_normal(uint32_t timeslot_length)
{
	if(UNINITIALIZED_TIMESLOT_LENGTH == m_slot_length)
	{
		/**
		 * "Note that the first request in a session must always be earliest
		 *  possible to create the timing reference point for later timeslots.
		 *  The application may also request to extend an on-going timeslot.
		 *  Extension requests may be repeated, prolonging the timeslot even further."
		 */
		configure_next_event_earliest(timeslot_length);
	}
	else
	{
		configure_next_event_normal(timeslot_length);
	}
	return sd_radio_request(&m_timeslot_request);
}

/**@brief Request next timeslot event in earliest configuration
 */
uint32_t request_next_event_earliest(uint32_t timeslot_length)
{
	configure_next_event_earliest(timeslot_length);
	return sd_radio_request(&m_timeslot_request);
}

/**@brief Configure next timeslot event in earliest configuration
 */
void configure_next_event_earliest(uint32_t timeslot_length)
{
    m_slot_length                                  = timeslot_length;
    m_timeslot_request.request_type                = NRF_RADIO_REQ_TYPE_EARLIEST;
    m_timeslot_request.params.earliest.hfclk       = NRF_RADIO_HFCLK_CFG_FORCE_XTAL;
    m_timeslot_request.params.earliest.priority    = NRF_RADIO_PRIORITY_NORMAL;
    m_timeslot_request.params.earliest.length_us   = m_slot_length;
    m_timeslot_request.params.earliest.timeout_us  = timeslot_length*10;
}


/**@brief Request next timeslot event in normal configuration
 */
void configure_next_event_normal(uint32_t timeslot_length)
{
    m_slot_length                                 = timeslot_length;
    m_timeslot_request.request_type               = NRF_RADIO_REQ_TYPE_NORMAL;
    m_timeslot_request.params.normal.hfclk        = NRF_RADIO_HFCLK_CFG_FORCE_XTAL;
    m_timeslot_request.params.normal.priority     = NRF_RADIO_PRIORITY_HIGH;
    m_timeslot_request.params.normal.distance_us  = timeslot_length*10;
    m_timeslot_request.params.normal.length_us    = m_slot_length;
}


/**@brief Timeslot signal handler
 */
void nrf_evt_signal_handler(uint32_t evt_id)
{
    uint32_t err_code;
    switch (evt_id)
    {
        case NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN:
            //No implementation needed
            break;
        case NRF_EVT_RADIO_SESSION_IDLE:
        	session_idle = true;
            //No remaining timeslots
            break;
        case NRF_EVT_RADIO_SESSION_CLOSED:
            //No implementation needed, session ended
            break;
        case NRF_EVT_RADIO_BLOCKED:
        case NRF_EVT_RADIO_CANCELED :
            err_code = request_next_event_earliest(m_slot_length);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}

/**@brief Timeslot event handler
 */
nrf_radio_signal_callback_return_param_t * radio_callback(uint8_t signal_type)
{
    switch(signal_type)
    {
        case NRF_RADIO_CALLBACK_SIGNAL_TYPE_START:
        case NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_SUCCEEDED:
            if(onTimeslotStartCb)
            {
            	TsNextAction nextAction = {0};
            	onTimeslotStartCb(&nextAction);
            	scheduleNextTimeslot(&nextAction);
            }
            else
            {
            	/** nothing to do - end timeslot */
                signal_callback_return_param.params.request.p_next = NULL;
                signal_callback_return_param.callback_action = NRF_RADIO_SIGNAL_CALLBACK_ACTION_END;
            }
            break;

        case NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO:
            signal_callback_return_param.params.request.p_next = NULL;
            signal_callback_return_param.callback_action = NRF_RADIO_SIGNAL_CALLBACK_ACTION_NONE;
            RADIO_timeslot_IRQHandler();
            break;

        case NRF_RADIO_CALLBACK_SIGNAL_TYPE_TIMER0:
        	if(onTimer0TimeslotIRQ){
        		TsNextAction nextAction = {0};
        		onTimer0TimeslotIRQ(&nextAction);
        		scheduleNextTimeslot(&nextAction);
            }
            break;

        case NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_FAILED:
            //Try scheduling a new timeslot
            configure_next_event_earliest(m_slot_length);
            signal_callback_return_param.callback_action = NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END;
            signal_callback_return_param.params.request.p_next = &m_timeslot_request;
            break;
        default:
            //No implementation needed
            break;
    }
    return (&signal_callback_return_param);
}


/**@brief Function for initializing the timeslot API.
 */
uint32_t timeslot_sd_init()
{
    uint32_t err_code;

    err_code = sd_radio_session_open(radio_callback);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    return NRF_SUCCESS;
}

void register_timeslot_start_cb(void (*arg_pf_onTimeslotStartCb)(TsNextAction*))
{
	if(onTimeslotStartCb != NULL)
	{
		APP_ERROR_CHECK_BOOL(false);
	}
	onTimeslotStartCb = arg_pf_onTimeslotStartCb;
}

void register_timer0_timeslot_IRQ(void (*arg_pf_onTimer0TimeslotIRQ)(TsNextAction*))
{
	if(onTimer0TimeslotIRQ != NULL)
	{
		APP_ERROR_CHECK_BOOL(false);
	}
	onTimer0TimeslotIRQ = arg_pf_onTimer0TimeslotIRQ;
}

void scheduleNextTimeslot(const TsNextAction* arg_p_nexAction)
{
	signal_callback_return_param.callback_action = arg_p_nexAction->callback_action;
	if(arg_p_nexAction->callback_action == NRF_RADIO_SIGNAL_CALLBACK_ACTION_EXTEND)
	{
		signal_callback_return_param.params.extend.length_us = arg_p_nexAction->extension.slot_length;
	}
	else if(arg_p_nexAction->callback_action == NRF_RADIO_SIGNAL_CALLBACK_ACTION_NONE
			|| arg_p_nexAction->callback_action == NRF_RADIO_SIGNAL_CALLBACK_ACTION_END)
	{
		signal_callback_return_param.params.request.p_next = NULL;
	}
	else if(arg_p_nexAction->callback_action == NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END)
	{
		if(arg_p_nexAction->next_timeslot.request_type == NRF_RADIO_REQ_TYPE_EARLIEST)
		{
			configure_next_event_earliest(arg_p_nexAction->next_timeslot.slot_length);
		}
		else if(arg_p_nexAction->next_timeslot.request_type == NRF_RADIO_REQ_TYPE_NORMAL)
		{
			configure_next_event_normal(arg_p_nexAction->next_timeslot.slot_length);
		}
		else
		{
			assert(false);
		}
		signal_callback_return_param.params.request.p_next = &m_timeslot_request;
	}
	else
	{
		assert(false);
	}
}


