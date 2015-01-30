/******************************************************************************
 * @file    application_config.h
 * @author  Rémi Pincent - INRIA
 * @date    14 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

#include <stdint.h>

/********************************
 *    Application timers        *
 ********************************/

/**< Value of the RTC1 PRESCALER register. No prescaling => 1 tick = 1/32768Hz = 30.517us*/
const uint32_t APP_TIMER_PRESCALER         = 0;
/**< Maximum number of simultaneously created timers. */
const uint8_t APP_TIMER_MAX_TIMERS         = 4;
/**< Size of timer operation queues. */
const uint8_t APP_TIMER_OP_QUEUE_SIZE      = 4;

/************************************
 *    Application scheduling        *
 ************************************/
/**< Application uses event scheduler. */
const bool USE_EVENT_SCHEDULER              = true;

/**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
const uint16_t SCHED_MAX_EVENT_DATA_SIZE    = 0x08;
const uint16_t SCHEDULER_QUEUE_SIZE         = 0x10;

/************************************
 *    Application wireless comm     *
 ************************************/

#endif /* APPLICATION_APPLICATION_CONFIGURATION_H_ */
