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
#ifndef APP_TIMER_PRESCALER
#define APP_TIMER_PRESCALER  (0U)
#endif

/**< Maximum number of simultaneously created timers. */
#ifndef APP_TIMER_MAX_TIMERS
#define APP_TIMER_MAX_TIMERS (4U)
#endif

/**< Size of timer operation queues. */
#ifndef APP_TIMER_OP_QUEUE_SIZE
#define APP_TIMER_OP_QUEUE_SIZE (4U)
#endif

/************************************
 *    Application scheduling        *
 ************************************/
/**< Application uses event scheduler. */
#ifndef USE_EVENT_SCHEDULER
#define USE_EVENT_SCHEDULER true
#endif

/**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#ifndef SCHED_MAX_EVENT_DATA_SIZE
#define SCHED_MAX_EVENT_DATA_SIZE (0x08U)
#endif

#ifndef SCHEDULER_QUEUE_SIZE
#define SCHEDULER_QUEUE_SIZE (0x10U)
#endif

/************************************
 *    Application wireless comm     *
 ************************************/

#endif /* APPLICATION_APPLICATION_CONFIGURATION_H_ */
