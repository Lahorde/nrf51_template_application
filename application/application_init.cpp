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

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
#define APP_GPIOTE_MAX_USERS            1                                           /**< Maximum number of users of the GPIOTE handler. */
#define RTC1_CYCLE_TIME                      APP_TIMER_TICKS(120000, 0)
/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/
static app_timer_id_t                        m_rtc_start_timer_id;
extern const uint32_t FREE_MEM_PATTERN;                                            /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions declarations
 **************************************************************************/

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void timers_init(void);

/** Start LFCLK in order to use RTC */
static void lfclk_init(void);

/**@brief Function for starting timers.
*/
static void timers_start(void);

static void m_rtc_start_handle(void * p_context);

/**************************************************************************
 * Global Functions
 **************************************************************************/
// Arduino support
extern void application_setup(void);
extern void application_loop(void);

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

/**************************************************************************
 * Global Functions definitions
 **************************************************************************/

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	lfclk_init();
    timers_init();
    timers_start();
    //Arduino initialization, LFCLK and timers must have been initialized before
    application_setup();

    // Enter main loop
    for (;;)
    {
        //application loop
        application_loop();
    }
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(FREE_MEM_PATTERN, line_num, p_file_name);
}

/**************************************************************************
 * Local Functions implementations
 **************************************************************************/

static void timers_init(void)
{
    // Initialize timer module, making it use the scheduler
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, USE_EVENT_SCHEDULER);
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

static void m_rtc_start_handle(void * p_context)
{
	//UNUSED_PARAMETER(p_context);
}

static void timers_start(void)
{
	uint32_t err_code;

	//RP - 22/01/2015 - Application timer started in application_init.cpp

	err_code = app_timer_create(&m_rtc_start_timer_id,
					 APP_TIMER_MODE_REPEATED,
					 m_rtc_start_handle);
	APP_ERROR_CHECK(err_code);

	err_code = app_timer_start(m_rtc_start_timer_id, RTC1_CYCLE_TIME, NULL);
    APP_ERROR_CHECK(err_code);
}
