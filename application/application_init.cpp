/******************************************************************************
 * @file    main.c
 * @author  Rémi Pincent - INRIA
 * @date    29 oct. 2014
 *
 * @brief main class for WIMU application
 * written from https://github.com/NordicSemiconductor/nrf51-ble-app-lbs
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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "application_config.h"

//RP 30/10/2014.... TODO check all headers
extern "C" {
#include "softdevice_handler.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "app_button.h"
#include "ble_conn_params.h"
#include "pstorage.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_gpiote.h"
#include "ble_debug_assert_handler.h"

}

// Arduino support
#include "wiring.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */

#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */

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
 * Local Functions declarations
 **************************************************************************/

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);

extern "C" {
/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void timers_init(void);

static void timer_timeout_handler(void * p_context);

/**@brief Function for the Event Scheduler initialization.
 */
static void scheduler_init(void);

/**@brief Function for initializing the GPIOTE handler module.
 */
static void gpiote_init(void);

/** Start LFCLK in order to use RTC */
static void lfclk_init(void);

/**@brief Function for starting timers.
*/
static void timers_start(void);

/**@brief Function for the Power manager.
 */
static void power_manage(void);

/**************************************************************************
 * Global Functions
 **************************************************************************/
// Arduino support
//RP - 15/01/2015 - missed something, using setup(), loop() leads to undefined ref...
extern void application_setup(void);
extern void application_loop(void);

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	lfclk_init();
    timers_init();
    //RP - 15/01/2015 - TODO - activate GPIOTE?
    //gpiote_init();
    scheduler_init();

    //Arduino initialization, LFCLK and timers must have been initialized before
    init();
    application_setup();

    // Start execution
    timers_start();

    // Enter main loop
    for (;;)
    {
        app_sched_execute();
        //power_manage();

        //application loop
        application_loop();
    }
}


/**************************************************************************
 * Local Functions implementations
 **************************************************************************/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    //NVIC_SystemReset();
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

static void timers_init(void)
{
    // Initialize timer module, making it use the scheduler
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, USE_EVENT_SCHEDULER);
}

static void timer_timeout_handler(void * p_context) {
    return;
}

static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void lfclk_init(void)
{
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Synth << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        //Do nothing.
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}

static void gpiote_init(void)
{
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}

static void timers_start(void)
{
}

static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

extern "C" void __cxa_pure_virtual()
{
	//pure virtual function called
  while (1);
}
